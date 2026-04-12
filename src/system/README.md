# 系统管理模块详解

## 📋 模块概述

系统管理模块负责管理数据库、表、索引等元数据，是数据库系统的核心管理组件。该模块实现了元数据的存储、管理和访问，为其他模块提供元数据服务，是数据库系统正常运行的基础。

## 🎯 核心功能

- **数据库管理**：创建、删除、切换数据库
- **表管理**：创建、删除、修改表结构
- **索引管理**：创建、删除索引
- **元数据存储**：持久化存储元数据信息
- **元数据访问**：提供元数据的查询和修改接口
- **用户管理**：用户认证和权限管理

## 📁 目录结构

```
src/system/
├── sm_manager.h/cpp          # 系统管理器
├── catalog.h/cpp             # 目录管理器
├── db_meta.h/cpp             # 数据库元数据
├── tab_meta.h/cpp            # 表元数据
├── col_meta.h/cpp            # 列元数据
└── README.md                # 模块文档
```

## 🏗️ 核心组件

### 1. SmManager

#### 功能概述
系统管理器，负责整个系统的管理，是系统管理模块的核心类。

#### 关键数据结构
```cpp
class SmManager {
private:
    Catalog *catalog_;  // 目录管理器
    DiskManager *disk_manager_;  // 磁盘管理器
    BufferPoolManager *buffer_pool_manager_;  // 缓冲池管理器
    std::mutex latch_;  // 互斥锁，保证并发安全
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `create_db()` | 创建数据库 | 创建数据库目录和元数据文件 |
| `drop_db()` | 删除数据库 | 删除数据库目录和元数据文件 |
| `use_db()` | 使用数据库 | 切换当前数据库 |
| `create_table()` | 创建表 | 创建表文件和元数据 |
| `drop_table()` | 删除表 | 删除表文件和元数据 |
| `create_index()` | 创建索引 | 创建索引文件和元数据 |
| `drop_index()` | 删除索引 | 删除索引文件和元数据 |
| `get_table()` | 获取表信息 | 返回表的元数据信息 |
| `get_index()` | 获取索引信息 | 返回索引的元数据信息 |

### 2. Catalog

#### 功能概述
目录管理器，管理数据库的元数据，提供元数据的访问接口。

#### 关键数据结构
```cpp
class Catalog {
private:
    std::unordered_map<std::string, TableInfo*> tables_;  // 表信息映射
    std::unordered_map<std::string, IndexInfo*> indexes_;  // 索引信息映射
    BufferPoolManager *buffer_pool_manager_;  // 缓冲池管理器
};
```

#### 核心方法

| 方法 | 功能 | 实现要点 |
|------|------|----------|
| `AddTable()` | 添加表 | 将表信息添加到映射中 |
| `GetTable()` | 获取表 | 根据表名获取表信息 |
| `HasTable()` | 检查表是否存在 | 检查表名是否在映射中 |
| `DropTable()` | 删除表 | 从映射中删除表信息 |
| `AddIndex()` | 添加索引 | 将索引信息添加到映射中 |
| `GetIndex()` | 获取索引 | 根据索引名获取索引信息 |
| `HasIndex()` | 检查索引是否存在 | 检查索引名是否在映射中 |
| `DropIndex()` | 删除索引 | 从映射中删除索引信息 |
| `Save()` | 保存元数据 | 将元数据持久化到磁盘 |
| `Load()` | 加载元数据 | 从磁盘加载元数据 |

### 3. DbMeta

#### 功能概述
数据库元数据，存储数据库的基本信息。

#### 关键字段

| 字段 | 功能 |
|------|------|
| `db_name` | 数据库名称 |
| `version` | 数据库版本 |
| `create_time` | 创建时间 |
| `last_modify_time` | 最后修改时间 |

### 4. TabMeta

#### 功能概述
表元数据，存储表的基本信息。

#### 关键字段

| 字段 | 功能 |
|------|------|
| `tab_name` | 表名称 |
| `schema` | 表结构 |
| `file_name` | 表文件名称 |
| `create_time` | 创建时间 |
| `last_modify_time` | 最后修改时间 |

### 5. ColMeta

#### 功能概述
列元数据，存储列的基本信息。

#### 关键字段

| 字段 | 功能 |
|------|------|
| `col_name` | 列名称 |
| `type` | 列类型 |
| `len` | 列长度 |
| `offset` | 列在记录中的偏移量 |
| `is_primary` | 是否为主键 |
| `is_not_null` | 是否非空 |

## 🔧 实现步骤

### 1. 实现 ColMeta, TabMeta, DbMeta

1. **定义结构**：定义列、表、数据库的元数据结构
2. **序列化/反序列化**：实现元数据的序列化和反序列化方法
3. **字段管理**：实现字段的访问和修改方法

### 2. 实现 Catalog

1. **初始化**：创建表和索引的映射
2. **元数据管理**：实现表和索引的添加、获取、检查和删除方法
3. **持久化**：实现元数据的保存和加载方法

### 3. 实现 SmManager

1. **初始化**：初始化磁盘管理器、缓冲池管理器和目录管理器
2. **数据库管理**：实现数据库的创建、删除和切换方法
3. **表管理**：实现表的创建、删除和修改方法
4. **索引管理**：实现索引的创建和删除方法
5. **并发控制**：为所有操作添加互斥锁保护

## 📝 代码示例

### 系统管理器示例

```cpp
// 创建表
void SmManager::create_table(const std::string &table_name, const Schema &schema) {
    std::lock_guard<std::mutex> lock(latch_);
    
    // 检查表是否存在
    if (catalog_->HasTable(table_name)) {
        throw TableExistsError(table_name);
    }
    
    // 创建表文件
    std::string table_file = table_name + ".table";
    disk_manager_->create_file(table_file.c_str());
    
    // 创建记录管理器
    RmFileHandle *file_handle = new RmFileHandle(disk_manager_, buffer_pool_manager_, table_file.c_str());
    
    // 添加表到目录
    catalog_->AddTable(table_name, file_handle, schema);
    
    // 持久化元数据
    catalog_->Save();
}

