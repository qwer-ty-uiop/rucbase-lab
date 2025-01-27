/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "ix_index_handle.h"

#include "ix_scan.h"
#define NUMKEY page_hdr->num_key
#define COMPARE(x, y) ix_compare(x, get_key(y), file_hdr->col_types_, file_hdr->col_lens_)

/**
 * @brief 在当前node中查找第一个>=target的key_idx
 *
 * @return key_idx，范围为[0,num_key)，如果返回的key_idx=num_key，则表示target大于最后一个key
 * @note 返回key index（同时也是rid index），作为slot no
 */

int IxNodeHandle::lower_bound(const char* target) const {
    // 查找当前节点中第一个大于等于target的key，并返回key的位置给上层
    // 提示: 可以采用多种查找方式，如顺序遍历、二分查找等；使用ix_compare()函数进行比较
    int left = 0, right = NUMKEY;
    while (left < right) {
        int mid = left + (right - left) / 2;

        if (COMPARE(target, mid) > 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

/**
 * @brief 在当前node中查找第一个>target的key_idx
 *
 * @return key_idx，范围为[1,num_key)，如果返回的key_idx=num_key，则表示target大于等于最后一个key
 * @note 注意此处的范围从1开始
 */
int IxNodeHandle::upper_bound(const char* target) const {
    // 查找当前节点中第一个大于target的key，并返回key的位置给上层
    // 提示: 可以采用多种查找方式：顺序遍历、二分查找等；使用ix_compare()函数进行比较
    int left = 1, right = NUMKEY;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (COMPARE(target, mid) >= 0)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

/**
 * @brief 用于叶子结点根据key来查找该结点中的键值对
 * 值value作为传出参数，函数返回是否查找成功
 *
 * @param key 目标key
 * @param[out] value 传出参数，目标key对应的Rid
 * @return 目标key是否存在
 */
bool IxNodeHandle::leaf_lookup(const char* key, Rid** value) {
    // 提示：可以调用lower_bound()和get_rid()函数。
    // 1. 在叶子节点中获取目标key所在位置
    int index = lower_bound(key);
    // 2. 判断目标key是否存在
    if (index == NUMKEY || COMPARE(key, index))
        return false;
    // 3. 如果存在，获取key对应的Rid，并赋值给传出参数value
    *value = get_rid(index);
    return true;
}
/**
 * @brief 判断key是不是叶子结点
 * @param key 目标key
 * @return 如果是叶子节点则返回true
 */
bool IxNodeHandle::is_leaf(const char* key) {
    int index = lower_bound(key);
    if (index == NUMKEY || COMPARE(key, index))
        return false;
    return true;
}

/**
 * 用于内部结点（非叶子节点）查找目标key所在的孩子结点（子树）
 * @param key 目标key
 * @return page_id_t 目标key所在的孩子节点（子树）的存储页面编号
 */
page_id_t IxNodeHandle::internal_lookup(const char* key) {
    // 1. 查找当前非叶子节点中目标key所在孩子节点（子树）的位置
    // 2. 获取该孩子节点（子树）所在页面的编号
    // 3. 返回页面编号
    int index = upper_bound(key);
    return get_rid(index - 1)->page_no;
}

/**
 * @brief 在指定位置插入n个连续的键值对
 * 将key的前n位插入到原来keys中的pos位置；将rid的前n位插入到原来rids中的pos位置
 *
 * @param pos 要插入键值对的位置
 * @param (key, rid) 连续键值对的起始地址，也就是第一个键值对，可以通过(key, rid)来获取n个键值对
 * @param n 键值对数量
 * @note [0,pos)           [pos,num_key)
 *                            key_slot
 *                            /      \
 *                           /        \
 *       [0,pos)     [pos,pos+n)   [pos+n,num_key+n)
 *                      key           key_slot
 */
void IxNodeHandle::insert_pairs(int pos, const char* key, const Rid* rid, int n) {
    // 1. 判断 pos 的合法性
    if (pos < 0 || pos > NUMKEY)
        return;
    // 计算需要右移的键值对数量
    int move_size = NUMKEY - pos;
    // 2. 插入 key 值
    char* key_slot = get_key(pos);
    int key_length = file_hdr->col_tot_len_;
    // 右移现有的键值对
    memmove(key_slot + n * key_length, key_slot, move_size * key_length);
    // 插入新的键值对
    memcpy(key_slot, key, n * key_length);
    // 3. 插入 rid 值
    Rid* rid_slot = get_rid(pos);
    int rid_length = sizeof(Rid);
    // 右移现有的 rid 值
    memmove(rid_slot + n, rid_slot, move_size * rid_length);
    // 插入新的 rid 值
    memcpy(rid_slot, rid, n * rid_length);
    // 4. 更新当前节点的键数量
    NUMKEY += n;
}

/**
 * @brief 用于在结点中插入单个键值对。
 * 函数返回插入后的键值对数量
 *
 * @param (key, value) 要插入的键值对
 * @return int 键值对数量
 */
int IxNodeHandle::insert(const char* key, const Rid& value) {
    // 1. 查找要插入的键值对应该插入到当前节点的哪个位置
    int insert_position = lower_bound(key);
    // 2. 如果key重复则不插入，抛出异常
    if (insert_position < NUMKEY && COMPARE(key, insert_position) == 0) {
        throw UniqueConstraintError();
    }
    // 3. 如果key不重复则插入键值对
    insert_pair(insert_position, key, value);
    // 4. 返回完成插入操作之后的键值对数量
    return NUMKEY;
}

/**
 * @brief 用于在结点中的指定位置删除单个键值对
 *
 * @param pos 要删除键值对的位置
 */
void IxNodeHandle::erase_pair(int pos) {
    // 1. 检查位置的合法性
    if (pos < 0 || pos >= NUMKEY) {
        return;
    }
    // 2. 删除该位置的 key
    int move_size = NUMKEY - pos - 1;
    char* key_slot = get_key(pos);
    int key_length = file_hdr->col_tot_len_;
    memmove(key_slot, key_slot + key_length, move_size * key_length);
    // 3. 删除该位置的 rid
    Rid* rid_slot = get_rid(pos);
    int rid_length = sizeof(Rid);
    memmove(rid_slot, rid_slot + 1, move_size * rid_length);
    // 4. 更新结点的键值对数量
    NUMKEY--;
}

/**
 * @brief 用于在结点中删除指定key的键值对。函数返回删除后的键值对数量
 *
 * @param key 要删除的键值对key值
 * @return 完成删除操作后的键值对数量
 */
int IxNodeHandle::remove(const char* key) {
    // 1. 查找要删除键值对的位置
    // 2. 如果要删除的键值对存在，删除键值对
    // 3. 返回完成删除操作后的键值对数量
    int remove_index = lower_bound(key);
    if (remove_index != NUMKEY && COMPARE(key, remove_index) == 0)
        erase_pair(remove_index);
    return NUMKEY;
}

IxIndexHandle::IxIndexHandle(DiskManager* disk_manager, BufferPoolManager* buffer_pool_manager, int fd)
    : disk_manager_(disk_manager), buffer_pool_manager_(buffer_pool_manager), fd_(fd) {
    // 初始化file_hdr_
    char* buffer = new char[PAGE_SIZE];
    memset(buffer, 0, PAGE_SIZE);
    disk_manager_->read_page(fd, IX_FILE_HDR_PAGE, buffer, PAGE_SIZE);
    file_hdr_ = std::make_unique<IxFileHdr>();
    file_hdr_->deserialize(buffer);
    delete[] buffer;

    // disk_manager管理的fd对应的文件中，设置从file_hdr_->num_pages开始分配page_no
    int now_page_no = disk_manager_->get_fd2pageno(fd);
    disk_manager_->set_fd2pageno(fd, now_page_no + 1);

    // 初始化node_
    node_ = fetch_node(file_hdr_->last_leaf_);
    head_ = fetch_node(IX_FILE_HDR_PAGE);
}

/**
 * @brief 用于查找指定键所在的叶子结点
 * @param key 要查找的目标key值
 * @param operation 查找到目标键值对后要进行的操作类型
 * @param txn 事务参数，如果不需要则默认传入nullptr
 * @return [leaf node] and [root_is_latched] 返回目标叶子结点以及根结点是否加锁
 * @note need to Unlatch and unpin the leaf node outside!
 * 注意：用了find_leaf_page之后一定要unlatch叶结点，否则下次latch该结点会堵塞！
 */
std::pair<std::shared_ptr<IxNodeHandle>, bool> IxIndexHandle::find_leaf_page(const char* key, Operation operation, std::shared_ptr<Transaction> txn, bool find_first) {
    // 1. 获取根节点
    std::shared_ptr<IxNodeHandle> current_node = fetch_node(file_hdr_->root_page_);
    // 2. 从根节点开始不断向下查找目标 key
    while (!current_node->page_hdr->is_leaf) {
        page_id_t next_page_id = current_node->internal_lookup(key);
        buffer_pool_manager_->unpin_page(current_node->get_page_id(), false);
        current_node = fetch_node(next_page_id);
    }
    // 3. 返回包含目标 key 的叶子结点以及根节点是否加锁
    return std::make_pair(current_node, false);
}

/**
 * @brief 用于查找指定键在叶子结点中的对应的值result
 *
 * @param key 查找的目标key值
 * @param result 用于存放结果的容器
 * @param txn 事务指针
 * @return bool 返回目标键值对是否存在
 */
bool IxIndexHandle::get_value(const char* key, std::vector<Rid>* result, std::shared_ptr<Transaction> txn) {
    // 提示：使用完buffer_pool提供的page之后，记得unpin page；记得处理并发的上锁
    std::scoped_lock lock{root_latch_};
    // 1. 获取目标key值所在的叶子结点
    auto [leaf, _] = find_leaf_page(key, Operation::FIND, txn);
    Rid* rid = nullptr;
    // 2. 在叶子节点中查找目标key值的位置，并读取key对应的rid
    bool key_exists = leaf->leaf_lookup(key, &rid);
    // 3. 把rid存入result参数中
    if (key_exists)
        result->push_back(*rid);
    buffer_pool_manager_->unpin_page(leaf->get_page_id(), false);
    return key_exists;
}

/**
 * @brief  将传入的一个node拆分(Split)成两个结点，在node的右边生成一个新结点new node
 * @param node 需要拆分的结点
 * @return 拆分得到的new_node
 * @note need to unpin the new node outside
 * 注意：本函数执行完毕后，原node和new node都需要在函数外面进行unpin
 */
std::shared_ptr<IxNodeHandle> IxIndexHandle::split(IxNodeHandle* node) {
    // 创建新节点
    std::shared_ptr<IxNodeHandle> new_node = create_node();
    // 1. 将原节点的键值对平均分配，右半部分分裂为新的右兄弟节点
    int split_pos = node->page_hdr->num_key / 2;
    new_node->page_hdr->is_leaf = node->page_hdr->is_leaf;
    new_node->page_hdr->parent = node->page_hdr->parent;
    new_node->page_hdr->next_free_page_no = node->page_hdr->next_free_page_no;
    // 插入新的键值对到新节点
    new_node->insert_pairs(0, node->get_key(split_pos), node->get_rid(split_pos), node->page_hdr->num_key - split_pos);
    node->page_hdr->num_key = split_pos;
    // 2. 如果新的右兄弟节点是叶子节点，更新新旧节点的 prev_leaf 和 next_leaf 指针
    if (new_node->page_hdr->is_leaf) {
        new_node->page_hdr->prev_leaf = node->get_page_no();
        new_node->page_hdr->next_leaf = node->page_hdr->next_leaf;
        // 更新新节点的下一个叶子节点的 prev_leaf 指针
        if (new_node->page_hdr->next_leaf != INVALID_PAGE_ID) {
            std::shared_ptr<IxNodeHandle> next_leaf = fetch_node(new_node->page_hdr->next_leaf);
            next_leaf->page_hdr->prev_leaf = new_node->get_page_no();
            buffer_pool_manager_->unpin_page(next_leaf->get_page_id(), true);
        }
        node->page_hdr->next_leaf = new_node->get_page_no();
    } else {
        // 3. 如果新的右兄弟节点不是叶子节点，更新该节点的所有孩子节点的父节点信息
        for (int i = 0; i < new_node->page_hdr->num_key; ++i) {
            maintain_child(new_node.get(), i);
        }
    }
    return new_node;
}

std::shared_ptr<IxNodeHandle> IxIndexHandle::sorted_split(IxNodeHandle* node) {
    // 1. 将原结点的键值对平均分配，右半部分分裂为新的右兄弟结点
    //    需要初始化新节点的page_hdr内容
    // 2. 如果新的右兄弟结点是叶子结点，更新新旧节点的prev_leaf和next_leaf指针
    //    为新节点分配键值对，更新旧节点的键值对数记录
    // 3. 如果新的右兄弟结点不是叶子结点，更新该结点的所有孩子结点的父节点信息(使用IxIndexHandle::maintain_child())
    std::shared_ptr<IxNodeHandle> split_ = create_node();
    split_->page_hdr->is_leaf = true;
    split_->page_hdr->parent = node->page_hdr->parent;
    split_->page_hdr->next_free_page_no = node->page_hdr->next_free_page_no;
    split_->page_hdr->num_key = 0;
    head_->page_hdr->prev_leaf = split_->get_page_no();
    split_->page_hdr->prev_leaf = node->get_page_no();
    split_->page_hdr->next_leaf = node->page_hdr->next_leaf;
    node->page_hdr->next_leaf = split_->get_page_no();
    return split_;
}

/**
 * @brief Insert key & value pair into internal page after split
 * 拆分(Split)后，向上找到old_node的父结点
 * 将new_node的第一个key插入到父结点，其位置在 父结点指向old_node的孩子指针 之后
 * 如果插入后>=maxsize，则必须继续拆分父结点，然后在其父结点的父结点再插入，即需要递归
 * 直到找到的old_node为根结点时，结束递归（此时将会新建一个根R，关键字为key，old_node和new_node为其孩子）
 *
 * @param (old_node, new_node) 原结点为old_node，old_node被分裂之后产生了新的右兄弟结点new_node
 * @param key 要插入parent的key
 * @note 一个结点插入了键值对之后需要分裂，分裂后左半部分的键值对保留在原结点，在参数中称为old_node，
 * 右半部分的键值对分裂为新的右兄弟节点，在参数中称为new_node（参考Split函数来理解old_node和new_node）
 * @note 本函数执行完毕后，new node和old node都需要在函数外面进行unpin
 */
void IxIndexHandle::insert_into_parent(IxNodeHandle* old_node, const char* key, IxNodeHandle* new_node, std::shared_ptr<Transaction> txn) {
    // 提示：记得unpin page
    // 1. 分裂前的结点（原结点, old_node）是否为根结点，如果为根结点需要分配新的root
    if (old_node->get_page_no() == file_hdr_->root_page_) {
        std::shared_ptr<IxNodeHandle> new_root = create_node();
        new_root->page_hdr->is_leaf = false;
        new_root->page_hdr->num_key = 0;
        new_root->page_hdr->parent = INVALID_PAGE_ID;
        new_root->page_hdr->next_free_page_no = IX_NO_PAGE;
        // 将新旧节点插入新根节点
        new_root->insert_pair(0, old_node->get_key(0), {old_node->get_page_no(), -1});
        new_root->insert_pair(1, key, {new_node->get_page_no(), -1});
        // 更新父节点指针
        new_node->page_hdr->parent = old_node->page_hdr->parent = new_root->get_page_no();
        // 更新file_hdr_中的根节点页号
        file_hdr_->root_page_ = new_root->get_page_no();
    } else {
        // 2. 获取原结点（old_node）的父亲结点
        std::shared_ptr<IxNodeHandle> parent = fetch_node(old_node->get_parent_page_no());
        // 3. 获取key对应的rid，并将(key, rid)插入到父亲结点
        int pos = parent->find_child(old_node);
        parent->insert_pair(pos + 1, key, {new_node->get_page_no(), -1});
        // 4. 如果父亲结点仍需要继续分裂，则进行递归插入
        if (parent->get_size() == parent->get_max_size()) {
            std::shared_ptr<IxNodeHandle> split_parent = split(parent.get());
            insert_into_parent(parent.get(), split_parent->get_key(0), split_parent.get(), txn);
            // buffer_pool_manager_->unpin_page( split_->get_page_id(), true );
        }
        // buffer_pool_manager_->unpin_page( parent->get_page_id(), true );
    }
}

/**
 * @brief 将指定键值对插入到B+树中
 * @param (key, value) 要插入的键值对
 * @param txn 事务指针
 * @return page_id_t 插入到的叶结点的page_no
 */
void IxIndexHandle::sorted_insert(const char* key, const Rid& value, std::shared_ptr<Transaction> txn) {
    if (node_->page_hdr->num_key == node_->get_max_size() - 1) {
        std::shared_ptr<IxNodeHandle> split_ = sorted_split(node_.get());
        split_->insert_pair(0, key, value);
        if (file_hdr_->last_leaf_ == node_->get_page_no())
            file_hdr_->last_leaf_ = split_->get_page_no();
        insert_into_parent(node_.get(), key, split_.get(), txn);
        node_ = split_;
    } else
        node_->insert_pair(node_->get_size(), key, value);
}
/**
 * @brief 将键值对插入叶子节点
 *
 * @param key 需要插入的key
 * @param value 需要插入的值
 * @param txn 事物指针
 * @return 返回插入的叶子节点的页号
 */
page_id_t IxIndexHandle::insert_entry(const char* key, const Rid& value, std::shared_ptr<Transaction> txn) {
    // 提示：记得unpin page；若当前叶子节点是最右叶子节点，则需要更新file_hdr_->last_leaf_；记得处理并发的上锁
    std::scoped_lock lock{root_latch_};
    // 1. 查找key值应该插入到哪个叶子节点
    auto [leaf, _] = find_leaf_page(key, Operation::INSERT, txn);
    // 2. 在该叶子节点中插入键值对
    int num_key = leaf->page_hdr->num_key;
    bool insertion_successful = (num_key != leaf->insert(key, value));
    // 3. 如果结点已满，分裂结点，并把新结点的相关信息插入父节点
    if (insertion_successful && leaf->page_hdr->num_key == leaf->get_max_size()) {
        std::shared_ptr<IxNodeHandle> split_ = split(leaf.get());
        // 如果是最右节点则更新 file_hdr_->last_leaf_
        if (file_hdr_->last_leaf_ == leaf->get_page_no())
            file_hdr_->last_leaf_ = split_->get_page_no();
        // 将新节点的相关信息插入父节点
        insert_into_parent(leaf.get(), split_->get_key(0), split_.get(), txn);
        buffer_pool_manager_->unpin_page(split_->get_page_id(), true);
    }
    buffer_pool_manager_->unpin_page(leaf->get_page_id(), insertion_successful);
    return leaf->get_page_id().page_no;
}

/**
 * @brief 用于删除B+树中含有指定key的键值对
 * @param key 要删除的key值
 * @param txn 事务指针
 */
bool IxIndexHandle::delete_entry(const char* key, std::shared_ptr<Transaction> txn) {
    // 上锁
    std::scoped_lock lock{root_latch_};

    // 找到key所在的叶子节点
    auto [leaf, _] = find_leaf_page(key, Operation::DELETE, txn);

    // 根据删除前后的键值对数量判断是否需要合并或重分配
    int num = leaf->get_size();
    bool delete_successful = (num != leaf->remove(key));
    if (delete_successful)
        coalesce_or_redistribute(leaf.get());
    buffer_pool_manager_->unpin_page(leaf->get_page_id(), delete_successful);
    return delete_successful;
}

/**
 * @brief 用于处理合并和重分配的逻辑，用于删除键值对后调用
 *
 * @param node 执行完删除操作的结点
 * @param txn 事务指针
 * @param root_is_latched 传出参数：根节点是否上锁，用于并发操作
 * @return 是否需要删除结点
 * @note User needs to first find the sibling of input page.
 * If sibling's size + input page's size >= 2 * page's minsize, then redistribute.
 * Otherwise, merge(Coalesce).
 */
bool IxIndexHandle::coalesce_or_redistribute(IxNodeHandle* node, std::shared_ptr<Transaction> txn, bool* root_is_latched) {
    // 1. 判断node结点是否为根节点

    //    1.1 如果是根节点，需要调用AdjustRoot() 函数来进行处理，返回根节点是否需要被删除
    if (node->get_page_no() == file_hdr_->root_page_) {
        return adjust_root(node);
    }
    //    1.2 如果不是根节点，并且不需要执行合并或重分配操作，则直接返回false，否则执行2
    if (node->get_size() >= node->get_min_size()) {
        maintain_parent(node);
        return false;
    }

    // 2. 获取node结点的父亲结点
    std::shared_ptr<IxNodeHandle> parent = fetch_node(node->get_parent_page_no());
    int index = parent->find_child(node);
    // 3. 寻找node结点的兄弟结点（优先选取前驱结点）
    int neighbor_index = index == 0 ? 1 : -1;
    std::shared_ptr<IxNodeHandle> neighbor = fetch_node(parent->get_rid(index + neighbor_index)->page_no);
    // 4. 如果node结点和兄弟结点的键值对数量之和，能够支撑两个B+树结点，则只需要重新分配键值对（调用redistribute函数）
    if (node->get_size() + neighbor->get_size() >= node->get_min_size() * 2) {
        redistribute(neighbor.get(), node, parent.get(), index);
        buffer_pool_manager_->unpin_page(parent->get_page_id(), true);
        buffer_pool_manager_->unpin_page(neighbor->get_page_id(), true);
        return false;
    }
    // 5. 如果不满足上述条件，则需要合并两个结点，将右边的结点合并到左边的结点（调用Coalesce函数）
    IxNodeHandle* neighbor_node = neighbor.get();
    IxNodeHandle* parent_node = parent.get();
    coalesce(&neighbor_node, &node, &parent_node, index, txn, root_is_latched);
    buffer_pool_manager_->unpin_page(parent->get_page_id(), true);
    buffer_pool_manager_->unpin_page(neighbor->get_page_id(), true);
    return true;
}

/**
 * @brief 用于当根结点被删除了一个键值对之后的处理
 * @param old_root_node 原根节点
 * @return bool 根结点是否需要被删除
 * @note size of root page can be less than min size and this method is only called within coalesce_or_redistribute()
 */
bool IxIndexHandle::adjust_root(IxNodeHandle* old_root_node) {
    // 1. 如果old_root_node是内部结点，并且大小为1，则直接把它的孩子更新成新的根结点
    if (!old_root_node->is_leaf_page() && old_root_node->page_hdr->num_key == 1) {
        // 获取唯一的子节点并将其设置为新的根节点
        std::shared_ptr<IxNodeHandle> child = fetch_node(old_root_node->get_rid(0)->page_no);
        // 释放旧根节点句柄
        release_node_handle(*old_root_node);
        // 更新文件头中的根页面
        file_hdr_->root_page_ = child->get_page_no();
        // 设置新的根节点的父页面为无效
        child->set_parent_page_no(IX_NO_PAGE);
        buffer_pool_manager_->unpin_page(child->get_page_id(), true);
        return true;
    }
    // 2. 如果old_root_node是叶结点，且大小为0，则直接更新root page
    if (old_root_node->is_leaf_page() && !old_root_node->page_hdr->num_key) {
        // 释放旧根节点句柄
        release_node_handle(*old_root_node);
        // 更新文件头中的根页面
        file_hdr_->root_page_ = 2;
        return true;
    }
    // 3. 除了上述两种情况，不需要进行操作
    return false;
}

/**
 * @brief 重新分配node和兄弟结点neighbor_node的键值对
 * redistribute key & value pairs from one page to its sibling page. If index == 0, move sibling page's first key
 * & value pair into end of input "node", otherwise move sibling page's last key & value pair into head of input "node".
 *
 * @param neighbor_node sibling page of input "node"
 * @param node input from method coalesce_or_redistribute()
 * @param parent the parent of "node" and "neighbor_node"
 * @param index node在parent中的rid_idx
 * @note node是之前刚被删除过一个key的结点
 * index=0，则neighbor是node后继结点，表示：node(left)      neighbor(right)
 * index>0，则neighbor是node前驱结点，表示：neighbor(left)  node(right)
 * 注意更新parent结点的相关kv对
 */
void IxIndexHandle::redistribute(IxNodeHandle* neighbor_node, IxNodeHandle* node, IxNodeHandle* parent, int index) {
    // 注意：neighbor_node的位置不同，需要移动的键值对不同，需要分类讨论
    // 1. 通过index判断neighbor_node是否为node的前驱结点
    bool is_predecessor = (index > 0);
    // 2. 从neighbor_node中移动一个键值对到node结点中
    int erase_pos = is_predecessor ? neighbor_node->get_size() - 1 : 0;
    int insert_pos = is_predecessor ? 0 : node->get_size();
    node->insert_pair(insert_pos, neighbor_node->get_key(erase_pos), *(neighbor_node->get_rid(erase_pos)));
    neighbor_node->erase_pair(erase_pos);
    // 3. 更新父节点中的相关信息，并且修改移动键值对对应孩字结点的父结点信息（maintain_child函数）
    maintain_child(node, insert_pos);
    maintain_parent(is_predecessor ? node : neighbor_node);
}

/**
 * @brief 合并(Coalesce)函数是将node和其直接前驱进行合并，也就是和它左边的neighbor_node进行合并；
 * 假设node一定在右边。如果上层传入的index=0，说明node在左边，那么交换node和neighbor_node，保证node在右边；合并到左结点，实际上就是删除了右结点；
 * Move all the key & value pairs from one page to its sibling page, and notify buffer pool manager to delete this page.
 * Parent page must be adjusted to take info of deletion into account. Remember to deal with coalesce or redistribute
 * recursively if necessary.
 *
 * @param neighbor_node sibling page of input "node" (neighbor_node是node的前结点)
 * @param node input from method coalesce_or_redistribute() (node结点是需要被删除的)
 * @param parent parent page of input "node"
 * @param index node在parent中的rid_idx
 * @return true means parent node should be deleted, false means no deletion happend
 * @note Assume that *neighbor_node is the left sibling of *node (neighbor -> node)
 */
bool IxIndexHandle::coalesce(IxNodeHandle** neighbor_node, IxNodeHandle** node, IxNodeHandle** parent, int index, std::shared_ptr<Transaction> txn, bool* root_is_latched) {
    // 提示：如果是叶子结点且为最右叶子结点，需要更新file_hdr_->last_leaf_
    // 1. 用index判断neighbor_node是否为node的前驱结点，若不是则交换两个结点，让neighbor_node作为左结点，node作为右结点
    if (index == 0) {
        std::swap(*node, *neighbor_node);
        index = 1;
    }
    // 2. 把node结点的键值对移动到neighbor_node中，并更新node结点孩子结点的父节点信息（调用maintain_child函数）
    if ((*node)->is_leaf_page() && (*node)->get_page_no() == file_hdr_->last_leaf_)
        file_hdr_->last_leaf_ = (*neighbor_node)->get_page_no();
    int insert_pos = (*neighbor_node)->get_size();
    (*neighbor_node)->insert_pairs(insert_pos, (*node)->get_key(0), (*node)->get_rid(0), (*node)->get_size());
    for (int i = 0; i < (*node)->get_size(); i++)
        maintain_child(*neighbor_node, i + insert_pos);
    // 3. 释放和删除node结点，并删除parent中node结点的信息，返回parent是否需要被删除
    if ((*node)->is_leaf_page())
        erase_leaf(*node);

    release_node_handle(**node);
    (*parent)->erase_pair(index);
    return coalesce_or_redistribute(*parent, txn);
}

/**
 * @brief 这里把iid转换成了rid，即iid的slot_no作为node的rid_idx(key_idx)
 * node其实就是把slot_no作为键值对数组的下标
 * 换而言之，每个iid对应的索引槽存了一对(key,rid)，指向了(要建立索引的属性首地址,插入/删除记录的位置)
 *32532
 * @param iid
 * @return Rid
 * @note iid和rid存的不是一个东西，rid是上层传过来的记录位置，iid是索引内部生成的索引槽位置
 */
Rid IxIndexHandle::get_rid(const Iid& iid) const {
    std::shared_ptr<IxNodeHandle> node = fetch_node(iid.page_no);
    if (iid.slot_no >= node->get_size()) {
        throw IndexEntryNotFoundError();
    }
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);  // unpin it!
    return *node->get_rid(iid.slot_no);
}

/**
 * @brief find_leaf_page + lower_bound
 *
 * @param key
 * @return Iid
 * @note 上层传入的key本来是int类型，通过(const char *)&key进行了转换
 * 可用*(int *)key转换回去
 */
Iid IxIndexHandle::lower_bound(const char* key) {
    std::scoped_lock lock{root_latch_};

    std::shared_ptr<IxNodeHandle> node = find_leaf_page(key, Operation::FIND, nullptr).first;
    int key_idx = node->lower_bound(key);
    Iid iid;

    // 如果当前页中没有大于key的，则第一个大于等于key的记录在下一页的0号位
    if (key_idx == node->get_size())
        if (node->get_next_leaf() == IX_LEAF_HEADER_PAGE)
            iid = leaf_end();
        else
            iid = {.page_no = node->get_next_leaf(), .slot_no = 0};
    else
        iid = {.page_no = node->get_page_no(), .slot_no = key_idx};

    // unpin leaf node
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);
    return iid;
}

/**
 * @brief find_leaf_page + upper_bound
 *
 * @param key
 * @return Iid
 */
Iid IxIndexHandle::upper_bound(const char* key) {
    std::scoped_lock lock{root_latch_};

    std::shared_ptr<IxNodeHandle> node = find_leaf_page(key, Operation::FIND, nullptr).first;
    int key_idx = node->upper_bound(key);

    Iid iid;
    if (key_idx >= node->get_size()) {
        if (node->get_next_leaf() == IX_LEAF_HEADER_PAGE)
            iid = leaf_end();
        else
            iid = {.page_no = node->get_next_leaf(), .slot_no = 0};
    } else {
        iid = {.page_no = node->get_page_no(), .slot_no = key_idx};
    }

    // unpin leaf node
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);
    return iid;
}

/**
 * @brief 指向最后一个叶子的最后一个结点的后一个
 * 用处在于可以作为IxScan的最后一个
 *
 * @return Iid
 */
Iid IxIndexHandle::leaf_end() const {
    std::shared_ptr<IxNodeHandle> node = fetch_node(file_hdr_->last_leaf_);
    Iid iid = {.page_no = file_hdr_->last_leaf_, .slot_no = node->get_size()};
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);  // unpin it!
    return iid;
}

