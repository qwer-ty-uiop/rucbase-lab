#include "storage/disk_manager.h"

#include <assert.h>    // for assert
#include <string.h>    // for memset
#include <sys/stat.h>  // for stat
#include <unistd.h>    // for lseek

#include "defs.h"

DiskManager::DiskManager() {
    memset(fd2pageno_, 0,
           MAX_FD * (sizeof(std::atomic<page_id_t>) / sizeof(char)));
}

/**
 * @brief Write the contents of the specified page into disk file
 *
 */
void DiskManager::write_page(int fd,
                             page_id_t page_no,
                             const char* offset,
                             int num_bytes) {
    // Todo:
    // 1.lseek()定位到文件头，通过(fd,page_no)可以定位指定页面及其在磁盘文件中的偏移量
    // 2.调用write()函数
    // 注意处理异常

    // offset表示文件指针的偏移量
    // whence表示偏移量的基准位置
    // SEEK_SET基准位置为文件开头，即offset表示距离文件开头的偏移量；SEEK_CUR基准位置为文件当前位置；SEEK_END基准位置为文件末尾
    // off_t lseek(int filedes, off_t offset, int whence) ;第二个参数是偏移量
    // 找要写页起始位置：页大小是固定的，从文件头开始偏移页id*固定大小，就是页的起始地址
    // lseek成功返回偏移的位置，失败返回-1
    // lseek还会改变文件指针fd的位置,将其变成偏移的位置
    if (lseek(fd, page_no * PAGE_SIZE, SEEK_SET) == -1)
        throw UnixError();
    // write(int  fd,  const  void  *buf, size_t num_bytes);
    // write()会把参数buf所指的内存中的num_bytes个字节写入到参数fd所指的文件内
    // 返回值：如果顺利write()会返回实际写入的字节数（len）。当有错误发生时则返回-1
    int write_bytes = write(fd, offset, num_bytes);
    // write返回值和num_bytes不相等时，抛出错误
    if (write_bytes < 0)
        throw InternalError("DisManager::write_page Error");
}

/**
 * @brief Read the contents of the specified page into the given memory area
 */
void DiskManager::read_page(int fd,
                            page_id_t page_no,
                            char* offset,
                            int num_bytes) {
    // Todo:
    // 1.lseek()定位到文件头，通过(fd,page_no)可以定位指定页面及其在磁盘文件中的偏移量
    // 2.调用read()函数
    // 注意处理异常

    if (lseek(fd, page_no * PAGE_SIZE, SEEK_SET) == -1)
        throw UnixError();
    int read_bytes = read(fd, offset, num_bytes);
    if (read_bytes < 0)
        throw InternalError("DiskManager::read_page Error");
}

/**
 * @brief Allocate new page (operations like create index/table)
 * For now just keep an increasing counter
 */
page_id_t DiskManager::AllocatePage(int fd) {
    // Todo:
    // 简单的自增分配策略，指定文件的页面编号加1

    // 申请分配页的空间
    return fd2pageno_[fd]++;
}

/**
 * @brief Deallocate page (operations like drop index/table)
 * Need bitmap in header page for tracking pages
 * This does not actually need to do anything for now.
 */
void DiskManager::DeallocatePage(__attribute__((unused)) page_id_t page_id) {
    return;
}

