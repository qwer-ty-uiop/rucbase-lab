/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sm_manager.h"

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include "index/ix.h"
#include "record/rm.h"
#include "record_printer.h"

/**
 * @description: 判断是否为一个文件夹
 * @return {bool} 返回是否为一个文件夹
 * @param {string&} db_name 数据库文件名称，与文件夹同名
 */
bool SmManager::is_dir(const std::string& db_name) {
    struct stat st;
    return stat(db_name.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

/**
 * @description: 创建数据库，所有的数据库相关文件都放在数据库同名文件夹下
 * @param {string&} db_name 数据库名称
 */
void SmManager::create_db(const std::string& db_name) {
    if (is_dir(db_name)) {
        throw DatabaseExistsError(db_name);
    }
    // 为数据库创建一个子目录
    std::string cmd = "mkdir " + db_name;
    if (system(cmd.c_str()) < 0) {  // 创建一个名为db_name的目录
        throw UnixError();
    }
    if (chdir(db_name.c_str()) < 0) {  // 进入名为db_name的目录
        throw UnixError();
    }
    // 创建系统目录
    DbMeta* new_db = new DbMeta();
    new_db->name_ = db_name;

    // 注意，此处ofstream会在当前目录创建(如果没有此文件先创建)和打开一个名为DB_META_NAME的文件
    std::ofstream ofs(DB_META_NAME);

    // 将new_db中的信息，按照定义好的operator<<操作符，写入到ofs打开的DB_META_NAME文件中
    ofs << *new_db;  // 注意：此处重载了操作符<<

    delete new_db;

    // 创建日志文件
    disk_manager_->create_file(LOG_FILE_NAME);

    // 回到根目录
    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 删除数据库，同时需要清空相关文件以及数据库同名文件夹
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::drop_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    std::string cmd = "rm -r " + db_name;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
    flush_meta();
}

/**
 * @description:
 * 打开数据库，找到数据库对应的文件夹，并加载数据库元数据和相关文件
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::open_db(const std::string& db_name) {
    // Check if directory exists and change to it
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }

    if (chdir(db_name.c_str()) < 0) {
        throw UnixError();  // Handle failure to change directory
    }

    // Load database metadata from a file
    std::ifstream ifs(DB_META_NAME);
    if (!ifs) {
        throw std::runtime_error("Failed to open database metadata file.");
    }
    ifs >> db_;

    // Open file handlers and index handlers for each table
    for (auto& entry : db_.tabs_) {
        auto& tab = entry.second;
        fhs_.emplace(tab.name, rm_manager_->open_file(tab.name));  // Open and store file handler

        for (auto& index : tab.indexes) {
            std::string indexName = ix_manager_->get_index_name(tab.name, index.cols);
            ihs_.emplace(indexName, ix_manager_->open_index(tab.name, index.cols));  // Open and store index handler
        }

        // Reset indexes for each table, seems redundant and could be a logical flaw
        for (auto& index : tab.indexes) {
            drop_index(tab.name, index.cols, nullptr);  // Dropping index right after opening might not be intentional
        }
    }
}

/**
 * @description: 把数据库相关的元数据刷入磁盘中
 */
void SmManager::flush_meta() {
    // 默认清空文件
    std::ofstream ofs(DB_META_NAME);
    ofs << db_;
}

/**
 * @description: 关闭数据库并把数据落盘
 */
void SmManager::close_db() {
    std::ofstream ofs(DB_META_NAME);
    if (!ofs) {
        throw std::runtime_error("Failed to open database metadata file for writing.");
    }
    ofs << db_;
    // Clear database name and tables data
    db_.name_.clear();
    db_.tabs_.clear();
    for (auto& entry : fhs_)
        rm_manager_->close_file(entry.second.get());
    for (auto& entry : ihs_)
        ix_manager_->close_index(entry.second.get());
    fhs_.clear();
    ihs_.clear();
    if (chdir("..") < 0)
        throw UnixError();
    flush_meta();
}

/**
 * @description:
 * 显示所有的表,通过测试需要将其结果写入到output.txt,详情看题目文档
 * @param {Context*} context
 */
void SmManager::show_tables(Context* context) {
    std::fstream outfile;
    outfile.open("output.txt", std::ios::out | std::ios::app);
    outfile << "| Tables |\n";
    RecordPrinter printer(1);
    printer.print_separator(context);
    printer.print_record({"Tables"}, context);
    printer.print_separator(context);
    for (auto& entry : db_.tabs_) {
        auto& tab = entry.second;
        printer.print_record({tab.name}, context);
        outfile << "| " << tab.name << " |\n";
    }
    printer.print_separator(context);
    outfile.close();
}

/**
 * @description: 显示表的元数据
 * @param {string&} tab_name 表名称
 * @param {Context*} context
 */
void SmManager::desc_table(const std::string& tab_name, Context* context) {
    TabMeta& tab = db_.get_table(tab_name);

    std::vector<std::string> captions = {"Field", "Type", "Index"};
    RecordPrinter printer(captions.size());
    // Print header
    printer.print_separator(context);
    printer.print_record(captions, context);
    printer.print_separator(context);
    // Print fields
    for (auto& col : tab.cols) {
        std::vector<std::string> field_info = {col.name, coltype2str(col.type),
                                               col.index ? "YES" : "NO"};
        printer.print_record(field_info, context);
    }
    // Print footer
    printer.print_separator(context);
}

/**
 * @description: 创建表
 * @param {string&} tab_name 表的名称
 * @param {vector<ColDef>&} col_defs 表的字段
 * @param {Context*} context
 */
void SmManager::create_table(const std::string& tab_name,
                             const std::vector<ColDef>& col_defs,
                             Context* context) {
    if (db_.is_table(tab_name)) {
        std::fstream outfile;
        throw TableExistsError(tab_name);
    }
    // Create table meta
    int curr_offset = 0;
    TabMeta tab;
    tab.name = tab_name;
    for (auto& col_def : col_defs) {
        ColMeta col = {.tab_name = tab_name,
                       .name = col_def.name,
                       .type = col_def.type,
                       .len = col_def.len,
                       .offset = curr_offset,
                       .index = false};
        curr_offset += col_def.len;
        tab.cols.push_back(col);
    }
    // Create & open record file
    int record_size =
        curr_offset;  // record_size就是col
                      // meta所占的大小（表的元数据也是以记录的形式进行存储的）
    rm_manager_->create_file(tab_name, record_size);
    db_.tabs_[tab_name] = tab;
    // fhs_[tab_name] = rm_manager_->open_file(tab_name);
    fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));

    flush_meta();
}

