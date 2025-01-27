/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "lock_manager.h"
#include <algorithm>
/**
 * @description: 申请行级共享锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID 记录所在的表的fd
 * @param {int} tab_fd
 */
bool LockManager::lock_shared_on_record(std::shared_ptr<Transaction> txn,
                                        const Rid& rid,
                                        int tab_fd) {
    LockDataId lock_data_id{tab_fd, rid, LockDataType::RECORD};
    return lock_general(lock_data_id, txn, LockMode::SHARED);
}

/**
 * @description: 申请行级排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_record(std::shared_ptr<Transaction> txn,
                                           const Rid& rid,
                                           int tab_fd) {
    LockDataId lock_data_id{tab_fd, rid, LockDataType::RECORD};
    return lock_general(lock_data_id, txn, LockMode::EXLUCSIVE);
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(std::shared_ptr<Transaction> txn,
                                       int tab_fd) {
    LockDataId lock_data_id{tab_fd, LockDataType::TABLE};
    return lock_general(lock_data_id, txn, LockMode::SHARED);
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(std::shared_ptr<Transaction> txn,
                                          int tab_fd) {
    LockDataId lock_data_id{tab_fd, LockDataType::TABLE};
    return lock_general(lock_data_id, txn, LockMode::EXLUCSIVE);
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(std::shared_ptr<Transaction> txn,
                                   int tab_fd) {
    LockDataId lock_data_id{tab_fd, LockDataType::TABLE};
    return lock_general(lock_data_id, txn, LockMode::INTENTION_SHARED);
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(std::shared_ptr<Transaction> txn,
                                   int tab_fd) {
    LockDataId lock_data_id{tab_fd, LockDataType::TABLE};
    return lock_general(lock_data_id, txn, LockMode::INTENTION_EXCLUSIVE);
}

bool LockManager::lock_SIX_on_table(std::shared_ptr<Transaction> txn,
                                    int tab_fd) {
    LockDataId lock_data_id{tab_fd, LockDataType::TABLE};
    return lock_general(lock_data_id, txn, LockMode::S_IX);
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(std::shared_ptr<Transaction> txn,
                         LockDataId lock_data_id) {
    std::unique_lock<std::mutex> lock(latch_);
    txn->set_state(TransactionState::SHRINKING);

    auto& queue = Queue(lock_data_id);
    auto it = std::find_if(
        queue.begin(), queue.end(), [&txn](const auto& lock_request) {
            return lock_request->txn_id_ == txn->get_transaction_id();
        });

    if (it != queue.end()) {
        queue.erase(it);

        if (queue.empty()) {
            Group_mode(lock_data_id) = GroupLockMode::NON_LOCK;
        } else {
            Group_mode(lock_data_id) = lock_trans(queue.front()->lock_mode_);
        }
    }

    lock_table_.at(lock_data_id).cv_.notify_all();
    return true;
}