bool DiskManager::is_dir(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

void DiskManager::create_dir(const std::string& path) {
    // Create a subdirectory
    std::string cmd = "mkdir " + path;
    if (system(cmd.c_str()) < 0) {  // 创建一个名为path的目录
        throw UnixError();
    }
}

void DiskManager::destroy_dir(const std::string& path) {
    std::string cmd = "rm -r " + path;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
}

/**
 * @brief 用于判断指定路径文件是否存在
 */
bool DiskManager::is_file(const std::string& path) {
    // Todo:
    // 用struct stat获取文件信息

    // struct stat {
    //     dev_t st_dev;            /* 文件所在设备的 ID */
    //     ino_t st_ino;            /* 文件对应 inode 节点编号 */
    //     mode_t st_mode;          /* 文件对应的模式 */
    //     nlink_t st_nlink;        /* 文件的链接数 */
    //     uid_t st_uid;            /* 文件所有者的用户 ID */
    //     gid_t st_gid;            /* 文件所有者的组 ID */
    //     dev_t st_rdev;           /* 设备号（指针对设备文件） */
    //     off_t st_size;           /* 文件大小（以字节为单位） */
    //     blksize_t st_blksize;    /* 文件内容存储的块大小 */
    //     blkcnt_t st_blocks;      /* 文件内容所占块数 */
    //     struct timespec st_atim; /* 文件最后被访问的时间 */
    //     struct timespec st_mtim; /* 文件内容最后被修改的时间 */
    //     struct timespec st_ctim; /* 文件状态最后被改变的时间 */
    // }

    struct stat st;
    // stat(filename,buf)读取filename获取文件信息，保存在buf所指的结构体stat中
    // 成功返回0，失败返回-1
    // S_ISREG(st_mode)查看文件模式是否是一般(常规)文件
    // 通过文件路径判断文件是否在数据库中
    // c_str()是为了和c语言兼容，其内容和std::string path是一样的
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

/**
 * @brief 用于创建指定路径文件
 */
void DiskManager::create_file(const std::string& path) {
    // Todo:
    // 调用open()函数，使用O_CREAT模式
    // 注意不能重复创建相同文件

    // 通过is_file判断文件是否重复，如果重复则抛出异常
    if (is_file(path)) {
        throw FileExistsError(path);
    }

    // 新建文件有读写权且能创建文件
    // 用户具有读写权

    // open函数：int open(const char *pathname, int flags, mode_t mode);
    // 操作成功，它将返回一个文件描述符
    // 操作失败返回-1
    // flags包括    O_RDONLY：只读模式 O_WRONLY：只写模式 O_RDWR：可读可写
    // 要用的可以追加的常量：O_CREAT 表示如果指定文件不存在，则创建这个文件
    // mode参数表示设置文件访问权限的初始值
    // 用S_IRUSR、S_IWUSR等宏定义按位或起来表示
    int fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0)
        throw(FileNotOpenError(fd));
    // 关闭fd描述的文件
    // fd定义：每一个进程在内核中，会有一个“打开文件”表数组，每一个元素都指向一个内核的打开文件对象，而fd就是数组的下标
    close(fd);
}

/**
 * @brief 用于删除指定路径文件
 */
void DiskManager::destroy_file(const std::string& path) {
    // Todo:
    // 调用unlink()函数
    // 注意不能删除未关闭的文件

    // 没找到文件就删不了
    if (!is_file(path))
        throw FileNotFoundError(path);
    // 文件没关
    // 哈希表path2fd_ 看这个路径有没有映射的fd，如果有那么这个文件还没关闭
    if (path2fd_.count(path))
        throw FileNotClosedError(path);
    // 和rm差不多，删除指定位置的文件
    unlink(path.c_str());
}

/**
 * @brief 用于打开指定路径文件
 */
int DiskManager::open_file(const std::string& path) {
    // Todo:
    // 调用open()函数，使用O_RDWR模式
    // 注意不能重复打开相同文件，并且需要更新文件打开列表

    // 不在数据库中
    if (!is_file(path))
        throw FileNotFoundError(path);
    // 已经打开文件
    if (path2fd_.count(path))
        throw FileNotClosedError(path);
    int fd = open(path.c_str(), O_RDWR);
    // 更新打开文件列表
    path2fd_[path] = fd;
    fd2path_[fd] = path;
    // 返回打开文件位置
    return fd;
}

/**
 * @brief 用于关闭指定路径文件
 */
void DiskManager::close_file(int fd) {
    // Todo:
    // 调用close()函数
    // 注意不能关闭未打开的文件，并且需要更新文件打开列表

    // 文件没打开
    if (!fd2path_.count(fd))
        throw FileNotOpenError(fd);
    close(fd);
    // 移除键，更新打开列表
    path2fd_.erase(fd2path_[fd]);
    fd2path_.erase(fd);
}

int DiskManager::GetFileSize(const std::string& file_name) {
    struct stat stat_buf;
    int rc = stat(file_name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::string DiskManager::GetFileName(int fd) {
    if (!fd2path_.count(fd)) {
        throw FileNotOpenError(fd);
    }
    return fd2path_[fd];
}

int DiskManager::GetFileFd(const std::string& file_name) {
    if (!path2fd_.count(file_name)) {
        return open_file(file_name);
    }
    return path2fd_[file_name];
}

bool DiskManager::ReadLog(char* log_data,
                          int size,
                          int offset,
                          int prev_log_end) {
    // read log file from the previous end
    if (log_fd_ == -1) {
        log_fd_ = open_file(LOG_FILE_NAME);
    }
    offset += prev_log_end;
    int file_size = GetFileSize(LOG_FILE_NAME);
    if (offset >= file_size) {
        return false;
    }

    size = std::min(size, file_size - offset);
    lseek(log_fd_, offset, SEEK_SET);
    ssize_t bytes_read = read(log_fd_, log_data, size);
    if (bytes_read != size) {
        throw UnixError();
    }
    return true;
}

void DiskManager::WriteLog(char* log_data, int size) {
    if (log_fd_ == -1) {
        log_fd_ = open_file(LOG_FILE_NAME);
    }

    // write from the file_end
    lseek(log_fd_, 0, SEEK_END);
    ssize_t bytes_write = write(log_fd_, log_data, size);
    if (bytes_write != size) {
        throw UnixError();
    }
}
