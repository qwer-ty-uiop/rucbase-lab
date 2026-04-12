# 查询执行模块详解

## 📋 模块概述

查询执行模块负责执行SQL查询，包括扫描、连接、排序等操作。该模块实现了火山模型（Volcano Model）的执行引擎，通过迭代器模式处理查询操作，是数据库系统中处理用户查询的核心组件。

## 🎯 核心功能

- **执行算子**：实现各种查询执行算子，如扫描、连接、投影等
- **执行计划**：根据查询计划执行相应的操作
- **结果处理**：处理查询结果并返回给用户
- **并发执行**：支持并发查询执行

## 📁 目录结构

```
src/execution/
├── executor.h/cpp            # 执行器基类
├── executor_seq_scan.h/cpp   # 顺序扫描执行器
├── executor_index_scan.h/cpp # 索引扫描执行器
├── executor_nlj.h/cpp        # 嵌套循环连接执行器
├── executor_projection.h/cpp # 投影执行器
├── executor_filter.h/cpp     # 过滤执行器
├── executor_insert.h/cpp     # 插入执行器
├── executor_delete.h/cpp     # 删除执行器
├── executor_update.h/cpp     # 更新执行器
└── README.md                # 模块文档
```

## 🏗️ 核心组件

### 1. AbstractExecutor

#### 功能概述
执行器的基类，定义了执行器的通用接口。

#### 核心方法

| 方法 | 功能 | 说明 |
|------|------|------|
| `Init()` | 初始化执行器 | 准备执行环境 |
| `Next()` | 获取下一条记录 | 返回是否有更多记录 |
| `GetOutputSchema()` | 获取输出 schema | 返回执行器的输出 schema |

### 2. SeqScanExecutor

#### 功能概述
实现顺序扫描表中的所有记录。

#### 关键数据结构
```cpp
class SeqScanExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;  // 表信息
    RmScan *scan_;          // 记录扫描器
};
```

### 3. IndexScanExecutor

#### 功能概述
使用索引进行扫描，提高查询效率。

#### 关键数据结构
```cpp
class IndexScanExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;  // 表信息
    IxIndexHandle *index_handle_;  // 索引句柄
    IxScan *scan_;          // 索引扫描器
    char *lower_key_;       // 下界键
    char *upper_key_;       // 上界键
};
```

### 4. NestedLoopJoinExecutor

#### 功能概述
实现嵌套循环连接操作。

#### 关键数据结构
```cpp
class NestedLoopJoinExecutor : public AbstractExecutor {
private:
    AbstractExecutor *left_executor_;  // 左表执行器
    AbstractExecutor *right_executor_; // 右表执行器
    Tuple left_tuple_;                // 当前左表记录
    bool left_tuple_valid_;           // 左表记录是否有效
};
```

### 5. ProjectionExecutor

#### 功能概述
实现投影操作，选择指定的列。

#### 关键数据结构
```cpp
class ProjectionExecutor : public AbstractExecutor {
private:
    AbstractExecutor *child_executor_;  // 子执行器
    std::vector<Column> columns_;       // 投影列
};
```

### 6. FilterExecutor

#### 功能概述
实现过滤操作，根据条件筛选记录。

#### 关键数据结构
```cpp
class FilterExecutor : public AbstractExecutor {
private:
    AbstractExecutor *child_executor_;  // 子执行器
    Expr *predicate_;                  // 过滤条件
};
```

### 7. InsertExecutor

#### 功能概述
实现插入操作，向表中插入记录。

#### 关键数据结构
```cpp
class InsertExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;  // 表信息
    std::vector<Tuple> tuples_;  // 待插入的记录
    size_t tuple_idx_;      // 当前处理的记录索引
};
```

### 8. DeleteExecutor

#### 功能概述
实现删除操作，从表中删除记录。

#### 关键数据结构
```cpp
class DeleteExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;  // 表信息
    AbstractExecutor *child_executor_;  // 子执行器
};
```

### 9. UpdateExecutor

#### 功能概述
实现更新操作，更新表中的记录。

#### 关键数据结构
```cpp
class UpdateExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;  // 表信息
    AbstractExecutor *child_executor_;  // 子执行器
    std::vector<UpdateInfo> updates_;  // 更新信息
};
```

## 🔧 实现步骤

### 1. 实现 AbstractExecutor

1. **定义接口**：声明 `Init()`, `Next()`, `GetOutputSchema()` 方法
2. **虚析构函数**：确保派生类正确析构

### 2. 实现 SeqScanExecutor

1. **初始化**：获取表信息，创建记录扫描器
2. **Next()**：使用扫描器获取下一条记录
3. **资源管理**：在析构函数中清理扫描器

### 3. 实现 IndexScanExecutor

1. **初始化**：获取表和索引信息，创建索引扫描器
2. **Next()**：使用索引扫描器获取下一条记录
3. **资源管理**：在析构函数中清理扫描器

### 4. 实现 NestedLoopJoinExecutor

1. **初始化**：初始化左右表执行器
2. **Next()**：实现嵌套循环连接逻辑
3. **资源管理**：在析构函数中清理执行器

### 5. 实现其他执行器

类似地实现 ProjectionExecutor, FilterExecutor, InsertExecutor, DeleteExecutor, UpdateExecutor。

## 📝 代码示例

### 顺序扫描执行器示例

