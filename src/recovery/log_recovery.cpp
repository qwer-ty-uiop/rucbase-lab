/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "log_recovery.h"

/**
 * @description: analyze阶段，需要获得脏页表（DPT）和未完成的事务列表（ATT）
 */
/**
 * @description: analyze阶段，需要获得脏页表（DPT）和未完成的事务列表（ATT）
 */
void RecoveryManager::analyze() {
    long long int offset = 0;
    std::shared_ptr<LogRecord> Logrecord;
    
    while ((Logrecord = read_log_record(offset)) != nullptr) {
        if (handleBeginLogRecord(Logrecord, offset))
            continue;
        if (handleCommitOrAbortLogRecord(Logrecord, offset))
            continue;
        if (handleInsertUpdateDeleteLogRecord(Logrecord, offset))
            continue;

        break;  // If no suitable handler, break the loop
    }
}

bool RecoveryManager::handleBeginLogRecord(const std::shared_ptr<LogRecord>& Logrecord, long long int& offset) {
    auto Castrecord = std::dynamic_pointer_cast<BeginLogRecord>(Logrecord);
    if (!Castrecord)
        return false;

    active_txns_.insert({Castrecord->log_tid_, Castrecord->lsn_});
    offset += Castrecord->log_tot_len_;
    log_recs_.push_back(Castrecord);
    return true;
}

bool RecoveryManager::handleCommitOrAbortLogRecord(const std::shared_ptr<LogRecord>& Logrecord, long long int& offset) {
    auto Castrecord = std::dynamic_pointer_cast<CommitLogRecord>(Logrecord);
    if (!Castrecord)
        return false;

    active_txns_.erase(Castrecord->log_tid_);
    offset += Castrecord->log_tot_len_;
    log_recs_.push_back(Castrecord);
    return true;
}

bool RecoveryManager::handleInsertUpdateDeleteLogRecord(const std::shared_ptr<LogRecord>& Logrecord, long long int& offset) {
    auto Castrecord = std::dynamic_pointer_cast<InsertLogRecord>(Logrecord);
    if (!Castrecord)
        return false;

    offset += Castrecord->log_tot_len_;
    Castrecord->prev_lsn_ = active_txns_[Castrecord->log_tid_];
    active_txns_[Castrecord->log_tid_] = Castrecord->lsn_;
    log_recs_.push_back(Castrecord);
    return true;
}

/**
 * @description: 重做所有未落盘的操作
 */
void RecoveryManager::redo() { 
    for (auto log_rec : log_recs_) {
        if (!processInsertLogRecord(log_rec))
            if (!processUpdateLogRecord(log_rec))
                if (!processDeleteLogRecord(log_rec))
                    if (!processBeginLogRecord(log_rec))
                        if (!processCommitLogRecord(log_rec))
                            processAbortLogRecord(log_rec);
    }
    log_recs_.clear();
}

bool RecoveryManager::processInsertLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<InsertLogRecord>(log_rec);
    if (!Logrecord) return false;

            std::string table_name(Logrecord->table_name_, Logrecord->table_name_size_);
            RmFileHandle *fh_ = sm_manager_->fhs_.at(table_name).get();
            PageId page_id;
            page_id.fd = fh_->GetFd();
            page_id.page_no = Logrecord->rid_.page_no;    
            Page* page = buffer_pool_manager_->fetch_page(page_id);

            bool is_dirty = false;
            if (Logrecord->lsn_ > page->get_page_lsn()) {
                // 需执行写操作
                auto &trans = TransactionManager::txn_map[Logrecord->log_tid_];
                sm_manager_->recovery_insert(table_name, Logrecord->rid_, Logrecord->insert_value_, trans);
                page->set_page_lsn(Logrecord->lsn_);
                is_dirty = true;
            }  
            buffer_pool_manager_->unpin_page(page_id, is_dirty); 
    return true;
}

bool RecoveryManager::processUpdateLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<UpdateLogRecord>(log_rec);
    if (!Logrecord) return false;

    std::string table_name(Logrecord->table_name_, Logrecord->table_name_size_);
            RmFileHandle *fh_ = sm_manager_->fhs_.at(table_name).get();
            PageId page_id;
            page_id.fd = fh_->GetFd();
            page_id.page_no = Logrecord->rid_.page_no;    
            Page* page = buffer_pool_manager_->fetch_page(page_id);

            bool is_dirty = false;
            if (Logrecord->lsn_ > page->get_page_lsn()) {
                // 需执行写操作
                auto &trans = TransactionManager::txn_map[Logrecord->log_tid_];
                sm_manager_->recovery_update(table_name, Logrecord->rid_, Logrecord->new_value_, trans);
                page->set_page_lsn(Logrecord->lsn_);
                is_dirty = true;
            } 
            buffer_pool_manager_->unpin_page(page_id, is_dirty); 
    return true;
}

bool RecoveryManager::processDeleteLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<DeleteLogRecord>(log_rec);
    if (!Logrecord) return false;

    std::string table_name(Logrecord->table_name_, Logrecord->table_name_size_);
            RmFileHandle *fh_ = sm_manager_->fhs_.at(table_name).get();
            PageId page_id;
            page_id.fd = fh_->GetFd();
            page_id.page_no = Logrecord->rid_.page_no;    
            Page* page = buffer_pool_manager_->fetch_page(page_id);

            bool is_dirty = false;
            if (Logrecord->lsn_ > page->get_page_lsn()) {
                // 需执行写操作
                auto &trans = TransactionManager::txn_map[Logrecord->log_tid_];
                sm_manager_->recovery_delete(table_name, Logrecord->rid_, trans);
                page->set_page_lsn(Logrecord->lsn_);
                is_dirty = true;
            }   
            buffer_pool_manager_->unpin_page(page_id, is_dirty); 
    return true;
}

