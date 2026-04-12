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

## 📖 模块文档

每个模块都有详细的实现文档，位于对应模块目录下：

- **存储管理**: `src/storage/README.md` - 磁盘管理、缓冲池管理、页面管理
- **替换策略**: `src/replacer/README.md` - LRU和Clock算法实现
- **记录管理**: `src/record/README.md` - 记录存储和扫描
- **索引管理**: `src/index/README.md` - B+树索引实现
- **事务管理**: `src/transaction/README.md` - 事务和并发控制
- **故障恢复**: `src/recovery/README.md` - 日志和恢复机制
- **查询执行**: `src/execution/README.md` - 查询执行引擎
- **SQL解析**: `src/parser/README.md` - SQL语法解析
- **语义分析**: `src/analyze/README.md` - 语义检查和类型推断
- **查询优化**: `src/optimizer/README.md` - 查询计划优化
- **系统管理**: `src/system/README.md` - 元数据管理

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