```cpp
// 顺序扫描执行器
class SeqScanExecutor : public AbstractExecutor {
private:
    TableInfo *table_info_;
    RmScan *scan_;
    
public:
    SeqScanExecutor(ExecuteContext *exec_ctx, const TabName &table_name)
        : AbstractExecutor(exec_ctx), table_name_(table_name) {}
    
    void Init() override {
        table_info_ = exec_ctx_->GetCatalog()->GetTable(table_name_);
        scan_ = new RmScan(table_info_->file_handle_);
    }
    
    bool Next(Tuple *tuple) override {
        Rid rid;
        if (scan_->next(&rid)) {
            // 读取记录
            char *record = new char[table_info_->table_meta_.record_size];
            table_info_->file_handle_->get_record(rid, record);
            
            // 转换为Tuple
            tuple->InitFromRecord(record, table_info_->table_meta_.schema_);
            delete[] record;
            return true;
        }
        return false;
    }
    
    Schema GetOutputSchema() override {
        return table_info_->table_meta_.schema_;
    }
};
```

### 嵌套循环连接执行器示例

```cpp
// 嵌套循环连接执行器
class NestedLoopJoinExecutor : public AbstractExecutor {
private:
    AbstractExecutor *left_executor_;
    AbstractExecutor *right_executor_;
    Tuple left_tuple_;
    bool left_tuple_valid_;
    
public:
    NestedLoopJoinExecutor(ExecuteContext *exec_ctx, AbstractExecutor *left, AbstractExecutor *right)
        : AbstractExecutor(exec_ctx), left_executor_(left), right_executor_(right) {}
    
    void Init() override {
        left_executor_->Init();
        right_executor_->Init();
        left_tuple_valid_ = left_executor_->Next(&left_tuple_);
    }
    
    bool Next(Tuple *tuple) override {
        while (left_tuple_valid_) {
            Tuple right_tuple;
            while (right_executor_->Next(&right_tuple)) {
                // 检查连接条件
                if (CheckJoinCondition(left_tuple_, right_tuple_)) {
                    // 合并元组
                    *tuple = MergeTuples(left_tuple_, right_tuple_);
                    return true;
                }
            }
            
            // 右表扫描完毕，重置右表扫描器
            right_executor_->Init();
            left_tuple_valid_ = left_executor_->Next(&left_tuple_);
        }
        return false;
    }
    
    Schema GetOutputSchema() override {
        // 合并左右表的schema
        return MergeSchemas(left_executor_->GetOutputSchema(), right_executor_->GetOutputSchema());
    }
};
```

## 🚀 使用方法

### 基本使用流程

1. **创建执行器**：
   ```cpp
   // 创建顺序扫描执行器
   SeqScanExecutor *seq_scan = new SeqScanExecutor(exec_ctx, "student");
   
   // 创建过滤执行器
   FilterExecutor *filter = new FilterExecutor(exec_ctx, seq_scan, predicate);
   
   // 创建投影执行器
   ProjectionExecutor *projection = new ProjectionExecutor(exec_ctx, filter, columns);
   ```

2. **初始化执行器**：
   ```cpp
   projection->Init();
   ```

3. **执行查询**：
   ```cpp
   Tuple tuple;
   while (projection->Next(&tuple)) {
       // 处理查询结果
       tuple.Print();
   }
   ```

4. **清理资源**：
   ```cpp
   delete projection;
   delete filter;
   delete seq_scan;
   ```

## ⚠️ 常见问题与解决方案

### 1. 执行效率低

**问题**：查询执行效率低，特别是对于大表
**解决方案**：
- 使用索引扫描代替顺序扫描
- 优化连接算法，如使用哈希连接或排序合并连接
- 实现查询优化，选择最优执行计划

### 2. 内存使用高

**问题**：执行器使用过多内存，导致系统性能下降
**解决方案**：
- 使用流水线执行，减少内存使用
- 实现批处理，减少内存分配次数
- 对大结果集使用游标，避免一次性加载所有数据

### 3. 并发冲突

**问题**：多线程并发执行查询时出现冲突
**解决方案**：
- 使用适当的锁机制，保护共享资源
- 实现乐观并发控制，减少锁竞争
- 合理设计执行器，支持并发执行

### 4. 错误处理

**问题**：执行过程中出现错误，没有适当的错误处理
**解决方案**：
- 实现异常处理机制，捕获和处理执行过程中的错误
- 提供详细的错误信息，便于调试
- 确保资源正确释放，避免资源泄漏

## 📊 执行算子性能对比

| 算子 | 适用场景 | 优点 | 缺点 |
|------|----------|------|------|
| **SeqScan** | 小表，无索引 | 实现简单 | 全表扫描，性能差 |
| **IndexScan** | 有索引的表 | 性能好 | 依赖索引 |
| **NestedLoopJoin** | 小表连接 | 实现简单 | 时间复杂度高 |
| **HashJoin** | 大表连接 | 性能好 | 内存消耗大 |
| **SortMergeJoin** | 有序数据连接 | 性能好 | 需要排序 |
| **Projection** | 选择部分列 | 减少数据传输 | 增加计算开销 |
| **Filter** | 条件筛选 | 减少处理数据量 | 增加计算开销 |

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第12章 查询处理
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 查询执行讲座
- [Volcano: An Extensible and Parallel Query Evaluation System](https://dl.acm.org/doi/10.1145/971701.971703) - 火山模型原始论文

## 🌟 总结

查询执行模块是数据库系统中处理用户查询的核心组件。通过理解和实现查询执行模块，你将掌握：

- 火山模型的原理和实现
- 各种执行算子的实现方法
- 查询执行的优化技巧
- 并发执行的处理方法

这些知识对于理解数据库系统的内部工作原理至关重要，也是实现高性能数据库系统的基础。

---

**Happy Coding!** 🚀
