/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <mutex>
#include <condition_variable>
#include "transaction/transaction.h"
#include "transaction/txn_defs.h"
#define Queue(id) lock_table_[id].request_queue_
#define Group_mode(id) lock_table_[id].group_lock_mode_

static const std::string GroupLockModeStr[10] = {"NON_LOCK", "IS", "IX", "S", "X", "SIX"};

class LockManager {
    /* 加锁类型，包括共享锁、排他锁、意向共享锁、意向排他锁、SIX（意向排他锁+共享锁） */
    enum class LockMode { SHARED, EXLUCSIVE, INTENTION_SHARED, INTENTION_EXCLUSIVE, S_IX };

    /* 用于标识加锁队列中排他性最强的锁类型，例如加锁队列中有SHARED和EXLUSIVE两个加锁操作，则该队列的锁模式为X */
    enum class GroupLockMode { NON_LOCK, IS, IX, S, X, SIX};

    /* 事务的加锁申请 */
    class LockRequest {
    public:
        LockRequest(txn_id_t txn_id, LockMode lock_mode)
            : txn_id_(txn_id), lock_mode_(lock_mode), granted_(false) {}

        txn_id_t txn_id_;   // 申请加锁的事务ID
        LockMode lock_mode_;    // 事务申请加锁的类型
        bool granted_;          // 该事务是否已经被赋予锁
    };

    /* 数据项上的加锁队列 */
    class LockRequestQueue {
    public:
        std::list<std::shared_ptr<LockRequest>> request_queue_;  // 加锁队列
        std::condition_variable cv_;   // 条件变量，用于唤醒正在等待加锁的申请，在no-wait策略下无需使用
        GroupLockMode group_lock_mode_ = GroupLockMode::NON_LOCK;   // 加锁队列的锁模式
    };

public:
    LockManager() {}

    ~LockManager() {}

    bool lock_shared_on_record(std::shared_ptr<Transaction> txn, const Rid& rid, int tab_fd);

    bool lock_exclusive_on_record(std::shared_ptr<Transaction> txn, const Rid& rid, int tab_fd);

    bool lock_shared_on_table(std::shared_ptr<Transaction> txn, int tab_fd);

    bool lock_exclusive_on_table(std::shared_ptr<Transaction> txn, int tab_fd);

    bool lock_IS_on_table(std::shared_ptr<Transaction> txn, int tab_fd);

    bool lock_IX_on_table(std::shared_ptr<Transaction> txn, int tab_fd);

    bool lock_SIX_on_table(std::shared_ptr<Transaction> txn, int tab_fd);

    bool unlock(std::shared_ptr<Transaction> txn, LockDataId lock_data_id);

private:
    bool check(LockDataId id, std::shared_ptr<Transaction> txn, LockMode mode)
    {
        switch (Group_mode(id))
        {
        case GroupLockMode::NON_LOCK:
            Group_mode(id) = lock_trans(mode);
            return true;
        case GroupLockMode::IS:
            if (mode != LockMode::EXLUCSIVE){
                Group_mode(id) = lock_trans(mode);
                return true;
            }
            break;
        case GroupLockMode::IX:
            if (mode == LockMode::INTENTION_EXCLUSIVE || mode == LockMode::INTENTION_SHARED)
                return true;
            break;
        case GroupLockMode::S:
            if (mode == LockMode::SHARED || mode == LockMode::INTENTION_SHARED)
                return true;
            break;
        case GroupLockMode::SIX:
            if (mode == LockMode::INTENTION_SHARED)
                return true;
            break;
        case GroupLockMode::X:
            return false;
        }
        return false;
    };

    GroupLockMode lock_trans(LockMode lock_mode)
    {
        GroupLockMode ret = GroupLockMode::NON_LOCK;
        switch (lock_mode)
        {
        case LockMode::SHARED:
            ret = GroupLockMode::S;
            break;
        case LockMode::EXLUCSIVE:
            ret = GroupLockMode::X;
            break;
        case LockMode::INTENTION_SHARED:
            ret = GroupLockMode::IS;
            break;
        case LockMode::INTENTION_EXCLUSIVE:
            ret = GroupLockMode::IX;
            break;
        case LockMode::S_IX:
            ret = GroupLockMode::SIX;
            break;
        }
        return ret;
    };

    bool lock_general(LockDataId id, std::shared_ptr<Transaction> txn, LockMode mode){
        std::unique_lock<std::mutex> lock(latch_);
        // 检查是否已经获得了该数据项的锁
        if (txn->get_lock_set()->find(id) != txn->get_lock_set()->end()) {
            auto it = Queue(id).begin();
            for( ; it != Queue(id).end(); it++) {
                if ((*it)->txn_id_ == txn->get_transaction_id()) {
                    break;
                }
            }
            if ((*it)->lock_mode_ == mode) // 已经持有了该锁
                return true;
            else if ((*it)->lock_mode_ == LockMode::EXLUCSIVE) // 如果持有排它锁，则无须获取其他锁
                return true;
            // 对于整张表，如果持有SIX锁，则无须重新获取除排它锁以外的任何锁
            else if (id.type_ == LockDataType::TABLE && (*it)->lock_mode_ == LockMode::S_IX && mode != LockMode::EXLUCSIVE)
                return true;
            // 其他情况都需要重新获取锁
            else if (Queue(id).size() == 1)
            {
                (*it)->lock_mode_ = mode;
                Group_mode(id) = lock_trans(mode);
                return true;
            }
            else
            {
                Queue(id).erase(it);
                txn->get_lock_set()->erase(id);
                Group_mode(id) = lock_trans(Queue(id).front()->lock_mode_);
            }
        }
        // 更改事务的状态
        txn->set_state(TransactionState::GROWING);

        // 封装请求
        auto lock_request = std::make_shared<LockRequest>(txn->get_transaction_id(), mode);

        // 如果该数据项上没有加锁队列，则创建一个加锁队列
        if (lock_table_.find(id) == lock_table_.end()){
            // lock_table_[id].group_lock_mode_ = GroupLockMode::NON_LOCK;
            Queue(id).push_back(lock_request);
            Group_mode(id) = lock_trans(mode);
        } else {
            while (!check(id, txn, mode)) {
                if(txn->get_transaction_id() > Queue(id).front()->txn_id_)
                    throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
                lock_table_.at(id).cv_.wait(lock);
            }
            // if(!check(id, txn, mode))
            //     throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            Queue(id).push_back(lock_request);
            Group_mode(id) = lock_trans(mode);          
        }
        // 将该数据项加入到事务的锁集合中
        txn->get_lock_set()->insert(id);
        
        lock_request->granted_ = true;
        return true;
    };

private:
    std::mutex latch_;                                                        // 用于锁表的并发
    std::unordered_map<LockDataId, LockRequestQueue> lock_table_;             // 全局锁表
    };