// 创建索引
void SmManager::create_index(const std::string &table_name, const std::vector<std::string> &col_names) {
    std::lock_guard<std::mutex> lock(latch_);
    
    // 检查表是否存在
    if (!catalog_->HasTable(table_name)) {
        throw TableNotFoundError(table_name);
    }
    
    // 检查列是否存在
    TableInfo *table_info = catalog_->GetTable(table_name);
    for (const auto &col_name : col_names) {
        if (!table_info->schema_.HasColumn(col_name)) {
            throw ColumnNotFoundError(col_name);
        }
    }
    
    // 创建索引
    std::string index_name = table_name + "_" + col_names[0] + "_idx";
    IxManager ix_manager(disk_manager_, buffer_pool_manager_);
    ix_manager.create_index(index_name.c_str(), table_info->schema_, col_names);
    
    // 添加索引到目录
    catalog_->AddIndex(table_name, index_name, col_names);
    
    // 持久化元数据
    catalog_->Save();
}
```

### 目录管理器示例

```cpp
// 添加表
void Catalog::AddTable(const std::string &table_name, RmFileHandle *file_handle, const Schema &schema) {
    // 创建表信息
    TableInfo *table_info = new TableInfo(table_name, file_handle, schema);
    
    // 添加到映射
    tables_[table_name] = table_info;
    
    // 持久化元数据
    Save();
}

