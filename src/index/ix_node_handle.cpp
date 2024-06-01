#include "ix_node_handle.h"

/**
 * @brief 在当前node中查找第一个>=target的key_idx
 *
 * @return
 * key_idx，范围为[0,num_key)，如果返回的key_idx=num_key，则表示target大于最后一个key
 * @note 返回key index（同时也是rid index），作为slot no
 */
int IxNodeHandle::lower_bound(const char* target) const {
    // Todo:
    // 查找当前节点中第一个大于等于target的key，并返回key的位置给上层
    // 提示:
    // 可以采用多种查找方式，如顺序遍历、二分查找等；使用ix_compare()函数进行比较

    // 递增排序用二分查找
    int left = 0, right = page_hdr->num_key - 1;
    while (left <= right) {
        int mid = (left + right) >> 1;
        // target大于keys[mid],用get_key获取mid位置的key
        if (ix_compare(target, get_key(mid), file_hdr->col_type,
                       file_hdr->col_len) > 0) {
            left = mid + 1;
        } else {  // target小于等于keys[mid]
            right = mid - 1;
        }
    }
    return left;
}

/**
 * @brief 在当前node中查找第一个>target的key_idx
 *
 * @return
 * key_idx，范围为[1,num_key)，如果返回的key_idx=num_key，则表示target大于等于最后一个key
 * @note 注意此处的范围从1开始
 */
int IxNodeHandle::upper_bound(const char* target) const {
    // Todo:
    // 查找当前节点中第一个大于target的key，并返回key的位置给上层
    // 提示:
    // 可以采用多种查找方式：顺序遍历、二分查找等；使用ix_compare()函数进行比较
    // 二分查找第一个大于等于的key
    int index = lower_bound(target);
    // 找第一个严格大于的key
    while (index < page_hdr->num_key &&
           0 == ix_compare(get_key(index), target, file_hdr->col_type,
                           file_hdr->col_len))
        index++;

    return index;
}

/**
 * @brief 用于叶子结点根据key来查找该结点中的键值对
 * 值value作为传出参数，函数返回是否查找成功
 *
 * @param key 目标key
 * @param[out] value 传出参数，目标key对应的Rid
 * @return 目标key是否存在
 */
bool IxNodeHandle::LeafLookup(const char* key, Rid** value) {
    // Todo:
    // 1. 在叶子节点中获取目标key所在位置
    // 2. 判断目标key是否存在
    // 3. 如果存在，获取key对应的Rid，并赋值给传出参数value
    // 提示：可以调用lower_bound()和get_rid()函数。
    // 查找
    int index = lower_bound(key);
    // 判断存不存在目标key
    if (index == page_hdr->num_key ||
        ix_compare(get_key(index), key, file_hdr->col_type,
                   file_hdr->col_len) != 0)
        return false;
    // 传键对应的值给value
    *value = get_rid(index);
    return true;
}

/**
 * 用于内部结点（非叶子节点）查找目标key所在的孩子结点（子树）
 * @param key 目标key
 * @return page_id_t 目标key所在的孩子节点（子树）的存储页面编号
 */
page_id_t IxNodeHandle::InternalLookup(const char* key) {
    // Todo:
    // 1. 查找当前非叶子节点中目标key所在孩子节点（子树）的位置
    // 2. 获取该孩子节点（子树）所在页面的编号
    // 3. 返回页面编号
    // key左边的value都是小于key的，所以用upper_bound找第一个大于target的key，其左边就是target所在页
    int index = upper_bound(key);
    // 获取目标页
    Rid* target_node = get_rid(index - 1);
    // 返回页编号
    return target_node->page_no;
}

/**
 * @brief 在指定位置插入n个连续的键值对
 * 将key的前n位插入到原来keys中的pos位置；将rid的前n位插入到原来rids中的pos位置
 *
 * @param pos 要插入键值对的位置
 * @param (key, rid) 连续键值对的起始地址，也就是第一个键值对，可以通过(key,
 * rid)来获取n个键值对
 * @param n 键值对数量
 * @note [0,pos)           [pos,num_key)
 *                            key_slot
 *                            /      \
 *                           /        \
 *       [0,pos)     [pos,pos+n)   [pos+n,num_key+n)
 *                      key           key_slot
 */
