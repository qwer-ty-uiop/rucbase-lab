/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "transaction_manager.h"
#include "record/rm_file_handle.h"
#include "system/sm_manager.h"

std::unordered_map<txn_id_t, std::shared_ptr<Transaction>>
    TransactionManager::txn_map = {};

/**
 * @description: 事务的开始方法
 * @return {Transaction*} 开始事务的指针
 * @param {Transaction*} txn
 * 事务指针，空指针代表需要创建新事务，否则开始已有事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
std::shared_ptr<Transaction> TransactionManager::begin(
    std::shared_ptr<Transaction> txn,
    LogManager* log_manager) {
    // 判断传入事务参数是否为空指针，如果为空则创建新事务
    if (!txn) {
        txn = std::make_shared<Transaction>(next_txn_id_++);
        txn->set_state(TransactionState::DEFAULT);
    }

    // 将事务加入到全局事务表中
    std::unique_lock<std::mutex> lock(latch_);
    txn_map[txn->get_transaction_id()] = txn;
    lock.unlock();
    // 释放锁

    // 记录事务开始日志
    BeginLogRecord log_record(txn->get_transaction_id());
    log_manager->add_log_to_buffer(&log_record);

    // Step 4: Return the current transaction pointer
    return txn;
}

/**
 * @description: 事务的提交方法
 * @param {Transaction*} txn 需要提交的事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
void TransactionManager::commit(std::shared_ptr<Transaction> txn,
                                LogManager* log_manager) {
    // Step 1: Commit all write operations if there are any pending
    auto writeSet = txn->get_write_set();
    writeSet->clear();

    // Step 2: Release all locks held by the transaction
    auto lockSet = txn->get_lock_set();
    for (auto& lock : *lockSet) {
        lock_manager_->unlock(txn, lock);
    }
    lockSet->clear();

    // Step 3: Release transaction resources, e.g., lock collection
    // Currently handled by clearing the lock set above.

    // Step 4: Flush transaction logs to disk
    // Assuming this is handled within 'add_log_to_buffer' which schedules logs
    // to be written to disk
    CommitLogRecord log_record(txn->get_transaction_id());
    log_manager->add_log_to_buffer(&log_record);

    // Step 5: Update transaction state to COMMITTED
    txn->set_state(TransactionState::COMMITTED);
}

/**
 * @description: 事务的终止（回滚）方法
 * @param {Transaction *} txn 需要回滚的事务
 * @param {LogManager} *log_manager 日志管理器指针
 */
void TransactionManager::abort(std::shared_ptr<Transaction> txn,
                               LogManager* log_manager) {
    // Create context to hold necessary managers and transaction information
    auto context = std::make_unique<Context>(lock_manager_, log_manager, txn);

    // Log the abortion attempt before modifying any data
    AbortLogRecord log_record(txn->get_transaction_id());
    log_manager->add_log_to_buffer(&log_record);

    // Step 1: Roll back all write operations
    auto write_set = txn->get_write_set();
    while (!write_set->empty()) {
        auto& lastWriteOperation = write_set->back();

        switch (lastWriteOperation.GetWriteType()) {
            case WType::INSERT_TUPLE:
                sm_manager_->rollback_insert(lastWriteOperation.GetTableName(),
                                             lastWriteOperation.GetRid(),
                                             context.get());
                break;
            case WType::DELETE_TUPLE:
                sm_manager_->rollback_delete(lastWriteOperation.GetTableName(),
                                             lastWriteOperation.GetRecord(),
                                             lastWriteOperation.GetRid(),
                                             context.get());
                break;
            case WType::UPDATE_TUPLE:
                sm_manager_->rollback_update(lastWriteOperation.GetTableName(),
                                             lastWriteOperation.GetRid(),
                                             lastWriteOperation.GetRecord(),
                                             context.get());
                break;
        }

        write_set->pop_back();  // Remove the write operation from the set after
                                // handling it
    }

    // Step 2: Release all locks held by the transaction
    auto lock_set = txn->get_lock_set();
    for (auto& lock : *lock_set) {
        lock_manager_->unlock(txn, lock);
    }
    lock_set->clear();

    // Step 3: Clear all transaction-related resources, e.g., lock collection
    // This is implicitly done above by clearing the lock set

    // Step 5: Update the transaction state to ABORTED
    txn->set_state(TransactionState::ABORTED);
}