/**
 * @description: 删除表
 * @param {string&} tab_name 表的名称
 * @param {Context*} context
 */
void SmManager::drop_table(const std::string& tab_name, Context* context) {
    // First, check if the table exists before attempting any operations
    if (!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }

    // Since table exists, get its metadata
    TabMeta& tab = db_.get_table(tab_name);

    // Close the file associated with the table before deleting it
    auto fileHandlerIt = fhs_.find(tab_name);
    if (fileHandlerIt != fhs_.end()) {
        rm_manager_->close_file(fileHandlerIt->second.get());
        rm_manager_->destroy_file(tab_name); // Delete the record file
        fhs_.erase(fileHandlerIt); // Remove file handler from the map after closure and deletion
    } else {
        throw std::runtime_error("File handler for table " + tab_name + " not found.");
    }

    // Drop all indexes associated with the table
    for (const auto& index : tab.indexes) {
        drop_index(tab_name, index.cols, context);
    }
    tab.indexes.clear(); // Clear index metadata after successfully dropping indexes

    // Delete the table metadata from the database
    db_.tabs_.erase(tab_name);
    // Ensure all metadata changes are flushed to persistent storage
    flush_meta();
}


/**
 * @description: 创建索引
 * @param {string&} tab_name 表的名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::create_index(const std::string& tab_name,
                             const std::vector<std::string>& col_names,
                             Context* context) {
    TabMeta& tab = db_.get_table(tab_name);

    // Check if the index already exists
    if (ix_manager_->exists(tab_name, col_names)) {
        throw IndexExistsError(tab_name, col_names);
    }

    // Prepare column metadata for index creation
    std::vector<ColMeta> cols;
    for (const auto& col_name : col_names) {
        cols.push_back(*tab.get_col(col_name));
    }

    // Create the index
    ix_manager_->create_index(tab_name, cols);

    // Open the newly created index file
    auto ih = ix_manager_->open_index(tab_name, cols);

    // Calculate total length of index columns
    int col_total_length = 0;
    for (const auto& col : cols) {
        col_total_length += col.len;
    }

    // Insert all existing records into the new index
    auto file_handle = fhs_.at(tab_name).get();
    std::vector<char> key(col_total_length);  // Use vector to handle variable length safely
    for (RmScan scan(file_handle); !scan.is_end(); scan.next()) {
        auto record = file_handle->get_record(scan.rid(), context);
        int offset = 0;
        for (size_t i = 0; i < cols.size(); ++i) {
            std::memcpy(&key[offset], record.get()->data + cols[i].offset, cols[i].len);
            offset += cols[i].len;
        }
        ih->insert_entry(key.data(), scan.rid(), context->txn_);
    }

    // Update table metadata with new index information
    tab.indexes.push_back(IndexMeta{tab_name, col_total_length, static_cast<int>(cols.size()), cols});

    // Store index handler in manager
    ihs_.emplace(ix_manager_->get_index_name(tab_name, col_names), std::move(ih));

    // Persist metadata changes to disk
    flush_meta();
}


/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name,
                           const std::vector<std::string>& col_names,
                           Context* context) {
    // Generate the index name
    std::string index_name = ix_manager_->get_index_name(tab_name, col_names);

    // Check if the index exists before proceeding
    if (!ix_manager_->exists(tab_name, col_names)) {
        throw IndexNotFoundError(tab_name, col_names);
    }

    // Safely close the index if it is currently managed
    auto indexHandleIter = ihs_.find(index_name);
    if (indexHandleIter != ihs_.end()) {
        ix_manager_->close_index(indexHandleIter->second.get());
        ihs_.erase(indexHandleIter);
    } else {
        throw std::runtime_error("Failed to find index handle for " + index_name);
    }

    // Destroy the index on disk
    ix_manager_->destroy_index(tab_name, col_names);

    // Update the table metadata to reflect the removal of the index
    TabMeta &tab = db_.get_table( tab_name );
    tab.indexes.erase(tab.get_index_meta(col_names));

    // Persist all changes to disk
    flush_meta();
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<ColMeta>&} 索引包含的字段元数据
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name,
                           const std::vector<ColMeta>& cols,
                           Context* context) {
    std::vector<std::string> col_names;
    for (auto& col : cols) {
        col_names.push_back(col.name);
    }
    drop_index(tab_name, col_names, context);
}

/**
 * @description: 显示索引
 * @param {string&} tab_name 表名称
 * @param {Context*} context
 */