/**
 * @brief 指向第一个叶子的第一个结点
 * 用处在于可以作为IxScan的第一个
 *
 * @return Iid
 */
Iid IxIndexHandle::leaf_begin() const {
    Iid iid = {.page_no = file_hdr_->first_leaf_, .slot_no = 0};
    return iid;
}

/**
 * @brief 获取一个指定结点
 *
 * @param page_no
 * @return IxNodeHandle*
 * @note pin the page, remember to unpin it outside!
 */
std::shared_ptr<IxNodeHandle> IxIndexHandle::fetch_node(int page_no) const {
    Page* page = buffer_pool_manager_->fetch_page(PageId{fd_, page_no});
    if (page == nullptr) {
        throw InternalError("fetch node failed");
    }
    return std::make_shared<IxNodeHandle>(file_hdr_.get(), page);
}

/**
 * @brief 创建一个新结点
 *
 * @return IxNodeHandle*
 * @note pin the page, remember to unpin it outside!
 * 注意：对于Index的处理是，删除某个页面后，认为该被删除的页面是free_page
 * 而first_free_page实际上就是最新被删除的页面，初始为IX_NO_PAGE
 * 在最开始插入时，一直是create node，那么first_page_no一直没变，一直是IX_NO_PAGE
 * 与Record的处理不同，Record将未插入满的记录页认为是free_page
 */