void IxNodeHandle::insert_pairs(int pos,
                                const char* key,
                                const Rid* rid,
                                int n) {
    // Todo:
    // 1. 判断pos的合法性
    // 2. 通过key获取n个连续键值对的key值，并把n个key值插入到pos位置
    // 3. 通过rid获取n个连续键值对的rid值，并把n个rid值插入到pos位置
    // 4. 更新当前节点的键数量
    // 非法位置
    if (pos < 0 || pos > GetSize()) {
        throw InternalError("Invalid insert pos");
    }
    char* move_key;
    for (int i = page_hdr->num_key - 1; i >= pos; i--) {
        move_key = get_key(i);
        set_key(i + n, move_key);
    }
    for (int i = n - 1; i >= 0; i--) {
        set_key(i + pos, key + i * file_hdr->col_len);
    }

    Rid move_rid;
    for (int i = page_hdr->num_key - 1; i >= pos; i--) {
        move_rid = *get_rid(i);
        set_rid(i + n, move_rid);
    }
    for (int i = n - 1; i >= 0; i--) {
        move_rid = rid[i];
        set_rid(i + pos, move_rid);
    }

    page_hdr->num_key += n;
}

/**
 * @brief 用于在结点中的指定位置插入单个键值对
 */
void IxNodeHandle::insert_pair(int pos, const char* key, const Rid& rid) {
    insert_pairs(pos, key, &rid, 1);
};

/**
 * @brief 用于在结点中插入单个键值对。
 * 函数返回插入后的键值对数量
 *
 * @param (key, value) 要插入的键值对
 * @return int 键值对数量
 */
int IxNodeHandle::Insert(const char* key, const Rid& value) {
    // Todo:
    // 1. 查找要插入的键值对应该插入到当前节点的哪个位置
    // 2. 如果key重复则不插入
    // 3. 如果key不重复则插入键值对
    // 4. 返回完成插入操作之后的键值对数量
    int pos = lower_bound(key);
    // 如果key大于当前所有的key，则getkey不了，先特判
    if (pos == page_hdr->num_key ||
        ix_compare(get_key(pos), key, file_hdr->col_type, file_hdr->col_len) !=
            0) {
        insert_pair(pos, key, value);
    }
    return page_hdr->num_key;
}

/**
 * @brief 用于在结点中的指定位置删除单个键值对
 *
 * @param pos 要删除键值对的位置
 */
void IxNodeHandle::erase_pair(int pos) {
    // Todo:
    // 1. 删除该位置的key
    // 2. 删除该位置的rid
    // 3. 更新结点的键值对数量
    if (pos < 0 || pos >= GetSize())
        return;
    // pos位置的key之后的所有key的长度之和
    int remain = page_hdr->num_key - (pos + 1);
    char* key_slot = get_key(pos);
    int len = file_hdr->col_len;
    // 覆盖pos位置的key
    memmove(key_slot, key_slot + len, len * remain);

    Rid* rid = get_rid(pos);
    len = sizeof(Rid);
    memmove(rid, rid + 1, len * remain);
    page_hdr->num_key--;
}

/**
 * @brief 用于在结点中删除指定key的键值对。函数返回删除后的键值对数量
 *
 * @param key 要删除的键值对key值
 * @return 完成删除操作后的键值对数量
 */
int IxNodeHandle::Remove(const char* key) {
    // Todo:
    // 1. 查找要删除键值对的位置
    // 2. 如果要删除的键值对存在，删除键值对
    // 3. 返回完成删除操作后的键值对数量
    int key_index = lower_bound(key);
    if (key_index != page_hdr->num_key &&
        ix_compare(key, get_key(key_index), file_hdr->col_type,
                   file_hdr->col_len) == 0) {
        erase_pair(key_index);
    }
    return page_hdr->num_key;
}

/**
 * @brief
 * 由parent调用，寻找child，返回child在parent中的rid_idx∈[0,page_hdr->num_key)
 *
 * @param child
 * @return int
 */
int IxNodeHandle::find_child(IxNodeHandle* child) {
    int rid_idx;
    for (rid_idx = 0; rid_idx < page_hdr->num_key; rid_idx++) {
        if (get_rid(rid_idx)->page_no == child->GetPageNo()) {
            break;
        }
    }
    assert(rid_idx < page_hdr->num_key);
    return rid_idx;
}

/**
 * @brief used in internal node to remove the last key in root node, and return
 * the last child
 *
 * @return the last child
 */
page_id_t IxNodeHandle::RemoveAndReturnOnlyChild() {
    assert(GetSize() == 1);
    page_id_t child_page_no = ValueAt(0);
    erase_pair(0);
    assert(GetSize() == 0);
    return child_page_no;
}