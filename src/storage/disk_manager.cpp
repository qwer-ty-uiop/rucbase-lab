/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "storage/disk_manager.h"

#include <assert.h>    // for assert
#include <filesystem>  // for C++17 filesystem
#include <fstream>     // for std::ofstream
#include <string.h>    // for memset
#include <sys/stat.h>  // for stat
#include <unistd.h>    // for pwrite and pread

#include "common/macros.h"
#include "defs.h"

namespace fs = std::filesystem;

DiskManager::DiskManager() {

}

/**
 * @description: 将数据写入文件的指定磁盘页面中
 * @param {int} fd 磁盘文件的文件句柄
 * @param {page_id_t} page_no 写入目标页面的page_id
 * @param {char} *data 要写入磁盘的数据
 * @param {int} num_bytes 要写入磁盘的数据大小
 */
void DiskManager::write_page(int fd,
                             page_id_t page_no,
                             const char* data,
                             int num_bytes) {
    // TODO: 实现将数据写入指定页面的功能
    // 使用pwrite()将数据写入指定页面，offset通过page_no * PAGE_SIZE计算得到
    // pwrite是原子操作，无需额外加锁，且不会修改文件偏移量
    ssize_t bytes_written = pwrite(fd, data, num_bytes, static_cast<off_t>(page_no) * PAGE_SIZE);
    if (bytes_written != num_bytes) {
        throw InternalError("DiskManager::write_page Error: incomplete write");
    }
}

/**
 * @description: 读取文件中指定编号的页面中的部分数据到内存中
 * @param {int} fd 磁盘文件的文件句柄
 * @param {page_id_t} page_no 指定的页面编号
 * @param {char} *offset 读取的内容写入到offset中
 * @param {int} num_bytes 读取的数据量大小
 */
void DiskManager::read_page(int fd,
                            page_id_t page_no,
                            char* offset,
                            int num_bytes) {
    // TODO: 实现从指定页面读取数据的功能
    // 使用pread()从指定页面读取数据，offset通过page_no * PAGE_SIZE计算得到
    // pread是原子操作，无需额外加锁，且不会修改文件偏移量
    ssize_t bytes_read = pread(fd, offset, num_bytes, static_cast<off_t>(page_no) * PAGE_SIZE);
    if (bytes_read != num_bytes) {
        throw InternalError("DiskManager::read_page Error: incomplete read");
    }
}

/**
 * @description: 分配一个新的页号
 *               工业级实现：优先复用已释放的页面，避免文件无限增长
 * @return {page_id_t} 分配的新页号
 * @param {int} fd 指定文件的文件句柄
 */
page_id_t DiskManager::allocate_page(int fd) {
    // 参数有效性检查
    if (UNLIKELY(fd < 0 || fd >= MAX_FD)) {
        throw InternalError("DiskManager::allocate_page Error: invalid fd " + std::to_string(fd));
    }
    
    // 尝试从空闲页面集合中获取可复用的页面
    {
        std::lock_guard<std::mutex> lock(free_pages_mutex_[fd]);
        if (!free_pages_[fd].empty()) {
            // 存在空闲页面，取出最小的页号复用
            // 使用最小页号的好处：局部性好，且便于调试
            auto it = free_pages_[fd].begin();
            page_id_t reused_page_no = *it;
            free_pages_[fd].erase(it);
            return reused_page_no;
        }
    }
    
    // 没有可复用的空闲页面，分配新页面
    // fd2pageno_[fd] 记录了下一个可用的页面编号
    // 使用原子操作保证线程安全
    page_id_t new_page_no = fd2pageno_[fd]++;
    
    // 检查页面编号是否溢出（int32_t 变为负数）
    if (UNLIKELY(new_page_no < 0)) {
        throw InternalError("DiskManager::allocate_page Error: page id overflow for fd " + std::to_string(fd));
    }
    
    return new_page_no;
}

/**
 * @description: 释放一个页号，将其加入空闲列表供后续复用
 *               工业级实现：支持页面重用，减少文件碎片
 * @param {int} fd 文件描述符
 * @param {page_id_t} page_no 要释放的页面编号
 */