bool RecoveryManager::processBeginLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<BeginLogRecord>(log_rec);
    if (!Logrecord) return false;

    auto trans = std::make_unique<Transaction>(Logrecord->log_tid_);
    trans->set_state(TransactionState::DEFAULT);
    TransactionManager::txn_map[Logrecord->log_tid_] = std::move(trans);
    return true;
}

bool RecoveryManager::processCommitLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<CommitLogRecord>(log_rec);
    if (!Logrecord) return false;

    auto &trans = TransactionManager::txn_map[Logrecord->log_tid_];
    auto write_set = trans->get_write_set();
    write_set->clear();
    trans->set_state(TransactionState::COMMITTED);
    return true;
}

bool RecoveryManager::processAbortLogRecord(const std::shared_ptr<LogRecord>& log_rec) {
    auto Logrecord = std::dynamic_pointer_cast<AbortLogRecord>(log_rec);
    if (!Logrecord) return false;

    auto &trans = TransactionManager::txn_map[Logrecord->log_tid_];
    auto write_set = trans->get_write_set();
    while (!write_set->empty()) {
    auto &entry = write_set->back();
    const auto writeType = entry.GetWriteType();
    const auto tableName = entry.GetTableName();
    const auto rid = entry.GetRid();
    const auto record = entry.GetRecord();

    switch (writeType) {
        case WType::INSERT_TUPLE:
            sm_manager_->rollback_insert(tableName, rid, trans);
            break;
        case WType::DELETE_TUPLE:
            sm_manager_->rollback_delete(tableName, record, rid, trans);
            break;
        case WType::UPDATE_TUPLE:
            sm_manager_->rollback_update(tableName, rid, record, trans);
            break;
    }

    write_set->pop_back();
}

    trans->get_write_set()->clear();
    trans->set_state(TransactionState::ABORTED);
    return true;
}

/**
 * @description: 回滚未完成的事务
 */
void RecoveryManager::undo() {
    for (auto active_txn : active_txns_) {
        std::shared_ptr<Transaction> trans = TransactionManager::txn_map[active_txn.first];
        processWriteSet(trans);
    }
}

void RecoveryManager::processWriteSet(const std::shared_ptr<Transaction>& trans) {
    auto write_set = trans->get_write_set();
    while (!write_set->empty()) {
        auto &it = write_set->back();
            switch (it.GetWriteType()) {
                case WType::INSERT_TUPLE:
                    sm_manager_->rollback_insert(it.GetTableName(), it.GetRid(), trans);
                    break;
                case WType::DELETE_TUPLE:
                    sm_manager_->rollback_delete(it.GetTableName(), it.GetRecord(), it.GetRid(), trans);
                    break;
                case WType::UPDATE_TUPLE:
                    sm_manager_->rollback_update(it.GetTableName(), it.GetRid(), it.GetRecord(), trans);
                    break;
            }
        write_set->pop_back();
    }
    trans->get_write_set()->clear();
}
/**
 * @description: 读取一条日志记录
 * @param {long long int} offset 日志记录在文件中的偏移量
 * @return {std::shared_ptr<LogRecord>} 返回该记录
 */
bool RecoveryManager::readLogHeader(char* log_header, long long int offset) {
    // 从磁盘读取日志头
    return disk_manager_->read_log(log_header, LOG_HEADER_SIZE, offset) != 0;
}

LogType RecoveryManager::extractLogType(const char* log_header) {
    // 提取日志记录类型
    return *reinterpret_cast<const LogType*>(log_header);
}

uint32_t RecoveryManager::extractLogLength(const char* log_header) {
    // 提取日志记录长度
    return *reinterpret_cast<const uint32_t*>(log_header + OFFSET_LOG_TOT_LEN);
}
std::shared_ptr<LogRecord> RecoveryManager::createLogRecordOfType(LogType log_type) {
    // 创建对应类型的日志记录实例
    switch (log_type) {
        case LogType::UPDATE:
            return std::make_shared<UpdateLogRecord>();
        case LogType::INSERT:
            return std::make_shared<InsertLogRecord>();
        case LogType::DELETE:
            return std::make_shared<DeleteLogRecord>();
        case LogType::begin:
            return std::make_shared<BeginLogRecord>();
        case LogType::commit:
            return std::make_shared<CommitLogRecord>();
        case LogType::ABORT:
            return std::make_shared<AbortLogRecord>();
        default:
            return nullptr;
    }
}
std::shared_ptr<LogRecord> RecoveryManager::read_log_record(long long int offset) {
    char log_header[LOG_HEADER_SIZE];
    if (!readLogHeader(log_header, offset)) {
        return nullptr; // Return if header read fails
    }

    LogType log_type = extractLogType(log_header);
    uint32_t log_length = extractLogLength(log_header);
    char log_buffer[log_length];
    disk_manager_->read_log(log_buffer, log_length, offset);
    //LogRecord* log_record = nullptr;
    std::shared_ptr<LogRecord> Logrecord = createLogRecordOfType(log_type);
    if (Logrecord != nullptr) {
        Logrecord->deserialize(log_buffer);
    }
    return std::shared_ptr<LogRecord>(Logrecord);
}

