/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "rm_scan.h"

#include "rm_file_handle.h"

/**
 * @brief 初始化file_handle和rid
 * @param file_handle
 */
RmScan::RmScan(const RmFileHandle* file_handle)
    : file_handle_(file_handle), rid_{RM_FIRST_RECORD_PAGE, -1} {
    next();
    // Todo:
    // 初始化file_handle和rid（指向第一个存放了记录的位置）
    // 初始化page_no
    // 让rid指向第一个存放记录的位置
}

/**
 * @brief 找到文件中下一个存放了记录的位置
 */
void RmScan::next() {
    // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置
    while (rid_.page_no != RM_NO_PAGE &&
           rid_.page_no < file_handle_->file_hdr_.num_pages) {
        RmPageHandle page_handle =
            file_handle_->fetch_page_handle(rid_.page_no);
        rid_.slot_no = Bitmap::next_bit(
            true, page_handle.bitmap,
            file_handle_->file_hdr_.num_records_per_page, rid_.slot_no);

        file_handle_->buffer_pool_manager_->unpin_page(
            {.fd = file_handle_->fd_, .page_no = rid_.page_no}, false);

        if (rid_.slot_no < file_handle_->file_hdr_.num_records_per_page) {
            return;
        }

        // 如果当前页面已经遍历完，更新到下一页
        move_to_next_page();
    }

    // 遍历所有page都没找到，则没有页
    rid_.page_no = RM_NO_PAGE;
}

/**
 * @brief 更新到下一页并重置 slot_no
 */
void RmScan::move_to_next_page() {
    rid_.slot_no = -1;
    rid_.page_no++;
}

/**
 * @brief ​ 判断是否到达文件末尾
 */
bool RmScan::is_end() const {
    // Todo: 修改返回值
    // 根据当前的 rid_.page_no 判断是否到达文件末尾
    return rid_.page_no == RM_NO_PAGE;
}

/**
 * @brief RmScan内部存放的rid
 */
Rid RmScan::rid() const {
    return rid_;
}