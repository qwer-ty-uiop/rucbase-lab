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

| 方法             | 功能       | 实现要点                                    |
| ---------------- | ---------- | ------------------------------------------- |
| `create_file()`  | 创建新文件 | 使用 `open()` 系统调用，指定 `O_CREAT` 标志 |
| `destroy_file()` | 删除文件   | 先从映射表中移除，再调用 `unlink()`         |
| `open_file()`    | 打开文件   | 检查文件是否已打开，否则调用 `open()`       |
| `close_file()`   | 关闭文件   | 调用 `close()` 并从映射表中移除             |
| `write_page()`   | 写入页面   | 计算偏移量，调用 `write()`，检查写入大小    |
| `read_page()`    | 读取页面   | 计算偏移量，调用 `read()`，检查读取大小     |

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

| 方法                        | 功能          | 实现要点                                |
| --------------------------- | ------------- | --------------------------------------- |
| `get_page_id()`             | 获取页面ID    | 直接返回 `id_`                          |
| `get_data()`                | 获取页面数据  | 返回 `data_` 指针                       |
| `is_dirty()`                | 检查是否脏页  | 返回 `is_dirty_`                        |
| `pin_count()`               | 获取固定计数  | 返回 `pin_count_`                       |
| `WLatch()` / `RLatch()`     | 获取写锁/读锁 | 调用 `rwlatch_.WLock()` / `RLock()`     |
| `WUnlatch()` / `RUnlatch()` | 释放写锁/读锁 | 调用 `rwlatch_.WUnlock()` / `RUnlock()` |

#### 实现注意事项

- **页面大小**：固定为4KB，与磁盘块大小对齐
- **并发控制**：使用读写锁支持多线程并发访问
- **脏页标记**：及时标记和清除脏页状态
- **固定计数**：正确管理页面的固定和释放

### 3. BufferPoolManager

#### 功能概述

管理内存中的页面缓存，负责页面的分配、回收和替换。缓冲池是全局的，所有文件共享同一个缓冲池，大小固定为 `BUFFER_POOL_SIZE`（默认 65536 个页框，即 256MB）。

#### 关键数据结构

```cpp
class BufferPoolManager {
private:
    std::unordered_map<PageId, frame_id_t, PageIdHash> page_table_;  // PageId → 帧编号映射
    std::list<frame_id_t> free_list_;       // 空闲帧编号链表
    DiskManager* disk_manager_;             // 磁盘管理器（不拥有）
    std::unique_ptr<Replacer> replacer_;    // 页面替换策略（LRU/Clock）
    std::unique_ptr<Page[]> pages_;         // 页面数组，堆上分配
    std::shared_mutex latch_;               // 读写锁，读多写少场景下减少并发冲突
    size_t pool_size_;                      // 缓冲池可容纳的页框数量
};
```

#### 核心方法

| 方法                | 功能                                         | 锁策略                                |
| ------------------- | -------------------------------------------- | ------------------------------------- |
| `fetch_page()`      | 获取页面，命中则直接返回，未命中则从磁盘加载 | 读写锁 + 双重检查                     |
| `unpin_page()`      | 释放页面引用，减少 pin_count                 | 读锁快速路径 + 写锁修改               |
| `flush_page()`      | 将指定页面写回磁盘                           | 三阶段锁（读锁复制→无锁I/O→写锁清脏） |
| `new_page()`        | 创建新页面，分配 page_no 并加载到缓冲池      | 写锁全程                              |
| `delete_page()`     | 从缓冲池删除页面，脏页写回磁盘               | 写锁修改元数据→无锁I/O                |
| `flush_all_pages()` | 将指定文件的所有页面写回磁盘                 | 三阶段锁（逐页刷新）                  |

#### 并发锁优化策略

缓冲池是典型的**读多写少**场景：大部分请求是页面命中直接返回，只有未命中时才需要修改数据结构。原始实现使用 `std::mutex` 互斥锁，所有操作串行执行，并发性能差。优化后使用 `std::shared_mutex` 读写锁，遵循两条核心原则：

##### 原则一：读写锁分离

| 场景                              | 原方案（mutex） | 优化后（shared_mutex）      |
| --------------------------------- | --------------- | --------------------------- |
| 多线程同时 fetch 不同页面（命中） | 串行等待        | **并行执行**（shared_lock） |
| 多线程同时检查页面是否存在        | 串行等待        | **并行执行**（shared_lock） |
| 修改数据结构（未命中/淘汰/创建）  | 互斥            | 独占写锁（unique_lock）     |

- **读锁（shared_lock）**：用于只读访问共享数据结构（如 page*table* 查找），允许多线程并发持有
- **写锁（unique_lock）**：用于修改共享数据结构（如 page*table* 插入/删除、free*list* 操作），独占持有

##### 原则二：I/O 期间释放锁

