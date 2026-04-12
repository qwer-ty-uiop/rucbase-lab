# 语义分析模块详解

## 📋 模块概述

语义分析模块负责对SQL语句进行语义检查和类型推断，确保SQL语句的正确性。该模块在SQL解析之后执行，对抽象语法树进行分析，验证表和列的存在性、类型兼容性等，为后续的查询优化和执行做准备。

## 🎯 核心功能

- **表和列的存在性检查**：验证SQL语句中引用的表和列是否存在
- **类型兼容性检查**：确保表达式的类型匹配
- **表达式求值**：计算常量表达式的值
- **权限检查**：验证用户是否有权限执行操作
- **语义错误检测**：检测和报告语义错误

## 📁 目录结构

```
src/analyze/
├── analyzer.h/cpp          # 语义分析器
├── symbol_table.h/cpp      # 符号表
├── type.h/cpp             # 类型系统
└── README.md              # 模块文档
```

## 🏗️ 核心组件

### 1. SemanticAnalyzer

#### 功能概述
语义分析器，对抽象语法树进行语义分析。

#### 关键数据结构
```cpp
class SemanticAnalyzer {
private:
    Catalog *catalog_;  // 目录管理器，提供表和索引信息
    SymbolTable symbol_table_;  // 符号表，管理表和列的信息
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `Analyze()` | 分析抽象语法树 | 对不同类型的语句调用相应的分析方法 |
| `AnalyzeSelectStmt()` | 分析SELECT语句 | 检查表和列的存在性，验证表达式类型 |
| `AnalyzeInsertStmt()` | 分析INSERT语句 | 检查表的存在性，验证值的类型 |
| `AnalyzeUpdateStmt()` | 分析UPDATE语句 | 检查表和列的存在性，验证表达式类型 |
| `AnalyzeDeleteStmt()` | 分析DELETE语句 | 检查表的存在性，验证表达式类型 |
| `AnalyzeCreateTableStmt()` | 分析CREATE TABLE语句 | 检查列类型的有效性 |
| `AnalyzeCreateIndexStmt()` | 分析CREATE INDEX语句 | 检查表和列的存在性 |
| `AnalyzeDropTableStmt()` | 分析DROP TABLE语句 | 检查表的存在性 |

### 2. SymbolTable

#### 功能概述
符号表，管理表和列的信息，支持作用域管理。

#### 关键数据结构
```cpp
class SymbolTable {
private:
    std::unordered_map<std::string, TableInfo*> tables_;  // 表信息映射
    std::unordered_map<std::string, ColumnInfo> columns_;  // 列信息映射
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `AddTable()` | 添加表到符号表 | 将表信息添加到映射中 |
| `GetTable()` | 获取表信息 | 根据表名获取表信息 |
| `HasTable()` | 检查表是否存在 | 检查表名是否在映射中 |
| `AddColumn()` | 添加列到符号表 | 将列信息添加到映射中 |
| `GetColumn()` | 获取列信息 | 根据列名获取列信息 |
| `HasColumn()` | 检查列是否存在 | 检查列名是否在映射中 |
| `EnterScope()` | 进入作用域 | 保存当前作用域状态 |
| `ExitScope()` | 退出作用域 | 恢复之前的作用域状态 |

### 3. Type System

#### 功能概述
类型系统，处理数据类型的表示和转换。

#### 支持的数据类型

| 类型 | 大小 | 描述 |
|------|------|------|
| **INTEGER** | 4字节 | 整数 |
| **BIGINT** | 8字节 | 长整数 |
| **FLOAT** | 4字节 | 浮点数 |
| **DOUBLE** | 8字节 | 双精度浮点数 |
| **VARCHAR(n)** | 变长 | 字符串 |
| **DATETIME** | 8字节 | 日期时间 |
| **BOOLEAN** | 1字节 | 布尔值 |

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `IsCompatible()` | 检查类型兼容性 | 检查两个类型是否兼容 |
| `Convert()` | 类型转换 | 将一个类型的值转换为另一个类型 |
| `GetTypeSize()` | 获取类型大小 | 返回类型的字节大小 |
| `GetTypeName()` | 获取类型名称 | 返回类型的名称 |

## 🔧 实现步骤

### 1. 实现 Type System

1. **定义类型**：定义支持的数据类型
2. **实现类型操作**：实现类型兼容性检查、转换等方法
3. **类型大小计算**：计算每种类型的字节大小

### 2. 实现 SymbolTable

1. **初始化**：创建表和列的映射
2. **作用域管理**：实现作用域的进入和退出
3. **符号管理**：实现表和列的添加、获取和检查

### 3. 实现 SemanticAnalyzer

1. **初始化**：获取目录管理器，初始化符号表
2. **语句分析**：实现对不同类型语句的分析方法
3. **表达式分析**：实现对表达式的类型推断和检查
4. **错误处理**：检测和报告语义错误

## 📝 代码示例

### 语义分析器示例

```cpp
// 分析SELECT语句
bool SemanticAnalyzer::AnalyzeSelectStmt(SelectStmt *stmt) {
    // 检查表是否存在
    for (const auto &table_node : *stmt->table_list_) {
        auto *table_ref = dynamic_cast<TableRef*>(table_node);
        if (!table_ref) {
            throw SemanticError("Invalid table reference");
        }
        
        const std::string &table_name = table_ref->table_name_;
        if (!catalog_->HasTable(table_name)) {
            throw SemanticError("Table not found: " + table_name);
        }
        
        // 添加表到符号表
        TableInfo *table_info = catalog_->GetTable(table_name);
        symbol_table_.AddTable(table_name, table_info);
        
        // 添加列到符号表
        for (const auto &column : table_info->table_meta_.schema_.columns_) {
            symbol_table_.AddColumn(column.name_, column);
        }
    }
    
    // 检查选择列表
    for (const auto &select_node : *stmt->select_list_) {
        if (auto *star_expr = dynamic_cast<StarExpr*>(select_node)) {
            // 处理通配符
            continue;
        } else if (auto *column_ref = dynamic_cast<ColumnRef*>(select_node)) {
            // 检查列是否存在
            if (!symbol_table_.HasColumn(column_ref->column_name_)) {
                throw SemanticError("Column not found: " + column_ref->column_name_);
            }
        } else if (auto *expr = dynamic_cast<Expr*>(select_node)) {
            // 分析表达式
            AnalyzeExpression(expr);
        }
    }
    
    // 检查WHERE子句
    if (stmt->where_clause_) {
        AnalyzeExpression(stmt->where_clause_);
    }
    
    return true;
}

// 分析表达式
ValueType SemanticAnalyzer::AnalyzeExpression(Expr *expr) {
    if (auto *binary_expr = dynamic_cast<BinaryExpr*>(expr)) {
        // 分析左右操作数
        ValueType left_type = AnalyzeExpression(binary_expr->left_);
        ValueType right_type = AnalyzeExpression(binary_expr->right_);
        
        // 检查类型兼容性
        if (!IsCompatible(left_type, right_type)) {
            throw SemanticError("Type mismatch in expression");
        }
        
        // 返回表达式类型
        return GetResultType(binary_expr->op_, left_type, right_type);
    } else if (auto *column_ref = dynamic_cast<ColumnRef*>(expr)) {
        // 检查列是否存在
        if (!symbol_table_.HasColumn(column_ref->column_name_)) {
            throw SemanticError("Column not found: " + column_ref->column_name_);
        }
        
        // 返回列类型
        return symbol_table_.GetColumn(column_ref->column_name_).type_;
    } else if (auto *constant = dynamic_cast<Constant*>(expr)) {
        // 返回常量类型
        return constant->type_;
    }
    
    throw SemanticError("Invalid expression");
}
```

### 符号表示例

```cpp
// 添加表到符号表
void SymbolTable::AddTable(const std::string &table_name, TableInfo *table_info) {
    tables_[table_name] = table_info;
    
    // 添加表的列
    for (const auto &column : table_info->table_meta_.schema_.columns_) {
        columns_[column.name_] = column;
    }
}

// 检查表是否存在
bool SymbolTable::HasTable(const std::string &table_name) {
    return tables_.find(table_name) != tables_.end();
}

// 检查列是否存在
bool SymbolTable::HasColumn(const std::string &column_name) {
    return columns_.find(column_name) != columns_.end();
}
```

## 🚀 使用方法

### 基本使用流程

1. **初始化语义分析器**：
   ```cpp
   Catalog *catalog = ...;  // 获取目录管理器
   SemanticAnalyzer analyzer(catalog);
   ```

2. **解析SQL语句**：
   ```cpp
   Parser parser;
   std::string sql = "SELECT * FROM student WHERE age > 20";
   ASTNode *ast = parser.Parse(sql);
   ```

3. **进行语义分析**：
   ```cpp
   analyzer.Analyze(ast);
   ```

4. **处理分析结果**：
   ```cpp
   // 语义分析成功，可以进行查询优化和执行
   ```

5. **清理资源**：
   ```cpp
   delete ast;
   ```

## ⚠️ 常见问题与解决方案

### 1. 语义错误处理

**问题**：语义错误没有提供清晰的错误信息
**解决方案**：
- 实现详细的错误消息，包含错误位置和原因
- 支持错误恢复，尽可能继续分析
- 提供错误代码，便于客户端处理

### 2. 类型系统复杂性

**问题**：类型系统过于复杂，难以维护
**解决方案**：
- 简化类型系统，只支持必要的类型
- 实现类型转换规则，处理类型兼容性
- 使用枚举或类层次结构表示类型

### 3. 符号表管理

**问题**：符号表管理不当，导致作用域混乱
**解决方案**：
- 实现清晰的作用域管理
- 支持表别名和列别名
- 处理名称冲突和解析

### 4. 性能优化

**问题**：语义分析性能不佳，特别是对于复杂查询
**解决方案**：
- 缓存分析结果，避免重复分析
- 优化符号表查找，使用高效的数据结构
- 并行分析独立的表达式

## 📊 语义分析流程

1. **初始化**：获取目录管理器，初始化符号表
2. **表分析**：检查SQL语句中引用的表是否存在
3. **列分析**：检查SQL语句中引用的列是否存在
4. **类型分析**：检查表达式的类型兼容性
5. **权限分析**：检查用户是否有权限执行操作
6. **错误处理**：检测和报告语义错误
7. **结果生成**：生成语义分析结果，供后续模块使用

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第12章 查询处理
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 语义分析讲座
- [Compilers: Principles, Techniques, and Tools](https://www.amazon.com/Compilers-Principles-Techniques-Tools-2nd/dp/0321486811) - 语义分析相关章节

## 🌟 总结

语义分析模块是数据库系统中确保SQL语句正确性的重要组件。通过理解和实现语义分析模块，你将掌握：

- 语义分析的原理和实现
- 类型系统的设计和管理
- 符号表的实现和作用域管理
- 错误处理和恢复机制

这些知识对于理解编译器和解释器的工作原理至关重要，也是实现高可靠性数据库系统的基础。

---

**Happy Coding!** 🚀