void DiskManager::deallocate_page(int fd, page_id_t page_no) {
    // 参数有效性检查
    if (UNLIKELY(fd < 0 || fd >= MAX_FD)) {
        throw InternalError("DiskManager::deallocate_page Error: invalid fd " + std::to_string(fd));
    }
    if (UNLIKELY(page_no < 0)) {
        throw InternalError("DiskManager::deallocate_page Error: invalid page_no " + std::to_string(page_no));
    }
    
    // 将页面加入空闲集合
    {
        std::lock_guard<std::mutex> lock(free_pages_mutex_[fd]);
        
        // 检查页面是否已经在空闲集合中（防止重复释放）
        if (free_pages_[fd].find(page_no) != free_pages_[fd].end()) {
            throw InternalError("DiskManager::deallocate_page Error: page " + 
                               std::to_string(page_no) + " already freed");
        }
        
        // 检查页面是否有效（不能释放未分配的页面）
        if (page_no >= fd2pageno_[fd].load()) {
            throw InternalError("DiskManager::deallocate_page Error: page " + 
                               std::to_string(page_no) + " not allocated yet");
        }
        
        free_pages_[fd].insert(page_no);
    }
}

/**
 * @description: 判断指定路径是否为目录
 * @param {std::string&} path 路径
 * @return {bool} 是目录返回 true
 */
bool DiskManager::is_dir(const std::string& path) {
    return fs::is_directory(path);
}

/**
 * @description: 创建目录（支持多级目录，类似 mkdir -p）
 * @param {std::string&} path 目录路径
 */
void DiskManager::create_dir(const std::string& path) {
    // create_directories 返回值说明:
    // - true:  成功创建了新目录
    // - false: 目录已存在，或创建失败
    //
    // std::error_code ec 说明:
    // - ec 为空: 操作成功（包括目录已存在的情况）
    // - ec 非空: 操作失败，包含具体错误信息
    //
    // 所以判断逻辑: !create_directories && ec
    // - 目录已存在: 返回 false，ec 为空 → 不抛异常 ✓
    // - 创建失败:   返回 false，ec 非空 → 抛出异常 ✓
    std::error_code ec;
    if (!fs::create_directories(path, ec) && ec) {
        throw UnixError(ec);  // 传递 ec，获取正确的错误信息
    }
}

/**
 * @description: 删除目录及其所有内容（类似 rm -r）
 * @param {std::string&} path 目录路径
 */
void DiskManager::destroy_dir(const std::string& path) {
    // remove_all 行为说明:
    // - 返回值: 删除的文件/目录数量
    // - 目录不存在: 返回 0，ec 为空（不报错）
    // - 删除成功: 返回删除数量，ec 为空
    // - 删除失败: 返回 0，ec 非空
    //
    // UnixError(ec) 说明:
    // - 从 std::error_code 获取错误信息
    // - 例如: "Permission denied", "No such file or directory"
    std::error_code ec;
    fs::remove_all(path, ec);
    if (ec) {
        throw UnixError(ec);  // 传递 ec，获取正确的错误信息
    }
}

/**
 * @description: 判断指定路径文件是否存在
 * @return {bool} 若指定路径文件存在则返回true
 * @param {string} &path 指定路径文件
 */
bool DiskManager::is_file(const std::string& path) {
    return fs::is_regular_file(path);
}

/**
 * @description: 创建指定路径的空文件
 * @param {std::string&} path 文件路径
 * @throws {FileExistsError} 文件已存在
 * @throws {UnixError} 创建失败
 */
void DiskManager::create_file(const std::string& path) {
    // 先检查文件是否存在，避免覆盖
    if (fs::exists(path)) {
        throw FileExistsError(path);
    }
    
    // 使用 std::ofstream 创建空文件
    // C++17 方式，跨平台，自动处理权限
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw UnixError();
    }
    // 析构时自动关闭文件
}

/**
 * @description: 删除指定路径的文件
 * @param {string} &path 文件所在路径
 * @throws {FileNotFoundError} 文件不存在
 * @throws {UnixError} 文件已打开或删除失败
 */
void DiskManager::destroy_file(const std::string& path) {
    // 检查文件是否存在
    if (!fs::exists(path)) {
        throw FileNotFoundError(path);
    }
    
    // 检查文件是否已打开（不能删除已打开的文件）
    if (path2fd_.find(path) != path2fd_.end()) {
        throw UnixError();
    }
    
    // 使用 filesystem 删除文件
    std::error_code ec;
    if (!fs::remove(path, ec)) {
        throw UnixError(ec);
    }
}

/**
 * @description: 打开指定路径的文件，返回文件描述符
 *               注意：必须使用 POSIX open()，因为后续 pread/pwrite 需要 fd
 * @param {std::string&} path 文件路径
 * @return {int} 文件描述符
 * @throws {FileNotFoundError} 文件不存在
 * @throws {UnixError} 打开失败
 */
