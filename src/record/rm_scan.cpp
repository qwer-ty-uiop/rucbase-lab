#include "rm_scan.h"

#include "rm_file_handle.h"

/**
 * @brief 初始化file_handle和rid
 *
 * @param file_handle
 */
RmScan::RmScan(const RmFileHandle* file_handle) : file_handle_(file_handle) {
    // Todo:
    // 初始化file_handle和rid（指向第一个存放了记录的位置）
    // 初始化page_no
    rid_.page_no = RM_FIRST_RECORD_PAGE;
    rid_.slot_no = -1;
    // 让rid指向第一个存放记录的位置
    next();
}

/**
 * @brief 找到文件中下一个存放了记录的位置
 */
void RmScan::next() {
    // Todo:
    // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置
    if (is_end())
        return;
    for (; rid_.page_no < file_handle_->file_hdr_.num_pages; rid_.page_no++) {
        rid_.slot_no = Bitmap::next_bit(
            true, file_handle_->fetch_page_handle(rid_.page_no).bitmap,
            file_handle_->file_hdr_.num_records_per_page, rid_.slot_no);
        // 找到了就返回
        if (rid_.slot_no < file_handle_->file_hdr_.num_records_per_page)
            return;
        rid_.slot_no = -1;
    }
    // 遍历所有page都没找到，则没有页
    rid_.page_no = RM_NO_PAGE;
}

/**
 * @brief ​ 判断是否到达文件末尾
 */
bool RmScan::is_end() const {
    // Todo: 修改返回值
    return rid_.page_no == RM_NO_PAGE;
}

/**
 * @brief RmScan内部存放的rid
 */
Rid RmScan::rid() const {
    // Todo: 修改返回值
    return rid_;
}