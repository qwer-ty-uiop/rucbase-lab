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

#include <map>
#include <unordered_map>
#include "log_manager.h"
#include "storage/disk_manager.h"
#include "system/sm_manager.h"
#include "transaction/transaction_manager.h"

class RedoLogsInPage {
public:
    RedoLogsInPage() { table_file_ = nullptr; }
    RmFileHandle* table_file_;
    std::vector<lsn_t> redo_logs_;   // 在该page上需要redo的操作的lsn
};

class RecoveryManager {
public:
    RecoveryManager(DiskManager* disk_manager, BufferPoolManager* buffer_pool_manager, SmManager* sm_manager) {
        disk_manager_ = disk_manager;
        buffer_pool_manager_ = buffer_pool_manager;
        sm_manager_ = sm_manager;

        active_txns_ = std::unordered_map<txn_id_t, lsn_t>();
    }

    void analyze();
    void redo();
    void undo();

    std::shared_ptr<LogRecord> read_log_record(long long int offset);

private:
    LogBuffer log_buffer_;                                          // 读入日志
    DiskManager* disk_manager_;                                     // 用来读写文件
    BufferPoolManager* buffer_pool_manager_;                        // 对页面进行读写
    SmManager* sm_manager_;                                         // 访问数据库元数据

    std::unordered_map<txn_id_t, lsn_t> active_txns_;               // 活动事务列表，记录当前系统运行过程中所有正在执行的事务
    std::vector<std::shared_ptr<LogRecord>> log_recs_;
    bool handleBeginLogRecord(const std::shared_ptr<LogRecord> &Logrecord, long long int &offset);
    bool handleCommitOrAbortLogRecord(const std::shared_ptr<LogRecord> &Logrecord, long long int &offset);
    bool handleInsertUpdateDeleteLogRecord(const std::shared_ptr<LogRecord> &Logrecord, long long int &offset);
    bool processInsertLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    bool processUpdateLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    bool processDeleteLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    void processDML(const std::shared_ptr<LogRecord> &Logrecord, const std::string &operation_type);
    bool processBeginLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    bool processCommitLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    bool processAbortLogRecord(const std::shared_ptr<LogRecord> &log_rec);
    void processWriteSet(const std::shared_ptr<Transaction> &trans);
    bool readLogHeader(char *log_header, long long int offset);
    LogType extractLogType(const char *log_header);
    uint32_t extractLogLength(const char *log_header);
    std::shared_ptr<LogRecord> constructLogRecord(LogType log_type, uint32_t log_length, long long int offset);
    std::shared_ptr<LogRecord> createLogRecordOfType(LogType log_type);
    // 日志记录列表
};