void SmManager::show_index(const std::string& tab_name, Context* context) {
    // Retrieve table metadata
    TabMeta &tab = db_.get_table(tab_name);

    // Open the output file with proper resource management
    std::ofstream outfile("output.txt", std::ios::out | std::ios::app);
    if (!outfile) {
        throw std::runtime_error("Failed to open output file.");
    }

    // Initialize the printer and print header and separator
    RecordPrinter printer(1);
    printer.print_separator(context);
    printer.print_record({"index"}, context);
    printer.print_separator(context);

    // Iterate over each index in the table and print details
    for (const auto& index : tab.indexes) {
        // Construct the index information string
       outfile << "| " << tab_name << " | unique | ("<< index.cols[0].name;
        for(size_t i = 1; i < index.cols.size(); ++i) 
            outfile << "," << index.cols[i].name;
        outfile << ") |\n";

        // Print index name using the RecordPrinter
        printer.print_record({ix_manager_->get_index_name(tab_name, index.cols)}, context);
    }

    // Print the final separator
    printer.print_separator(context);

    // Automatically close the output file by destructor when going out of scope
    outfile.close();
}



/**
 * @description: 回滚插入记录
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {Context*} context
 */
void SmManager::rollback_insert(const std::string& tab_name, const Rid& rid, Context* context) {
    // Ensure the table exists and retrieve metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta& tab = db_.get_table(tab_name);

    // Ensure file handle exists for the table
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Get the record, throw if record cannot be retrieved
    auto rec = file_handle->get_record(rid, context);
    if (!rec) {
        throw std::runtime_error("Failed to retrieve record for deletion.");
    }

    // Delete associated indexes
    for (auto& index : tab.indexes) {
        std::vector<char> key(index.col_tot_len);  // Use vector to manage variable-length key storage
        index.get_key(rec.get(), key.data());  // Assuming get_key modifies the vector

        auto indexHandleIt = ihs_.find(ix_manager_->get_index_name(tab_name, index.cols));
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found for index: " + tab_name);
        }
        indexHandleIt->second->delete_entry(key.data(), context->txn_);
    }

    // Delete the record from the file
    file_handle->delete_record(rid, context);
}


/**
 * @description: 回滚删除记录
 * @param {string&} tab_name 表名称
 * @param {RmRecord} rec 记录的数据
 * @param {Context*} context
 */