磁盘 I/O 是毫秒级慢操作，持锁期间做 I/O 会严重阻塞其他线程。优化方式为**三阶段锁策略**：

```
阶段1：持读锁 → 复制页面数据到栈上缓冲区 → 释放读锁
阶段2：无锁 → 执行磁盘写入（慢操作，不阻塞其他线程）
阶段3：持写锁 → 验证映射关系未变 → 更新元数据（清脏标记）→ 释放写锁
```

**为什么阶段3需要重新验证映射关系？** 因为在释放锁期间，其他线程可能执行了页面淘汰操作，导致该页面被替换到其他帧，甚至被完全换出缓冲池。验证 `page_table_[pid] == fid` 确保我们修改的是正确的页面。

##### 各函数锁策略详解

**fetch_page — 读写锁 + 双重检查模式**

```
第一步：shared_lock 查找 page_table_
  ├─ 命中（高频路径）：pin + pin_count++ → 返回页面（多线程可并发执行）
  └─ 未命中：释放读锁

第二步：unique_lock 修改数据结构
  ├─ 双重检查：再次查找 page_table_（防止其他线程已加载该页面）
  │   ├─ 命中：同第一步命中逻辑
  │   └─ 未命中：find_victim_page → update_page → read_page → pin → 返回
  └─ 释放写锁
```

双重检查的必要性：线程A和线程B同时请求同一页面，A先获取写锁完成加载；B获取写锁后如果不再次检查，会重复加载同一页面。

**unpin_page — 读锁快速路径**

```
第一步：shared_lock 检查页面是否存在
  ├─ 不存在：直接返回 false（避免无谓的写锁竞争）
  └─ 存在：释放读锁

第二步：unique_lock 修改 pin_count 和 replacer 状态
```

**flush_page — 三阶段锁**

```
阶段1：shared_lock → 查找页面 → memcpy 复制数据 → 释放读锁
阶段2：无锁 → disk_manager_->write_page()（磁盘I/O，不阻塞其他线程）
阶段3：unique_lock → 验证 page_table_[pid] == fid → 清除 is_dirty_ → 释放写锁
```

**delete_page — 先修改元数据，后写磁盘**

```
第一步：unique_lock → 查找页面 → 复制脏页数据 → 从 page_table_ 移除
       → 重置页面元数据 → 加入 free_list_ → 释放写锁
第二步：无锁 → 如果是脏页，写回磁盘（此时页面已从缓冲池移除，不影响其他线程）
```

**flush_all_pages — 逐页三阶段刷新**

```
阶段1：shared_lock → 遍历所有页框，收集目标文件的页面列表 → 释放读锁
阶段2：逐页执行三阶段刷新（复制 → 写磁盘 → 清脏标记）
       避免在遍历 65536 个页框和磁盘 I/O 期间持锁
```

**new_page — 写锁全程**

new*page 需要同时修改 free_list*、page*table*、replacer\_ 等多个数据结构，无法分段释放锁，因此全程持有写锁。

#### 实现注意事项

- **页面替换**：当缓冲池满时，优先从 free*list* 获取空闲帧，其次使用替换策略（LRU/Clock）选择淘汰页面
- **脏页管理**：淘汰脏页前必须写回磁盘，确保数据不丢失
- **双重检查**：fetch_page 获取写锁后必须再次检查页面是否已被其他线程加载
- **映射验证**：三阶段锁的第三阶段必须验证 page*table* 映射未变，防止修改错误的页面
- **pin 语义**：pin 表示页面正在被使用（不可淘汰），unpin 表示页面可被淘汰（加入 replacer 候选）

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
// 获取页面（读写锁 + 双重检查模式）
Page *BufferPoolManager::fetch_page(PageId page_id) {
    // 第一步：读锁检查页面是否已在缓冲池（高频路径，允许并发读）
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            // 页面命中：增加引用计数，标记为正在使用
            frame_id_t frame_id = it->second;
            replacer_->pin(frame_id);
            pages_[frame_id].pin_count_++;
            return &pages_[frame_id];
        }
    }

    // 第二步：页面未命中，需要写锁修改数据结构（低频路径）
    {
        std::unique_lock lock{latch_};
        // 双重检查：获取写锁后再次检查，防止其他线程已加载该页面
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            replacer_->pin(it->second);
            pages_[it->second].pin_count_++;
            return &pages_[it->second];
        }
        // 确认未命中：淘汰页面 → 从磁盘加载
        frame_id_t frame_id;
        if (!find_victim_page(&frame_id)) return nullptr;
        Page *page = &pages_[frame_id];
        update_page(page, page_id, frame_id);
        disk_manager_->read_page(page_id.fd, page_id.page_no, page->data_, PAGE_SIZE);
        replacer_->pin(frame_id);
        page->pin_count_ = 1;
        return page;
    }
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
