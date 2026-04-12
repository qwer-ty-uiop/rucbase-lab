# 替换策略模块详解

## 📋 模块概述

替换策略模块负责在缓冲池满时选择淘汰哪些页面，以优化内存使用效率。该模块实现了两种经典的页面替换算法：LRU（最近最少使用）和Clock（时钟算法）。

## 🎯 核心功能

- **页面替换**：当缓冲池满时选择淘汰页面
- **状态管理**：跟踪页面的使用状态
- **并发控制**：支持多线程并发操作

## 📁 目录结构

```
src/replacer/
├── lru_replacer.h/cpp        # LRU替换器
├── clock_replacer.h/cpp      # Clock替换器
├── replacer.h                # 替换器基类
└── README.md                 # 模块文档
```

## 🏗️ 核心组件

### 1. Replacer（基类）

#### 功能概述
定义替换器的接口，是所有具体替换策略的基类。

#### 关键方法

| 方法 | 功能 | 说明 |
|------|------|------|
| `Victim()` | 选择淘汰页面 | 选择一个页面淘汰，返回是否成功 |
| `Pin()` | 固定页面 | 标记页面为正在使用，不能被淘汰 |
| `Unpin()` | 取消固定 | 标记页面为可淘汰 |
| `Size()` | 获取可淘汰页面数 | 返回当前可淘汰的页面数量 |

### 2. LRUReplacer

#### 功能概述
实现最近最少使用（LRU）算法，淘汰最长时间未使用的页面。

#### 实现原理
- **数据结构**：双向链表 + 哈希表
  - 双向链表：按访问时间排序，最近访问的在头部
  - 哈希表：快速定位页面在链表中的位置
- **操作复杂度**：所有操作均为O(1)时间复杂度

#### 关键数据结构
```cpp
class LRUReplacer : public Replacer {
private:
    size_t capacity_;  // 容量
    size_t size_;      // 当前大小
    std::list<std::pair<frame_id_t, std::list<std::pair<frame_id_t, bool>>::iterator>> list_;  // 双向链表
    std::unordered_map<frame_id_t, std::list<std::pair<frame_id_t, bool>>::iterator> map_;  // 哈希表
    std::mutex latch_;  // 互斥锁，保证并发安全
};
```

#### 核心方法

| 方法 | 实现要点 |
|------|----------|
| `Victim()` | 选择链表尾部的页面淘汰，更新数据结构 |
| `Pin()` | 从链表中移除页面，从哈希表中删除 |
| `Unpin()` | 将页面添加到链表头部，更新哈希表 |
| `Size()` | 返回 `size_` |

### 3. ClockReplacer

#### 功能概述
实现时钟算法（二次机会算法），为每个页面提供第二次机会。

#### 实现原理
- **数据结构**：环形缓冲区 + 状态数组
  - 环形缓冲区：模拟时钟指针
  - 状态数组：记录每个页面的状态（EMPTY_OR_PINNED, UNTOUCHED, ACCESSED）
- **操作流程**：
  1. 时钟指针从上次淘汰位置开始
  2. 遇到ACCESSED状态的页面，标记为UNTOUCHED并继续
  3. 遇到UNTOUCHED状态的页面，选择其作为victim

#### 关键数据结构
```cpp
class ClockReplacer : public Replacer {
private:
    size_t capacity_;  // 容量
    size_t size_;      // 当前大小
    frame_id_t clock_hand_;  // 时钟指针
    enum class State { EMPTY_OR_PINNED, UNTOUCHED, ACCESSED };
    std::vector<State> status_;  // 页面状态数组
    std::mutex latch_;  // 互斥锁，保证并发安全
};
```

#### 核心方法

| 方法 | 实现要点 |
|------|----------|
| `Victim()` | 遍历环形缓冲区，寻找UNTOUCHED状态的页面 |
| `Pin()` | 将页面状态设置为EMPTY_OR_PINNED |
| `Unpin()` | 将页面状态设置为ACCESSED |
| `Size()` | 统计ACCESSED和UNTOUCHED状态的页面数 |

## 🔧 实现步骤

### 1. 实现 Replacer 基类

1. **定义接口**：声明 `Victim()`, `Pin()`, `Unpin()`, `Size()` 方法
2. **虚析构函数**：确保派生类正确析构

### 2. 实现 LRUReplacer

1. **初始化**：创建双向链表和哈希表，初始化容量和大小
2. **Victim()**：选择链表尾部的页面，更新数据结构
3. **Pin()**：从链表和哈希表中移除页面
4. **Unpin()**：将页面添加到链表头部，更新哈希表
5. **Size()**：返回当前可淘汰的页面数
6. **并发控制**：为所有操作添加互斥锁保护

### 3. 实现 ClockReplacer

1. **初始化**：创建状态数组，初始化容量、大小和时钟指针
2. **Victim()**：遍历环形缓冲区，寻找UNTOUCHED状态的页面
3. **Pin()**：将页面状态设置为EMPTY_OR_PINNED
4. **Unpin()**：将页面状态设置为ACCESSED
5. **Size()**：统计可淘汰的页面数
6. **并发控制**：为所有操作添加互斥锁保护