std::shared_ptr<IxNodeHandle> IxIndexHandle::create_node() {
    file_hdr_->num_pages_++;

    PageId new_page_id = {.fd = fd_, .page_no = INVALID_PAGE_ID};
    // 从3开始分配page_no，第一次分配之后，new_page_id.page_no=3，file_hdr_.num_pages=4
    Page* page = buffer_pool_manager_->new_page(&new_page_id);
    return std::make_shared<IxNodeHandle>(file_hdr_.get(), page);
}

/**
 * @brief 从node开始更新其父节点的第一个key，一直向上更新直到根节点
 *
 * @param node
 */
void IxIndexHandle::maintain_parent(IxNodeHandle* node) {
    IxNodeHandle* current = node;
    std::shared_ptr<IxNodeHandle> parent;
    while (current->get_parent_page_no() != IX_NO_PAGE) {
        // Load its parent
        parent = fetch_node(current->get_parent_page_no());
        int rank = parent->find_child(current);
        char* parent_key = parent->get_key(rank);
        char* child_first_key = current->get_key(0);
        if (memcmp(parent_key, child_first_key, file_hdr_->col_tot_len_) == 0) {
            assert(buffer_pool_manager_->unpin_page(parent->get_page_id(), false));  // 此时无须对父节点进行修改，直接释放即可
            break;
        }
        memcpy(parent_key, child_first_key, file_hdr_->col_tot_len_);  // 修改了父节点
        // @note 此时curr递归为其父节点，unpin掉原理的curr，但此时还不能unpin掉parent。
        assert(buffer_pool_manager_->unpin_page(current->get_page_id(), true));
        current = parent.get();
    }
    assert(buffer_pool_manager_->unpin_page(current->get_page_id(), true));
}