int DiskManager::open_file(const std::string& path) {
    // 检查文件是否存在
    if (!fs::exists(path)) {
        throw FileNotFoundError(path);
    }
    
    // 如果文件已打开，直接返回已有的 fd（避免重复打开）
    auto it = path2fd_.find(path);
    if (it != path2fd_.end()) {
        return it->second;
    }
    
    // 打开文件（必须用 POSIX open，因为 pread/pwrite 需要 fd）
    int fd = ::open(path.c_str(), O_RDWR);
    if (fd < 0) {
        throw UnixError();
    }
    
    // 更新映射表
    path2fd_[path] = fd;
    fd2path_[fd] = path;
    
    return fd;
}

/**
 * @description: 关闭指定文件描述符对应的文件
 *               注意：必须使用 POSIX close()，因为文件是通过 open() 打开的
 * @param {int} fd 文件描述符
 * @throws {FileNotOpenError} 文件未打开
 * @throws {UnixError} 关闭失败
 */
void DiskManager::close_file(int fd) {
    // 检查文件是否已打开
    auto it = fd2path_.find(fd);
    if (it == fd2path_.end()) {
        throw FileNotOpenError(fd);
    }
    
    // 从映射表中移除（先移除，再关闭）
    std::string path = it->second;
    path2fd_.erase(path);
    fd2path_.erase(it);
    
    // 关闭文件描述符
    if (::close(fd) < 0) {
        throw UnixError();
    }
}

/**
 * @description: 获得文件的大小
 * @return {int} 文件的大小，文件不存在返回 -1
 * @param {string} &file_name 文件名
 */
int DiskManager::get_file_size(const std::string& file_name) {
    std::error_code ec;
    auto size = fs::file_size(file_name, ec);
    return ec ? -1 : static_cast<int>(size);
}

/**
 * @description: 根据文件句柄获得文件名
 * @return {string} 文件句柄对应文件的文件名
 * @param {int} fd 文件句柄
 */
std::string DiskManager::get_file_name(int fd) {
    if (!fd2path_.count(fd)) {
        throw FileNotOpenError(fd);
    }
    return fd2path_[fd];
}

/**
 * @description:  获得文件名对应的文件句柄
 * @return {int} 文件句柄
 * @param {string} &file_name 文件名
 */
int DiskManager::get_file_fd(const std::string& file_name) {
    if (!path2fd_.count(file_name)) {
        return open_file(file_name);
    }
    return path2fd_[file_name];
}

/**
 * @description: 读取日志文件内容
 *               使用 pread 实现原子定位读取，线程安全
 * @param {char*} log_data 读取内容存放的缓冲区
 * @param {int} size 要读取的数据量大小
 * @param {int} offset 读取的起始位置
 * @return {int} 实际读取的数据量，-1 表示 offset 超过文件大小
 */
int DiskManager::read_log(char* log_data, int size, int offset) {
    // 延迟打开日志文件
    if (log_fd_ == -1) {
        log_fd_ = open_file(LOG_FILE_NAME);
    }
    
    // 获取文件大小
    int file_size = get_file_size(LOG_FILE_NAME);
    if (file_size < 0) {
        throw UnixError();
    }
    
    // 检查 offset 是否有效
    if (offset >= file_size) {
        return -1;
    }
    
    // 计算实际可读取的大小
    size = std::min(size, file_size - offset);
    if (size == 0) {
        return 0;
    }
    
    // 使用 pread 原子定位读取
    ssize_t bytes_read = pread(log_fd_, log_data, size, offset);
    if (bytes_read < 0) {
        throw UnixError();
    }
    
    return static_cast<int>(bytes_read);
}

/**
 * @description: 写入日志内容
 *               使用 pwrite 实现原子定位写入，追加到文件末尾
 * @param {char*} log_data 要写入的日志内容
 * @param {int} size 要写入的内容大小
 */
void DiskManager::write_log(char* log_data, int size) {
    // 延迟打开日志文件
    if (log_fd_ == -1) {
        log_fd_ = open_file(LOG_FILE_NAME);
    }
    
    // 获取当前文件大小，作为写入偏移量（追加写入）
    off_t offset = lseek(log_fd_, 0, SEEK_END);
    if (offset < 0) {
        throw UnixError();
    }
    
    // 使用 pwrite 原子定位写入
    ssize_t bytes_write = pwrite(log_fd_, log_data, size, offset);
    if (bytes_write < 0) {
        throw UnixError();
    }
    if (bytes_write != size) {
        throw InternalError("DiskManager::write_log Error: incomplete write");
    }
}
