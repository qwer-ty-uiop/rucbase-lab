/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include <cstring>
#include "log_manager.h"

/**
 * @description: 添加日志记录到日志缓冲区中，并返回日志记录号
 * @param {LogRecord*} log_record 要写入缓冲区的日志记录
 * @return {lsn_t} 返回该日志的日志记录号
 */
lsn_t LogManager::add_log_to_buffer(LogRecord* log_record) {
    // 获取互斥锁latch_
    std::scoped_lock lock{latch_};
    // 判断log_buffer_中是否还存在足够的剩余空间
    if (!log_buffer_.is_full(log_record->log_tot_len_)) {
        // 为该日志分配日志序列号
        log_record->lsn_ = global_lsn_++;
        // 把该日志写入到log_buffer_中
        log_buffer_.write_log_record(log_record);
        // latch_.unlock();
        // flush_log_to_disk();
        flag = true;
        return log_record->lsn_;
    } else {
        // latch_.unlock();
        // flush_log_to_disk();
        return INVALID_LSN;
    }
}

/**
 * @description: 把日志缓冲区的内容刷到磁盘中，由于目前只设置了一个缓冲区，因此需要阻塞其他日志操作
 */
void LogManager::flush_log_to_disk() {
    // 获取互斥锁latch_
    std::scoped_lock lock{latch_};
    // 把日志缓冲区的内容刷到磁盘中
    disk_manager_->write_log(log_buffer_.buffer_, log_buffer_.offset_);
    // 更新日志缓冲区、persistent_lsn_
    memset(log_buffer_.buffer_, 0, sizeof(log_buffer_.offset_));
    log_buffer_.offset_ = 0; 
    persist_lsn_ = global_lsn_ - 1; 
}