void SmManager::rollback_delete(const std::string& tab_name, const RmRecord& rec, const Rid& rid, Context* context) {
    // Check if the table exists in the database and retrieve metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta &tab = db_.get_table(tab_name);

    // Check if the file handle exists before proceeding
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Insert the record into the file
    file_handle->insert_record(rid, rec.data);

    // Reinsert the index entries for the record
    for (auto& index : tab.indexes) {
        // Create a buffer for the index key based on the size needed
        std::vector<char> key(index.col_tot_len);
        index.get_key(&rec, key.data());

        // Retrieve the index handle and insert the key
        std::string indexName = ix_manager_->get_index_name(tab_name, index.cols);
        auto indexHandleIt = ihs_.find(indexName);
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found for index: " + indexName);
        }
        indexHandleIt->second->insert_entry(key.data(), rid, context->txn_);
    }
}


/**
 * @description: 回滚更新记录
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {RmRecord} rec 记录的数据
 * @param {Context*} context
 */
void SmManager::rollback_update(const std::string& tab_name, const Rid& rid, const RmRecord& rec, Context* context) {
    // Ensure the table exists and retrieve its metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta& tab = db_.get_table(tab_name);

    // Retrieve the file handle and check its existence
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Retrieve the current record from the database
    auto old_rec = file_handle->get_record(rid, context);
    if (!old_rec) {
        throw std::runtime_error("Failed to retrieve the original record for RID.");
    }

    // Update the indexes associated with this table
    for (auto& index : tab.indexes) {
        std::vector<char> old_key(index.col_tot_len);
        std::vector<char> new_key(index.col_tot_len);

        index.get_key(old_rec.get(), old_key.data());
        index.get_key(&rec, new_key.data());

        // Retrieve index handle and update entries
        std::string indexName = ix_manager_->get_index_name(tab_name, index.cols);
        auto indexHandleIt = ihs_.find(indexName);
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found for: " + indexName);
        }

        indexHandleIt->second->delete_entry(old_key.data(), context->txn_);
        indexHandleIt->second->insert_entry(new_key.data(), rid, context->txn_);
    }

    // Update the record in the file
    file_handle->update_record(rid, rec.data, context);
}


/**
 * @description: 插入记录（故障恢复redo）
 * @param {string&} tab_name 表名称
 * @param {std::shared_ptr<Transaction>} trans 当前事务
 */
void SmManager::recovery_insert(const std::string& tab_name, const Rid& rid, const RmRecord& rec, std::shared_ptr<Transaction> trans) {
    // Check if the table exists and retrieve metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta& tab = db_.get_table(tab_name);

    // Retrieve the file handle and ensure it exists
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Insert the record into the file
    file_handle->insert_record(rid, rec.data);

    // Insert entries into indexes
    for (auto& index : tab.indexes) {
        // Use a vector to manage variable-length key storage safely
        std::vector<char> key(index.col_tot_len);
        index.get_key(&rec, key.data());

        // Retrieve the index handle and insert the key
        auto indexName = ix_manager_->get_index_name(tab_name, index.cols);
        auto indexHandleIt = ihs_.find(indexName);
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found for: " + indexName);
        }

        indexHandleIt->second->insert_entry(key.data(), rid, trans);
    }

    // Record the transaction
    WriteRecord writeRecord(WType::INSERT_TUPLE, tab_name, rid);
    trans->append_write_record(writeRecord);
}


/**
 * @description: 删除记录（故障恢复redo）
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {Context*} context
 */
void SmManager::recovery_delete(const std::string& tab_name, const Rid& rid, std::shared_ptr<Transaction> trans) {
    // Validate the existence of the table and retrieve its metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta &tab = db_.get_table(tab_name);

    // Ensure the file handle exists
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Attempt to retrieve the record to be deleted
    auto rec = file_handle->get_record(rid, nullptr);
    if (!rec) {
        throw std::runtime_error("Failed to retrieve record for deletion.");
    }

    // Delete entries from indexes
    for (auto& index : tab.indexes) {
        // Manage key memory using a vector for safety
        std::vector<char> key(index.col_tot_len);
        index.get_key(rec.get(), key.data());

        // Ensure index handle exists
        std::string indexName = ix_manager_->get_index_name(tab_name, index.cols);
        auto indexHandleIt = ihs_.find(indexName);
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found for: " + indexName);
        }

        indexHandleIt->second->delete_entry(key.data(), trans);
    }

    // Proceed to delete the record
    file_handle->delete_record(rid, nullptr);

    // Log the transaction for the deleted record
    WriteRecord writeRecord(WType::DELETE_TUPLE, tab_name, rid, *rec);
    trans->append_write_record(writeRecord);
}


/**
 * @description: 更新记录（故障恢复redo）
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {RmRecord} rec 记录的数据
 * @param {Context*} context
 */
