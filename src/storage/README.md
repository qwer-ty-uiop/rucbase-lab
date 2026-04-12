# 存储管理模块详解

## 📋 模块概述

存储管理模块是整个数据库系统的基础，负责管理数据的物理存储，包括磁盘文件操作、页面管理和内存缓冲池。该模块直接与底层存储设备交互，为上层模块提供高效的数据访问接口。

## 🎯 核心功能

- **磁盘管理**：管理磁盘文件的创建、打开、读写和删除
- **页面管理**：管理固定大小的页面（4KB），作为数据存储的基本单位
- **缓冲池管理**：管理内存中的页面缓存，减少磁盘I/O操作
- **并发控制**：支持多线程并发访问数据

## 📁 目录结构

```
src/storage/
├── disk_manager.h/cpp        # 磁盘管理器
├── buffer_pool_manager.h/cpp  # 缓冲池管理器
├── page.h/cpp                # 页面定义
└── README.md                 # 模块文档
```

## 🏗️ 核心组件

### 1. DiskManager

#### 功能概述
负责磁盘文件的管理，包括文件的创建、打开、关闭、读写等操作。

#### 关键数据结构
```cpp
class DiskManager {
private:
    std::unordered_map<std::string, int> path2fd_;  // 文件路径到文件描述符的映射
    std::unordered_map<int, std::string> fd2path_;  // 文件描述符到文件路径的映射
    int next_fd_;  // 下一个可用的文件描述符
    std::mutex latch_;  // 互斥锁，保证并发安全
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `create_file()` | 创建新文件 | 使用 `open()` 系统调用，指定 `O_CREAT` 标志 |
| `destroy_file()` | 删除文件 | 先从映射表中移除，再调用 `unlink()` |
| `open_file()` | 打开文件 | 检查文件是否已打开，否则调用 `open()` |
| `close_file()` | 关闭文件 | 调用 `close()` 并从映射表中移除 |
| `write_page()` | 写入页面 | 计算偏移量，调用 `write()`，检查写入大小 |
| `read_page()` | 读取页面 | 计算偏移量，调用 `read()`，检查读取大小 |

#### 实现注意事项
- **文件描述符管理**：避免文件描述符泄漏，正确管理文件的打开和关闭
- **错误处理**：对系统调用的返回值进行检查，及时抛出异常
- **并发安全**：使用互斥锁保护共享数据结构
- **数据完整性**：确保读写操作的完整性，处理部分读写的情况

### 2. Page

#### 功能概述
表示数据库的基本存储单位，固定大小为4KB。

#### 关键数据结构
```cpp
class Page {
private:
    page_id_t id_;          // 页面ID
    char data_[PAGE_SIZE];  // 页面数据
    int pin_count_;         // 固定计数
    bool is_dirty_;         // 脏页标记
    ReaderWriterLatch rwlatch_;  // 读写锁，支持并发访问
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `get_page_id()` | 获取页面ID | 直接返回 `id_` |
| `get_data()` | 获取页面数据 | 返回 `data_` 指针 |
| `is_dirty()` | 检查是否脏页 | 返回 `is_dirty_` |
| `pin_count()` | 获取固定计数 | 返回 `pin_count_` |
| `WLatch()` / `RLatch()` | 获取写锁/读锁 | 调用 `rwlatch_.WLock()` / `RLock()` |
| `WUnlatch()` / `RUnlatch()` | 释放写锁/读锁 | 调用 `rwlatch_.WUnlock()` / `RUnlock()` |

#### 实现注意事项
- **页面大小**：固定为4KB，与磁盘块大小对齐
- **并发控制**：使用读写锁支持多线程并发访问
- **脏页标记**：及时标记和清除脏页状态
- **固定计数**：正确管理页面的固定和释放

### 3. BufferPoolManager

#### 功能概述
管理内存中的页面缓存，负责页面的分配、回收和替换。

#### 关键数据结构
```cpp
class BufferPoolManager {
private:
    size_t pool_size_;                // 缓冲池大小
    Page *pages_;                     // 页面数组
    std::unordered_map<page_id_t, frame_id_t> page_table_;  // 页面ID到帧ID的映射
    std::list<frame_id_t> free_list_;  // 空闲帧列表
    Replacer *replacer_;              // 页面替换策略
    DiskManager *disk_manager_;       // 磁盘管理器
    std::mutex latch_;                // 互斥锁，保证并发安全
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `fetch_page()` | 获取页面 | 检查缓冲池，无则从磁盘加载，必要时替换 |
| `unpin_page()` | 释放页面 | 减少固定计数，必要时标记为可替换 |
| `flush_page()` | 刷新页面 | 将脏页写回磁盘，清除脏页标记 |
| `new_page()` | 创建新页面 | 分配页面ID，从空闲列表获取帧 |
| `delete_page()` | 删除页面 | 释放页面，写回脏页，加入空闲列表 |

#### 实现注意事项
- **页面替换**：当缓冲池满时，使用替换策略选择淘汰页面
- **脏页管理**：淘汰脏页前必须写回磁盘
- **并发控制**：使用互斥锁保护共享数据结构
- **内存管理**：避免内存泄漏，正确管理页面的生命周期

## 🔧 实现步骤

### 1. 实现 DiskManager

1. **初始化**：创建文件描述符映射表，初始化 `next_fd_`
2. **文件操作**：实现 `create_file()`, `destroy_file()`, `open_file()`, `close_file()`
3. **页面操作**：实现 `write_page()`, `read_page()`，处理偏移量计算和错误检查
4. **并发控制**：为所有操作添加互斥锁保护

### 2. 实现 Page

1. **初始化**：设置默认值，初始化 `id_` 为 `INVALID_PAGE_ID`
2. **数据访问**：提供 `get_data()` 方法访问页面数据
3. **状态管理**：实现 `is_dirty()`, `pin_count()` 等状态查询方法
4. **并发控制**：集成 `ReaderWriterLatch` 支持并发访问

### 3. 实现 BufferPoolManager

1. **初始化**：分配页面数组，初始化数据结构，创建替换策略
2. **页面获取**：实现 `fetch_page()`，处理缓冲池命中和未命中的情况
3. **页面释放**：实现 `unpin_page()`，管理固定计数
4. **页面刷新**：实现 `flush_page()`，将脏页写回磁盘
5. **页面管理**：实现 `new_page()` 和 `delete_page()`，管理页面的创建和删除

## 📝 代码示例

### 磁盘管理器示例

```cpp
// 写入页面
void DiskManager::write_page(int fd, page_id_t page_no, char *page_data) {
    lseek(fd, page_no * PAGE_SIZE, SEEK_SET);
    ssize_t bytes_written = write(fd, page_data, PAGE_SIZE);
    if (bytes_written != PAGE_SIZE) {
        throw InternalError("DiskManager::write_page Error: incomplete write");
    }
}

// 读取页面
void DiskManager::read_page(int fd, page_id_t page_no, char *page_data) {
    lseek(fd, page_no * PAGE_SIZE, SEEK_SET);
    ssize_t bytes_read = read(fd, page_data, PAGE_SIZE);
    if (bytes_read != PAGE_SIZE) {
        throw InternalError("DiskManager::read_page Error: incomplete read");
    }
}
```

### 缓冲池管理器示例

```cpp
// 获取页面
Page *BufferPoolManager::fetch_page(page_id_t page_id) {
    std::lock_guard<std::mutex> lock(latch_);
    
    // 检查页面是否已在缓冲池
    auto it = page_table_.find(page_id);
    if (it != page_table_.end()) {
        frame_id_t frame_id = it->second;
        Page *page = pages_ + frame_id;
        page->pin_count_++;
        replacer_->Pin(frame_id);
        return page;
    }
    
    // 分配新页面
    frame_id_t frame_id = -1;
    if (!free_list_.empty()) {
        frame_id = free_list_.front();
        free_list_.pop_front();
    } else if (replacer_->Victim(&frame_id)) {
        // 替换策略选择淘汰页面
        Page *victim_page = pages_ + frame_id;
        if (victim_page->is_dirty_) {
            disk_manager_->write_page(victim_page->get_page_id(), victim_page->get_data());
        }
        page_table_.erase(victim_page->get_page_id());
    } else {
        throw Exception("BufferPoolManager::fetch_page Error: no free frame");
    }
    
    // 从磁盘读取页面
    Page *page = pages_ + frame_id;
    disk_manager_->read_page(page_id, page->get_data());
    page->id_ = page_id;
    page->is_dirty_ = false;
    page->pin_count_ = 1;
    page_table_[page_id] = frame_id;
    replacer_->Pin(frame_id);
    
    return page;
}
```

## 🚀 使用方法

### 基本使用流程

1. **初始化**：
   ```cpp
   DiskManager *disk_manager = new DiskManager();
   BufferPoolManager *buffer_pool_manager = new BufferPoolManager(BUFFER_POOL_SIZE, disk_manager);
   ```

2. **获取页面**：
   ```cpp
   Page *page = buffer_pool_manager->fetch_page(page_id);
   ```

3. **修改页面**：
   ```cpp
   char *data = page->get_data();
   memcpy(data, new_data, PAGE_SIZE);
   page->is_dirty_ = true;
   ```

4. **释放页面**：
   ```cpp
   buffer_pool_manager->unpin_page(page_id, true);  // true表示页面被修改
   ```

5. **刷新页面**（可选）：
   ```cpp
   buffer_pool_manager->flush_page(page_id);
   ```

6. **创建新页面**：
   ```cpp
   Page *new_page;
   page_id_t new_page_id;
   buffer_pool_manager->new_page(&new_page_id, &new_page);
   ```

7. **删除页面**：
   ```cpp
   buffer_pool_manager->delete_page(page_id);
   ```

### 并发访问示例

```cpp
// 读操作
Page *page = buffer_pool_manager->fetch_page(page_id);
page->RLatch();  // 获取读锁
try {
    // 读取页面内容
    const char *data = page->get_data();
    // 处理数据
} finally {
    page->RUnlatch();  // 释放读锁
    buffer_pool_manager->unpin_page(page_id, false);
}

// 写操作
Page *page = buffer_pool_manager->fetch_page(page_id);
page->WLatch();  // 获取写锁
try {
    // 修改页面内容
    char *data = page->get_data();
    memcpy(data, new_data, PAGE_SIZE);
    page->is_dirty_ = true;
} finally {
    page->WUnlatch();  // 释放写锁
    buffer_pool_manager->unpin_page(page_id, true);
}
```

## ⚠️ 常见问题与解决方案

### 1. 内存泄漏

**问题**：页面未正确释放，导致内存泄漏
**解决方案**：
- 确保每次 `fetch_page()` 后都有对应的 `unpin_page()`
- 检查固定计数，确保页面最终能被释放
- 使用智能指针管理页面生命周期

### 2. 脏页未写回

**问题**：脏页未及时写回磁盘，导致数据丢失
**解决方案**：
- 在页面淘汰前检查并写回脏页
- 定期调用 `flush_page()` 刷新脏页
- 系统关闭前确保所有脏页写回

### 3. 并发冲突

**问题**：多线程并发访问导致数据不一致
**解决方案**：
- 使用互斥锁保护共享数据结构
- 对页面使用读写锁支持并发访问
- 遵循两阶段封锁协议

### 4. 替换策略选择

**问题**：选择不合适的替换策略影响性能
**解决方案**：
- 内存充足时使用LRU策略
- 内存受限时使用Clock策略
- 根据实际工作负载选择合适的策略

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第10章 存储管理
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 存储管理相关讲座
- [BusTub](https://github.com/cmu-db/bustub) - 存储管理实现参考

## 🌟 总结

存储管理模块是数据库系统的基础，其性能直接影响整个系统的运行效率。通过理解和实现存储管理模块，你将掌握：

- 磁盘I/O操作的优化
- 内存缓冲池的管理
- 页面替换策略的实现
- 并发控制的应用

这些知识对于理解数据库系统的内部工作原理至关重要，也是实现高性能数据库系统的基础。

---

**Happy Coding!** 🚀
