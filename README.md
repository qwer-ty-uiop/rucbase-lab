# RucBase - 关系型数据库管理系统实验

> 一个完整的关系型数据库管理系统(RDBMS)原型，用于《数据库系统实现》课程实验教学

## 📚 项目背景

RucBase 是一个基于 C++17 实现的关系型数据库管理系统原型，参考了以下经典数据库系统设计：

- **CMU 15-445** 的 [BusTub](https://github.com/cmu-db/bustub)
- **Stanford CS346** 的 [Redbase](https://web.stanford.edu/class/cs346/2015/redbase.html)

该项目旨在帮助学生理解数据库系统的内部实现原理，从底层存储到上层查询的全栈实现，涵盖了 RDBMS 的核心组件。

## 🏗️ 系统架构

RucBase 采用模块化设计，各模块之间通过清晰的接口进行交互：

```
┌─────────────────────────────────────────────────────────┐
│                     应用层                            │
├─────────────────────────────────────────────────────────┤
│    SQL 解析器    │  语义分析  │  查询优化器  │  执行引擎  │
├─────────────────────────────────────────────────────────┤
│                    核心服务层                            │
├─────────────────────────────────────────────────────────┤
│  事务管理  │  并发控制  │  故障恢复  │  索引管理  │  系统管理  │
├─────────────────────────────────────────────────────────┤
│                    存储层                              │
├─────────────────────────────────────────────────────────┤
│  记录管理  │  缓冲池管理  │  磁盘管理  │  页面管理  │  替换策略  │
└─────────────────────────────────────────────────────────┘
```

## 📁 项目结构

```
rucbase-lab/
├── src/                 # 核心源代码
│   ├── storage/         # 存储管理
│   ├── replacer/         # 缓冲区替换策略
│   ├── record/           # 记录管理
│   ├── index/            # B+树索引
│   ├── transaction/      # 事务管理
│   ├── recovery/         # 故障恢复
│   ├── execution/        # 查询执行
│   ├── parser/           # SQL解析器
│   ├── analyze/          # 语义分析
│   ├── optimizer/        # 查询优化
│   ├── system/           # 系统管理
│   └── common/           # 公共组件
├── rucbase_client/       # 客户端
├── ownbase/              # 学生代码目录
├── pics/                 # 架构图示
├── build_wsl.sh          # WSL编译脚本
└── README.md            # 项目文档
```

## 🎯 功能模块详解

### 1. 存储管理 (Storage)

#### 功能概述
负责数据库的物理存储，包括磁盘文件管理、页面管理和缓冲池管理。

#### 核心组件
- **DiskManager**: 管理磁盘文件的读写操作
- **BufferPoolManager**: 管理内存中的页面缓存
- **Page**: 数据页的抽象，4KB固定大小

#### 关键实现
- 页面分配与回收
- 脏页写回机制
- 并发控制支持

#### 代码示例
```cpp
// 从缓冲池获取页面
Page* page = buffer_pool_manager->fetch_page(page_id);
// 修改页面内容
memcpy(page->get_data(), data, PAGE_SIZE);
// 标记为脏页
page->is_dirty_ = true;
// 释放页面
buffer_pool_manager->unpin_page(page_id, true);
```

### 2. 替换策略 (Replacer)

#### 功能概述
实现缓冲区页面的替换算法，当缓冲池满时选择淘汰页面。

#### 核心算法
- **LRU (Least Recently Used)**: 最近最少使用算法
- **Clock**: 时钟算法（二次机会算法）

#### 实现原理
- LRU: 使用双向链表+哈希表，O(1)时间复杂度
- Clock: 使用环形缓冲区，减少内存开销

### 3. 记录管理 (Record)

#### 功能概述
管理表中的记录，包括记录的插入、删除、更新和扫描。

#### 核心组件
- **RmFileHandle**: 管理文件级别的记录操作
- **RmScan**: 记录扫描器
- **Bitmap**: 管理记录的空闲空间

#### 实现原理
- 定长记录存储
- 页内记录组织
- 空间管理（Bitmap）

### 4. 索引管理 (Index)

#### 功能概述
实现B+树索引，加速数据查询。

#### 核心组件
- **IxIndexHandle**: B+树索引管理
- **IxNodeHandle**: 节点操作
- **IxScan**: 索引扫描

#### 实现原理
- B+树数据结构
- 节点分裂与合并
- 并发控制

#### 代码示例
```cpp
// 创建索引
ix_manager->create_index("student", {id_col});
// 插入索引项
index_handle->insert_entry(key, rid, txn);
// 查找
std::vector<Rid> results;
index_handle->get_value(key, &results, txn);
```

### 5. 事务管理 (Transaction)

#### 功能概述
管理数据库事务，确保ACID特性。

#### 核心组件
- **TransactionManager**: 事务生命周期管理
- **LockManager**: 并发控制锁管理

#### 实现原理
- 两阶段封锁协议 (2PL)
- 死锁检测与预防
- 行级锁、表级锁、意向锁

### 6. 故障恢复 (Recovery)

#### 功能概述
实现数据库的故障恢复机制。

#### 核心组件
- **LogManager**: 日志管理
- **LogRecovery**: 恢复算法

#### 实现原理
- WAL (Write-Ahead Logging)
- ARIES 恢复算法
- 检查点机制

### 7. 查询执行 (Execution)

#### 功能概述
执行SQL查询，包括扫描、连接、排序等操作。

#### 核心算子
- **SeqScan**: 顺序扫描
- **IndexScan**: 索引扫描
- **NestedLoopJoin**: 嵌套循环连接
- **Projection**: 投影
- **Insert/Delete/Update**: 数据修改

#### 实现原理
- 火山模型 (Volcano Model)
- 迭代器模式
- 流水线执行

### 8. SQL解析 (Parser)

#### 功能概述
解析SQL语句，生成抽象语法树。

#### 核心组件
- **Lexer**: 词法分析
- **Parser**: 语法分析
- **AST**: 抽象语法树

#### 实现原理
- Flex 词法分析器
- Bison 语法分析器
- 递归下降解析

### 9. 语义分析 (Analyze)

#### 功能概述
对SQL语句进行语义检查和类型推断。

#### 核心功能
- 表和列的存在性检查
- 类型兼容性检查
- 表达式求值

### 10. 查询优化 (Optimizer)

#### 功能概述
生成最优执行计划。

#### 核心功能
- 基于规则的优化
- 执行计划生成
- 成本估算

### 11. 系统管理 (System)

#### 功能概述
管理数据库、表、索引等元数据。

#### 核心组件
- **SmManager**: 系统管理器
- **DbMeta/TabMeta/ColMeta**: 元数据结构

#### 实现原理
- 元数据存储与管理
- 目录结构维护
- 系统目录表

## 🛠️ 环境配置

### 支持平台
- **Ubuntu 18.04+** (64位)
- **WSL 2.0** (Windows Subsystem for Linux)

### 依赖项
- **C++17** 兼容编译器
- **CMake 3.16+**
- **Flex** (词法分析)
- **Bison** (语法分析)
- **libreadline-dev** (命令行交互)

### 快速配置 (WSL)
```bash
# 1. 安装依赖
sudo apt-get update
sudo apt-get install -y build-essential cmake g++ flex bison libreadline-dev

# 2. 编译项目
./build_wsl.sh

# 3. 运行
cd build
./bin/rmdb
```

## 📖 学习路径

### 实验顺序
1. **Lab 1: 存储管理** (15h) - 实现磁盘管理和缓冲池
2. **Lab 2: 索引管理** (35h) - 实现B+树索引
3. **Lab 3: 查询执行** (30-40h) - 实现查询执行引擎
4. **Lab 4: 并发控制** (25-30h) - 实现事务和锁管理

### 学习建议
1. **从底层开始**：先理解存储管理，再学习上层功能
2. **实验驱动**：通过完成实验来巩固理解
3. **代码阅读**：阅读现有代码，理解设计思路
4. **调试技巧**：使用GDB进行调试，理解执行流程

## 🚀 使用方法

### 启动服务器
```bash
# 启动数据库服务器
./bin/rmdb <database_name>
```

### 客户端连接
```bash
# 在另一个终端启动客户端
cd rucbase_client/build
./rmdb_client
```

### SQL示例
```sql
-- 创建表
CREATE TABLE student (id INT, name VARCHAR(20), age INT);

-- 插入数据
INSERT INTO student VALUES (1, 'Alice', 20);

-- 查询数据
SELECT * FROM student;

-- 创建索引
CREATE INDEX idx_student_id (id);

-- 事务操作
BEGIN;
INSERT INTO student VALUES (2, 'Bob', 21);
COMMIT;
```

## 📝 配置与调优

### 配置参数
- **REPLACER_TYPE**: 缓冲区替换策略 ("LRU" 或 "CLOCK")
- **BUFFER_POOL_SIZE**: 缓冲池大小
- **LOG_FILE**: 日志文件路径

### 性能优化
- 调整缓冲池大小以适应内存
- 合理使用索引提高查询性能
- 优化SQL语句，减少全表扫描

## 🔧 测试与调试

### 单元测试
```bash
# 运行所有单元测试
./bin/unit_test

# 运行特定测试
./bin/unit_test --gtest_filter=BufferPoolManagerTest.SampleTest
```

### 功能测试
```bash
# 运行完整功能测试
./test_full_function.sh
```

### 调试技巧
- **GDB调试**：`gdb ./bin/rmdb`
- **日志查看**：检查系统日志文件
- **性能分析**：使用 `perf` 工具

## 🎯 关键模块交互

### 数据写入流程
1. **SQL解析** → 生成AST
2. **语义分析** → 验证SQL正确性
3. **查询优化** → 生成执行计划
4. **执行引擎** → 执行插入操作
5. **记录管理** → 写入记录
6. **缓冲池** → 管理页面
7. **磁盘管理** → 持久化存储
8. **事务管理** → 确保ACID
9. **日志管理** → 记录操作

### 查询流程
1. **SQL解析** → 生成AST
2. **语义分析** → 验证SQL正确性
3. **查询优化** → 选择执行计划
4. **执行引擎** → 执行查询
5. **索引管理** → 加速数据检索
6. **记录管理** → 读取记录
7. **缓冲池** → 缓存页面
8. **事务管理** → 保证一致性

## ⚠️ 注意事项

### 常见问题
1. **编译错误**：确保依赖项已安装，使用正确的C++17编译器
2. **内存泄漏**：检查页面是否正确释放，使用Valgrind检测
3. **死锁**：避免循环依赖，合理设计事务顺序
4. **性能问题**：检查索引使用，优化SQL语句

### 安全建议
- 不要在生产环境使用
- 定期备份数据库文件
- 避免使用过大的缓冲池

## 🌟 项目特点

1. **教学导向**：针对数据库系统课程设计，循序渐进
2. **完整系统**：从底层存储到上层查询的全栈实现
3. **工业级设计**：参考CMU BusTub等成熟项目
4. **丰富文档**：详细的结构说明和实验指导
5. **可扩展性**：模块化设计，便于添加新功能

## 📚 参考资料

- [Database System Concepts](https://www.db-book.com/)
- [CMU 15-445: Database Systems](https://15445.courses.cs.cmu.edu/fall2022/)
- [Stanford CS346: Database Systems Implementation](https://web.stanford.edu/class/cs346/2015/)
- [BusTub](https://github.com/cmu-db/bustub)
- [Redbase](https://web.stanford.edu/class/cs346/2015/redbase.html)

## 👥 贡献

欢迎提交Issue和Pull Request来改进项目！

## 📄 许可证

本项目采用 Mulan PSL v2 许可证。

## 🎉 总结

RucBase 是一个理想的数据库系统学习平台，通过实践理解数据库内部工作原理。无论是课程学习还是个人兴趣，都能从中获得宝贵的数据库系统实现经验。

---

**Happy Coding!** 🚀