## 📝 代码示例

### LRUReplacer 示例

```cpp
// 淘汰页面
bool LRUReplacer::Victim(frame_id_t *frame_id) {
    if (size_ == 0) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(latch_);
    auto last = list_.end();
    --last;
    *frame_id = last->first;
    map_.erase(last->first);
    list_.pop_back();
    size_--;
    
    return true;
}

// 固定页面
void LRUReplacer::Pin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    auto it = map_.find(frame_id);
    if (it != map_.end()) {
        list_.erase(it->second);
        map_.erase(it);
        size_--;
    }
}

// 取消固定
void LRUReplacer::Unpin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (map_.find(frame_id) == map_.end()) {
        list_.push_front({frame_id, list_.begin()});
        map_[frame_id] = list_.begin();
        size_++;
    }
}
```

### ClockReplacer 示例

```cpp
// 淘汰页面
bool ClockReplacer::Victim(frame_id_t *frame_id) {
    if (size_ == 0) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(latch_);
    int count = 0;
    while (count < capacity_) {
        frame_id_t current_frame = clock_hand_;
        clock_hand_ = (clock_hand_ + 1) % capacity_;
        
        if (status_[current_frame] == ACCESSED) {
            status_[current_frame] = UNTOUCHED;
        } else if (status_[current_frame] == UNTOUCHED) {
            status_[current_frame] = EMPTY_OR_PINNED;
            *frame_id = current_frame;
            size_--;
            return true;
        }
        count++;
    }
    
    return false;
}

// 固定页面
void ClockReplacer::Pin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (status_[frame_id] == ACCESSED || status_[frame_id] == UNTOUCHED) {
        status_[frame_id] = EMPTY_OR_PINNED;
        size_--;
    }
}

// 取消固定
void ClockReplacer::Unpin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(latch_);
    if (status_[frame_id] == EMPTY_OR_PINNED) {
        status_[frame_id] = ACCESSED;
        size_++;
    }
}
```

## 🚀 使用方法

### 在 BufferPoolManager 中使用

1. **创建替换器**：
   ```cpp
   // 使用LRU策略
   Replacer *replacer = new LRUReplacer(pool_size_);
   
   // 或使用Clock策略
   Replacer *replacer = new ClockReplacer(pool_size_);
   ```

2. **页面淘汰**：
   ```cpp
   frame_id_t frame_id = -1;
   if (replacer_->Victim(&frame_id)) {
       // 淘汰该页面
       Page *victim_page = pages_ + frame_id;
       if (victim_page->is_dirty_) {
           disk_manager_->write_page(victim_page->get_page_id(), victim_page->get_data());
       }
       page_table_.erase(victim_page->get_page_id());
   }
   ```

3. **页面固定**：
   ```cpp
   // 页面被使用时
   replacer_->Pin(frame_id);
   ```

4. **页面取消固定**：
   ```cpp
   // 页面不再使用时
   replacer_->Unpin(frame_id);
   ```

## ⚠️ 常见问题与解决方案

### 1. 页面状态管理错误

**问题**：页面状态管理不当，导致错误的页面被淘汰
**解决方案**：
- 确保每次页面访问后正确更新状态
- 检查固定和取消固定操作的逻辑
- 验证替换算法的正确性

### 2. 并发安全问题

**问题**：多线程并发访问导致数据不一致
**解决方案**：
- 为所有操作添加互斥锁保护
- 确保状态更新的原子性
- 避免死锁和竞态条件

### 3. 性能优化

**问题**：替换策略性能不佳，影响系统整体性能
**解决方案**：
- 选择合适的替换策略（LRU vs Clock）
- 优化数据结构，减少操作时间复杂度
- 考虑批量操作，减少锁竞争

### 4. 内存使用

**问题**：替换策略本身占用过多内存
**解决方案**：
- LRU：注意链表和哈希表的内存开销
- Clock：使用紧凑的状态表示
- 根据实际情况选择内存效率更高的算法

## 📊 算法对比

| 特性 | LRU | Clock |
|------|-----|-------|
| **时间复杂度** | O(1) | O(n)，但实际接近O(1) |
| **空间复杂度** | O(n) | O(n)，但更紧凑 |
| **实现复杂度** | 较高 | 较低 |
| **适用场景** | 内存充足，追求最佳性能 | 内存受限，追求空间效率 |
| **优点** | 替换策略最优 | 实现简单，内存开销小 |
| **缺点** | 内存开销大 | 替换策略不是最优 |

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第10章 缓冲区管理
- [Operating System Concepts](https://www.os-book.com/) - 内存管理相关章节
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 缓冲池管理讲座

## 🌟 总结

替换策略模块是缓冲池管理的重要组成部分，其性能直接影响数据库系统的整体性能。通过理解和实现替换策略模块，你将掌握：

- 经典页面替换算法的原理
- 数据结构的选择和优化
- 并发控制的应用
- 性能调优的方法

这些知识对于理解操作系统和数据库系统的内存管理至关重要，也是实现高性能系统的基础。

---

**Happy Coding!** 🚀