void SmManager::recovery_update(const std::string& tab_name, const Rid& rid, const RmRecord& rec, std::shared_ptr<Transaction> trans) {
    // Ensure the table exists and retrieve metadata
    if (!db_.is_table(tab_name)) {
        throw std::runtime_error("Table not found: " + tab_name);
    }
    TabMeta& tab = db_.get_table(tab_name);

    // Retrieve the file handle and ensure it exists
    auto fileHandleIt = fhs_.find(tab_name);
    if (fileHandleIt == fhs_.end()) {
        throw std::runtime_error("File handle not found for table: " + tab_name);
    }
    auto& file_handle = fileHandleIt->second;

    // Get the old record, and handle possible errors
    auto old_rec = file_handle->get_record(rid, nullptr);
    if (!old_rec) {
        throw std::runtime_error("Failed to retrieve old record for update recovery.");
    }

    // Update indexes
    for (auto& index : tab.indexes) {
        // Use vector to handle variable-length keys safely
        std::vector<char> old_key(index.col_tot_len);
        std::vector<char> new_key(index.col_tot_len);
        
        index.get_key(old_rec.get(), old_key.data());
        index.get_key(&rec, new_key.data());

        // Manage index operations with appropriate error checks
        std::string indexName = ix_manager_->get_index_name(tab_name, index.cols);
        auto indexHandleIt = ihs_.find(indexName);
        if (indexHandleIt == ihs_.end()) {
            throw std::runtime_error("Index handle not found: " + indexName);
        }

        indexHandleIt->second->delete_entry(old_key.data(), trans);
        indexHandleIt->second->insert_entry(new_key.data(), rid, trans);
    }

    // Update the record in the database
    file_handle->update_record(rid, rec.data, nullptr);

    // Log the transaction
    WriteRecord writeRecord(WType::UPDATE_TUPLE, tab_name, rid, *old_rec);
    trans->append_write_record(writeRecord);
}


/**
 * @description: 回滚插入记录（故障恢复undo）
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {Context*} context
 */
void SmManager::rollback_insert(const std::string& tab_name, const Rid& rid, std::shared_ptr<Transaction> trans) {
    TabMeta &table = db_.get_table(tab_name);
    auto& Filehandle = fhs_.at(tab_name);
    auto rec = Filehandle->get_record(rid, nullptr);
    // 删除索引
    for(auto& index : table.indexes){
        char key[index.col_tot_len];
        index.get_key(rec.get(),key);
        ihs_.at(ix_manager_->get_index_name(tab_name, index.cols))->delete_entry(key, trans);
    }
    // 删除记录
    Filehandle->delete_record(rid, nullptr);
}

/**
 * @description: 回滚删除记录（故障恢复undo）
 * @param {string&} tab_name 表名称
 * @param {RmRecord} rec 记录的数据
 * @param {Context*} context
 */
void SmManager::rollback_delete(const std::string& tab_name, const RmRecord& rec, const Rid& rid, std::shared_ptr<Transaction> trans) {
    TabMeta &table = db_.get_table(tab_name);
    auto& Filehandle = fhs_.at(tab_name);
    // 插入记录
    Filehandle->insert_record(rid, rec.data);
    // 插入索引
    for(auto& index : table.indexes){
        char key[index.col_tot_len];
        index.get_key(&rec,key);
        ihs_.at(ix_manager_->get_index_name(tab_name, index.cols))->insert_entry(key, rid, trans);
    }
}

/**
 * @description: 回滚更新记录（故障恢复undo）
 * @param {string&} tab_name 表名称
 * @param {Rid} rid 记录的位置
 * @param {RmRecord} rec 记录的数据
 * @param {Context*} context
 */
void SmManager::rollback_update(const std::string& tab_name, const Rid& rid, const RmRecord& rec, std::shared_ptr<Transaction> trans) {
    TabMeta &table = db_.get_table(tab_name);
    auto& Filehandle = fhs_.at(tab_name);
    auto old_rec = Filehandle->get_record(rid, nullptr);

    // 回滚索引
    for(auto& index : table.indexes){
        char old_key[index.col_tot_len];
        char new_key[index.col_tot_len];
        index.get_key(old_rec.get(),old_key);
        index.get_key(&rec,new_key);
        if (memcmp(old_key, new_key, index.col_tot_len) != 0)
        {
            auto &ih = ihs_.at(ix_manager_->get_index_name(tab_name, index.cols));
            ih->delete_entry(old_key, trans);
            ih->insert_entry(new_key, rid, trans);
        }
    }

    // 回滚记录
    Filehandle->update_record(rid, rec.data, nullptr);
}