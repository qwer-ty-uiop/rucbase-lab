# 查询优化模块详解

## 📋 模块概述

查询优化模块负责生成最优执行计划，提高查询执行效率。该模块在语义分析之后执行，根据SQL语句的结构和数据库的统计信息，选择最合适的执行策略，是数据库系统中提高查询性能的关键组件。

## 🎯 核心功能

- **逻辑优化**：应用启发式规则优化逻辑执行计划
- **物理优化**：选择合适的物理操作符和访问路径
- **成本估算**：估算不同执行计划的成本
- **计划选择**：选择成本最低的执行计划
- **统计信息管理**：收集和维护数据库统计信息

## 📁 目录结构

```
src/optimizer/
├── optimizer.h/cpp          # 查询优化器
├── logical_plan.h/cpp       # 逻辑执行计划
├── physical_plan.h/cpp      # 物理执行计划
├── cost_model.h/cpp         # 成本模型
├── statistics.h/cpp         # 统计信息
└── README.md                # 模块文档
```

## 🏗️ 核心组件

### 1. QueryOptimizer

#### 功能概述
查询优化器，负责生成最优执行计划。

#### 关键数据结构
```cpp
class QueryOptimizer {
private:
    Catalog *catalog_;  // 目录管理器，提供表和索引信息
    Statistics *statistics_;  // 统计信息管理器
    CostModel *cost_model_;  // 成本模型
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `Optimize()` | 优化查询 | 生成最优执行计划 |
| `GenerateLogicalPlan()` | 生成逻辑执行计划 | 根据抽象语法树生成逻辑计划 |
| `ApplyLogicalOptimizations()` | 应用逻辑优化 | 应用启发式规则优化逻辑计划 |
| `GeneratePhysicalPlan()` | 生成物理执行计划 | 从逻辑计划生成物理计划 |
| `ApplyPhysicalOptimizations()` | 应用物理优化 | 选择最佳物理操作符 |
| `ChooseBestPlan()` | 选择最优计划 | 基于成本估算选择最优计划 |

### 2. LogicalPlan

#### 功能概述
逻辑执行计划，表示查询的逻辑操作。

#### 核心节点类型

| 节点类型 | 功能 |
|----------|------|
| **LogicalSeqScan** | 逻辑顺序扫描 |
| **LogicalIndexScan** | 逻辑索引扫描 |
| **LogicalNestedLoopJoin** | 逻辑嵌套循环连接 |
| **LogicalHashJoin** | 逻辑哈希连接 |
| **LogicalSortMergeJoin** | 逻辑排序合并连接 |
| **LogicalProjection** | 逻辑投影 |
| **LogicalFilter** | 逻辑过滤 |
| **LogicalAggregation** | 逻辑聚合 |

### 3. PhysicalPlan

#### 功能概述
物理执行计划，表示查询的物理操作。

#### 核心节点类型

| 节点类型 | 功能 |
|----------|------|
| **PhysicalSeqScan** | 物理顺序扫描 |
| **PhysicalIndexScan** | 物理索引扫描 |
| **PhysicalNestedLoopJoin** | 物理嵌套循环连接 |
| **PhysicalHashJoin** | 物理哈希连接 |
| **PhysicalSortMergeJoin** | 物理排序合并连接 |
| **PhysicalProjection** | 物理投影 |
| **PhysicalFilter** | 物理过滤 |
| **PhysicalAggregation** | 物理聚合 |

### 4. CostModel

#### 功能概述
成本模型，估算执行计划的成本。

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `EstimateCost()` | 估算执行计划成本 | 计算执行计划的总成本 |
| `EstimateIOCost()` | 估算I/O成本 | 计算磁盘I/O操作的成本 |
| `EstimateCPUCost()` | 估算CPU成本 | 计算CPU处理的成本 |
| `EstimateMemoryCost()` | 估算内存成本 | 计算内存使用的成本 |

### 5. Statistics

#### 功能概述
统计信息管理器，收集和维护数据库统计信息。

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `CollectStatistics()` | 收集统计信息 | 收集表和索引的统计信息 |
| `GetTableStatistics()` | 获取表统计信息 | 返回表的统计信息 |
| `GetIndexStatistics()` | 获取索引统计信息 | 返回索引的统计信息 |
| `UpdateStatistics()` | 更新统计信息 | 更新表和索引的统计信息 |

## 🔧 实现步骤

### 1. 实现 Statistics

1. **初始化**：创建统计信息存储结构
2. **统计信息收集**：实现 `CollectStatistics()` 方法，收集表和索引的统计信息
3. **统计信息管理**：实现统计信息的获取和更新方法

### 2. 实现 CostModel

1. **成本计算**：实现 `EstimateCost()`, `EstimateIOCost()`, `EstimateCPUCost()`, `EstimateMemoryCost()` 方法
2. **成本参数**：设置合理的成本参数，如I/O操作成本、CPU操作成本等

### 3. 实现 LogicalPlan

1. **节点定义**：定义各种逻辑计划节点
2. **计划生成**：实现 `GenerateLogicalPlan()` 方法，根据抽象语法树生成逻辑计划
3. **逻辑优化**：实现 `ApplyLogicalOptimizations()` 方法，应用启发式规则优化逻辑计划

### 4. 实现 PhysicalPlan

1. **节点定义**：定义各种物理计划节点
2. **计划生成**：实现 `GeneratePhysicalPlan()` 方法，从逻辑计划生成物理计划
3. **物理优化**：实现 `ApplyPhysicalOptimizations()` 方法，选择最佳物理操作符

### 5. 实现 QueryOptimizer

1. **初始化**：获取目录管理器，初始化统计信息和成本模型
2. **优化流程**：实现 `Optimize()` 方法，执行完整的优化流程
3. **计划选择**：实现 `ChooseBestPlan()` 方法，基于成本估算选择最优计划

## 📝 代码示例

### 查询优化器示例

```cpp
// 优化查询
AbstractExecutor *QueryOptimizer::Optimize(SelectStmt *stmt) {
    // 生成逻辑执行计划
    LogicalPlan *logical_plan = GenerateLogicalPlan(stmt);
    
    // 应用逻辑优化
    logical_plan = ApplyLogicalOptimizations(logical_plan);
    
    // 生成物理执行计划
    std::vector<PhysicalPlan*> physical_plans = GeneratePhysicalPlan(logical_plan);
    
    // 估算成本并选择最优计划
    PhysicalPlan *best_plan = ChooseBestPlan(physical_plans);
    
    // 生成执行器
    AbstractExecutor *executor = GenerateExecutor(best_plan);
    
    // 清理资源
    delete logical_plan;
    for (auto plan : physical_plans) {
        if (plan != best_plan) {
            delete plan;
        }
    }
    
    return executor;
}