/**
 * @brief 要删除leaf之前调用此函数，更新leaf前驱结点的next指针和后继结点的prev指针
 *
 * @param leaf 要删除的leaf
 */
void IxIndexHandle::erase_leaf(IxNodeHandle* leaf) {
    assert(leaf->is_leaf_page());

    std::shared_ptr<IxNodeHandle> prev = fetch_node(leaf->get_prev_leaf());
    prev->set_next_leaf(leaf->get_next_leaf());
    buffer_pool_manager_->unpin_page(prev->get_page_id(), true);

    std::shared_ptr<IxNodeHandle> next = fetch_node(leaf->get_next_leaf());
    next->set_prev_leaf(leaf->get_prev_leaf());  // 注意此处是SetPrevLeaf()
    buffer_pool_manager_->unpin_page(next->get_page_id(), true);
}

/**
 * @brief 删除node时，更新file_hdr_.num_pages
 *
 * @param node
 */
void IxIndexHandle::release_node_handle(IxNodeHandle& node) {
    file_hdr_->num_pages_--;
}

/**
 * @brief 将node的第child_idx个孩子结点的父节点置为node
 */
void IxIndexHandle::maintain_child(IxNodeHandle* node, int child_idx) {
    if (!node->is_leaf_page()) {
        //  Current node is inner node, load its child and set its parent to current node
        int child_page_no = node->value_at(child_idx);
        std::shared_ptr<IxNodeHandle> child = fetch_node(child_page_no);
        child->set_parent_page_no(node->get_page_no());
        buffer_pool_manager_->unpin_page(child->get_page_id(), true);
    }
}