// 保存元数据
void Catalog::Save() {
    // 创建元数据文件
    std::string meta_file = "catalog.meta";
    FILE *fp = fopen(meta_file.c_str(), "w");
    if (!fp) {
        throw InternalError("Failed to open catalog.meta");
    }
    
    // 写入表信息
    int num_tables = tables_.size();
    fwrite(&num_tables, sizeof(int), 1, fp);
    
    for (const auto &entry : tables_) {
        const std::string &table_name = entry.first;
        TableInfo *table_info = entry.second;
        
        // 写入表名
        int name_len = table_name.size();
        fwrite(&name_len, sizeof(int), 1, fp);
        fwrite(table_name.c_str(), sizeof(char), name_len, fp);
        
        // 写入表结构
        table_info->schema_.Serialize(fp);
    }
    
    // 写入索引信息
    int num_indexes = indexes_.size();
    fwrite(&num_indexes, sizeof(int), 1, fp);
    
    for (const auto &entry : indexes_) {
        const std::string &index_name = entry.first;
        IndexInfo *index_info = entry.second;
        
        // 写入索引名
        int name_len = index_name.size();
        fwrite(&name_len, sizeof(int), 1, fp);
        fwrite(index_name.c_str(), sizeof(char), name_len, fp);
        
        // 写入表名
        const std::string &table_name = index_info->table_name_;
        int table_name_len = table_name.size();
        fwrite(&table_name_len, sizeof(int), 1, fp);
        fwrite(table_name.c_str(), sizeof(char), table_name_len, fp);
        
        // 写入列名
        const std::vector<std::string> &col_names = index_info->col_names_;
        int num_cols = col_names.size();
        fwrite(&num_cols, sizeof(int), 1, fp);
        
        for (const auto &col_name : col_names) {
            int col_name_len = col_name.size();
            fwrite(&col_name_len, sizeof(int), 1, fp);
            fwrite(col_name.c_str(), sizeof(char), col_name_len, fp);
        }
    }
    
    fclose(fp);
}
```

## 🚀 使用方法

### 基本使用流程

1. **初始化系统管理器**：
   ```cpp
   SmManager *sm_manager = new SmManager();
   ```

2. **创建数据库**：
   ```cpp
   sm_manager->create_db("test_db");
   sm_manager->use_db("test_db");
   ```

3. **创建表**：
   ```cpp
   Schema schema;
   schema.AddColumn(Column("id", TypeId::INTEGER, 4, true));
   schema.AddColumn(Column("name", TypeId::VARCHAR, 20));
   schema.AddColumn(Column("age", TypeId::INTEGER, 4));
   sm_manager->create_table("student", schema);
   ```

4. **创建索引**：
   ```cpp
   sm_manager->create_index("student", {"id"});
   ```

5. **获取表信息**：
   ```cpp
   TableInfo *table_info = sm_manager->get_table("student");
   ```

6. **删除表**：
   ```cpp
   sm_manager->drop_table("student");
   ```

7. **删除数据库**：
   ```cpp
   sm_manager->drop_db("test_db");
   ```

## ⚠️ 常见问题与解决方案

### 1. 元数据一致性

**问题**：元数据与实际数据不一致
**解决方案**：
- 确保元数据的修改与实际数据的修改同步
- 实现事务，保证元数据操作的原子性
- 定期验证元数据的一致性

### 2. 并发访问

**问题**：多线程并发访问元数据导致数据不一致
**解决方案**：
- 使用互斥锁保护元数据操作
- 实现读写锁，支持并发读操作
- 合理设计锁粒度，减少锁竞争

### 3. 元数据存储

**问题**：元数据存储不当，导致数据丢失
**解决方案**：
- 定期备份元数据
- 实现元数据的冗余存储
- 使用事务保证元数据修改的持久性

### 4. 性能优化

**问题**：元数据操作性能不佳，影响系统整体性能
**解决方案**：
- 缓存常用元数据，减少磁盘I/O
- 优化元数据存储格式，提高读写效率
- 实现元数据的增量更新，减少写入开销

## 📊 系统管理流程

1. **初始化**：启动系统，加载元数据
2. **数据库操作**：创建、删除、切换数据库
3. **表操作**：创建、删除、修改表结构
4. **索引操作**：创建、删除索引
5. **元数据访问**：查询和修改元数据
6. **持久化**：将元数据变化持久化到磁盘
7. **关闭**：保存元数据，关闭系统

## 📚 学习资源

- [Database System Concepts](https://www.db-book.com/) - 第10章 存储管理
- [CMU 15-445](https://15445.courses.cs.cmu.edu/fall2022/) - 系统管理讲座
- [Designing Data-Intensive Applications](https://www.amazon.com/Designing-Data-Intensive-Applications-Reliable-Maintainable/dp/1449373321) - 数据系统设计

## 🌟 总结

系统管理模块是数据库系统的核心管理组件，负责元数据的存储、管理和访问。通过理解和实现系统管理模块，你将掌握：

- 元数据管理的原理和实现
- 数据库和表的管理方法
- 索引的管理和维护
- 并发控制和事务处理

这些知识对于理解数据库系统的内部工作原理至关重要，也是实现高可靠性数据库系统的基础。

---

**Happy Coding!** 🚀