// 应用逻辑优化
LogicalPlan *QueryOptimizer::ApplyLogicalOptimizations(LogicalPlan *plan) {
    // 谓词下推
    plan = PushPredicatesDown(plan);
    
    // 投影消除
    plan = EliminateProjections(plan);
    
    // 常量折叠
    plan = FoldConstants(plan);
    
    return plan;
}

// 选择最优计划
PhysicalPlan *QueryOptimizer::ChooseBestPlan(std::vector<PhysicalPlan*> plans) {
    PhysicalPlan *best_plan = nullptr;
    double min_cost = std::numeric_limits<double>::max();
    
    for (auto plan : plans) {
        double cost = cost_model_->EstimateCost(plan);
        if (cost < min_cost) {
            min_cost = cost;
            best_plan = plan;
        }
    }
    
    return best_plan;
}
```

### 逻辑优化示例

```cpp
// 谓词下推
LogicalPlan *QueryOptimizer::PushPredicatesDown(LogicalPlan *plan) {
    if (auto *join = dynamic_cast<LogicalJoin*>(plan)) {
        // 分析连接条件
        Expr *join_predicate = join->predicate_;
        
        // 尝试将谓词下推到左右子计划
        join->left_ = PushPredicatesDown(join->left_);
        join->right_ = PushPredicatesDown(join->right_);
        
        // 分离连接谓词和非连接谓词
        std::pair<Expr*, Expr*> predicates = SplitPredicates(join_predicate);
        join->predicate_ = predicates.first;  // 连接谓词
        
        // 将非连接谓词下推到相应的子计划
        if (predicates.second) {
            // 分析谓词涉及的表
            std::vector<std::string> left_tables = GetTables(join->left_);
            std::vector<std::string> right_tables = GetTables(join->right_);
            
            // 将谓词下推到涉及表的子计划
            if (IsOnlyInTables(predicates.second, left_tables)) {
                join->left_ = new LogicalFilter(join->left_, predicates.second);
            } else if (IsOnlyInTables(predicates.second, right_tables)) {
                join->right_ = new LogicalFilter(join->right_, predicates.second);
            }
        }
    } else if (auto *filter = dynamic_cast<LogicalFilter*>(plan)) {
        // 递归处理子计划
        filter->child_ = PushPredicatesDown(filter->child_);
    } else if (auto *project = dynamic_cast<LogicalProjection*>(plan)) {
        // 递归处理子计划
        project->child_ = PushPredicatesDown(project->child_);
    }
    
    return plan;
}
```

## 🚀 使用方法

### 基本使用流程

1. **初始化查询优化器**：
   ```cpp
   Catalog *catalog = ...;  // 获取目录管理器
   Statistics *statistics = new Statistics(catalog);
   CostModel *cost_model = new CostModel();
   QueryOptimizer optimizer(catalog, statistics, cost_model);
   ```

2. **解析和分析SQL语句**：
   ```cpp
   Parser parser;
   SemanticAnalyzer analyzer(catalog);
   std::string sql = "SELECT * FROM student WHERE age > 20";
   ASTNode *ast = parser.Parse(sql);
   analyzer.Analyze(ast);
   ```

3. **优化查询**：
   ```cpp
   auto *select_stmt = dynamic_cast<SelectStmt*>(ast);
   AbstractExecutor *executor = optimizer.Optimize(select_stmt);
   ```

4. **执行查询**：
   ```cpp
   executor->Init();
   Tuple tuple;
   while (executor->Next(&tuple)) {
       // 处理查询结果
       tuple.Print();
   }
   ```

5. **清理资源**：
   ```cpp
   delete executor;
   delete ast;
   delete statistics;
   delete cost_model;
   ```

## ⚠️ 常见问题与解决方案

### 1. 优化器性能

**问题**：查询优化器本身性能不佳，特别是对于复杂查询
**解决方案**：
- 限制优化时间，避免过度优化
- 缓存优化结果，避免重复优化
- 使用启发式规则减少搜索空间

### 2. 统计信息过时

**问题**：统计信息过时，导致优化器做出错误的决策
**解决方案**：
- 定期收集统计信息
- 在数据发生重大变化时自动更新统计信息
- 提供手动更新统计信息的接口

### 3. 成本估算不准确

**问题**：成本估算不准确，导致选择次优执行计划
**解决方案**：
- 改进成本模型，考虑更多因素
- 调整成本参数，使其更符合实际情况
- 使用反馈机制，根据实际执行情况调整成本估算

### 4. 优化器复杂性

**问题**：优化器过于复杂，难以维护
**解决方案**：
- 模块化设计，将优化器分解为多个组件
- 文档化优化规则和成本模型
- 提供配置选项，允许用户调整优化行为

## 📊 优化策略

### 逻辑优化策略

| 策略 | 描述 | 效果 |
|------|------|------|
| **谓词下推** | 将过滤条件下推到扫描操作附近 | 减少中间结果集大小 |
| **投影消除** | 消除不必要的列投影 | 减少数据传输和处理开销 |
| **常量折叠** | 计算常量表达式的值 | 减少运行时计算开销 |
| **连接顺序优化** | 选择最优的表连接顺序 | 减少中间结果集大小 |
| **子查询优化** | 优化子查询，转换为连接操作 | 提高查询执行效率 |

### 物理优化策略

| 策略 | 描述 | 适用场景 |
|------|------|----------|
| **顺序扫描** | 顺序扫描表中的所有记录 | 小表，无索引 |
| **索引扫描** | 使用索引进行扫描 | 有索引的表，选择性查询 |
| **嵌套循环连接** | 嵌套循环连接表 | 小表连接 |
| **哈希连接** | 使用哈希表连接表 | 大表连接 |
| **排序合并连接** | 先排序后合并连接 | 有序数据连接 |

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第13章 查询优化
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 查询优化讲座
- [The Design and Implementation of Modern Column-Oriented Database Systems](https://www.amazon.com/Design-Implementation-Modern-Column-Oriented-Database/dp/1611972308) - 现代数据库系统设计

## 🌟 总结

查询优化模块是数据库系统中提高查询性能的关键组件。通过理解和实现查询优化模块，你将掌握：

- 逻辑优化和物理优化的原理
- 成本模型的设计和实现
- 统计信息的收集和管理
- 执行计划的生成和选择

这些知识对于理解数据库系统的内部工作原理至关重要，也是实现高性能数据库系统的基础。

---

**Happy Coding!** 🚀
