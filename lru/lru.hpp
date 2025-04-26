#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP
#include <ranges>

#include "utility.hpp"
#include "exceptions.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"


/**
    包含
        sjtu :: double_list <T >
        sjtu :: hashmap < Key ,T , Hash , Equal >
        sjtu :: linked_hashmap < Key ,T , Hash , Equal >
            // derived from sjtu :: hashmap < Key ,T , Hash , Equal >
        sjtu :: lru
        linked hashmap是派生类
    模板类的派生类在调用基类函数时，需要指定this
    double list包含实现linked hashmap所需要的双向链表的接口。
    自行实现链表，你可以自己新定义一些类来辅助实现。
    lru没有默认构造函数，构造时必须给定size参数。
*/


//hash接口
class Hash {
public:
    unsigned int operator ()(Integer lhs) const {
        int val = lhs.val;
        return std::hash<int>()(val);
    }
};

//equal接口
class Equal {
public:
    bool operator ()(const Integer &lhs, const Integer &rhs) const {
        return lhs.val == rhs.val;
    }
};

namespace sjtu {
    //Node,包括数据，前后的指针，用于实现双向链表
    template<typename T>
    class Node {
    public:
        T data;
        Node *prev;
        Node *next;

        explicit Node(T data): data(data), prev(nullptr), next(nullptr) {
        }
    };

    //———————————————————————————————————————double_list————————————————————————————————————————————————————//

    //双向链表
    template<class T>
    class double_list {
    public:
        Node<T> *head; // 指向头节点的指针
        Node<T> *tail; // 指向尾节点的指针
        int size; // 链表的大小
        // --------------------------
        //默认构造函数
        double_list() : head(nullptr), tail(nullptr), size(0) {
        }

        //拷贝构造函数，不断地在尾部插入值，复杂度n
        double_list(const double_list<T> &other) : head(nullptr), tail(nullptr), size(0) {
            for (Node<T> *current = other.head; current != nullptr; current = current->next) {
                insert_tail(current->data);
            }
        }

        //清空，复杂度n
        void clear() {
            while (head != nullptr) {
                delete_head();
            }
            size = 0;
        }

        //析构，复杂度n
        ~double_list() {
            while (head != nullptr) {
                delete_head();
            }
        }

        //内置迭代器类
        class iterator {
        public:
            Node<T> *current; //当前指向的数据指针
            double_list<T> *list; //所在的doublelist
            // --------------------------
            //默认构造函数
            iterator() : current(nullptr), list(nullptr) {
            }

            //初始化
            iterator(Node<T> *node, double_list<T> *lst) : current(node), list(lst) {
            }

            //拷贝
            iterator(const iterator &t) : current(t.current), list(t.list) {
            }

            //析构
            ~iterator() = default;

            //后移，区分前后缀
            iterator operator++(int) {
                iterator temp = *this;
                if (current != nullptr) {
                    current = current->next;
                }
                return temp;
            }

            iterator &operator++() {
                if (current != nullptr) {
                    current = current->next;
                }
                return *this;
            }

            //前移，区分前后缀
            iterator operator--(int) {
                iterator temp = *this;
                if (current != nullptr) {
                    current = current->prev;
                }
                return temp;
            }

            iterator &operator--() {
                if (current != nullptr) {
                    current = current->prev;
                }
                return *this;
            }

            //解引用
            T &operator*() const {
                if (current == nullptr) {
                    throw std::invalid_argument("Invalid iterator");
                }
                return current->data;
            }

            //箭头解引用
            T *operator->() const noexcept {
                return &(current->data);
            }

            //判断是否相等
            bool operator==(const iterator &rhs) const {
                return current == rhs.current && list == rhs.list;
            }

            //判断是否不等
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        // 非const版本的begin
        iterator begin() {
            return iterator(head, this);
        }

        // const版本的begin
        iterator begin() const {
            return iterator(head, const_cast<double_list<T> *>(this));
        }

        // 非const版本的end
        iterator end() {
            return iterator(nullptr, this);
        }

        // const版本的end
        iterator end() const {
            return iterator(nullptr, const_cast<double_list<T> *>(this));
        }

        //去掉某一个元素，注意头尾特判等
        iterator erase(iterator pos) {
            if (pos.current == nullptr) {
                return pos;
            }
            Node<T> *toDelete = pos.current;
            iterator nextIter(toDelete->next, this);
            if (toDelete->prev != nullptr) {
                toDelete->prev->next = toDelete->next;
            } else {
                head = toDelete->next;
            }
            if (toDelete->next != nullptr) {
                toDelete->next->prev = toDelete->prev;
            } else {
                tail = toDelete->prev;
            }
            delete toDelete;
            size--;
            return nextIter;
        }

        //在头部插入元素
        void insert_head(const T &val) {
            auto *newNode = new Node<T>(val);
            if (head == nullptr) {
                head = tail = newNode;
            } else {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
            size++;
        }

        //在尾部插入元素
        void insert_tail(const T &val) {
            auto *newNode = new Node<T>(val);
            if (tail == nullptr) {
                head = tail = newNode;
            } else {
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            }
            size++;
        }

        //去除头部元素
        void delete_head() {
            if (head == nullptr) {
                return;
            }
            Node<T> *temp = head;
            if (head == tail) {
                head = tail = nullptr;
            } else {
                head = head->next;
                head->prev = nullptr;
            }
            delete temp;
            size--;
        }

        //去除尾部元素
        void delete_tail() {
            if (tail == nullptr) {
                return;
            }
            Node<T> *temp = tail;
            if (head == tail) {
                head = tail = nullptr;
            } else {
                tail = tail->prev;
                tail->next = nullptr;
            }
            delete temp;
            size--;
        }

        //是否为空
        bool empty() const {
            return size == 0;
        }
    };

    //————————————————————————————————————————hashmap————————————————————————————————————————————————————————//

    template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key> >
    class hashmap {
    private:
        using value_type = pair<const Key, T>;
        std::vector<double_list<value_type> > buckets; //用双向列表作为一个桶，有很多个桶
        size_t size_; //哈希表的大小
        static constexpr double LOAD_FACTOR_THRESHOLD = 0.5; //负载因子


        // --------------------------
        //默认设置为16大小
        public:
        hashmap() : size_(0) {
            buckets.resize(16);
        }

        //拷贝
        hashmap(const hashmap &other) : size_(other.size_) {
            buckets.resize(other.buckets.size());
            for (size_t i = 0; i < other.buckets.size(); ++i) {
                for (auto it = other.buckets[i].begin(); it != other.buckets[i].end(); ++it) {
                    insert(*it);
                }
            }
        }

        //析构
        ~hashmap() {
            clear();
        }

        //赋值运算符重载
        hashmap &operator=(const hashmap &other) {
            if (this != &other) {
                clear();
                size_ = other.size_;
                buckets.resize(other.buckets.size());
                for (size_t i = 0; i < other.buckets.size(); ++i) {
                    for (auto it = other.buckets[i].begin(); it != other.buckets[i].end(); ++it) {
                        insert(*it);
                    }
                }
            }
            return *this;
        }



        //内置指针类
        class iterator {
        public:
            typename double_list<value_type>::iterator list_it; //双向列表的指针
            size_t bucket_index; //第几位
            const hashmap *map; //所在的哈希表

            // --------------------------
            iterator() : bucket_index(0), map(nullptr) {
            }

            iterator(typename double_list<value_type>::iterator it, size_t index, const hashmap *m)
                : list_it(it), bucket_index(index), map(m) {
            }

            iterator(const iterator &t) : list_it(t.list_it), bucket_index(t.bucket_index), map(t.map) {
            }

            ~iterator() {
            }

            value_type &operator*() const {
                return *list_it;
            }

            value_type *operator->() const noexcept {
                return list_it.operator->();
            }

            bool operator==(const iterator &rhs) const {
                return list_it == rhs.list_it && map == rhs.map;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        //清空
        void clear() {
            for (auto &bucket: buckets) {
                while (!bucket.empty()) {
                    bucket.delete_head();
                }
            }
            size_ = 0;
        }

        //扩容为两倍
        void expand() {
            std::vector<double_list<value_type> > old_buckets = buckets;
            buckets.clear();
            buckets.resize(old_buckets.size() * 2);
            size_ = 0;
            for (auto &bucket: old_buckets) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    insert(*it);
                }
            }
        }

        iterator end() const {
            return iterator(typename double_list<value_type>::iterator(), buckets.size(), this);
        }

        //在桶里查找
        iterator find(const Key &key) const {
            Hash hasher;
            size_t index = hasher(key) % buckets.size();
            for (auto it = buckets[in   dex].begin(); it != buckets[index].end(); ++it) {
                if (Equal{}(it->first, key)) {
                    return iterator(it, index, this);
                }
            }
            return end();
        }

        //插入，如果存在，更新，如果不存在，插入新元素，注意是否要扩容
        sjtu::pair<iterator, bool> insert(const value_type &value_pair) {
            Hash hasher;
            size_t index = hasher(value_pair.first) % buckets.size();
            for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
                if (Equal{}(it->first, value_pair.first)) {
                    // 如果键已经存在，更新值
                    it->second = value_pair.second;
                    return {iterator(it, index, this), false};
                }
            }
            // 检查负载因子是否超过阈值，若超过则扩容
            if (static_cast<double>(size_) / buckets.size() >= LOAD_FACTOR_THRESHOLD) {
                expand();
                index = hasher(value_pair.first) % buckets.size();
            }
            // 在桶的头部插入新元素
            buckets[index].insert_head(value_pair);
            ++size_;
            return {iterator(buckets[index].begin(), index, this), true};
        }

        //remove，找不找得到元素
        bool remove(const Key &key) {
            Hash hasher;
            size_t index = hasher(key) % buckets.size();
            // 遍历当前桶中的每个元素
            for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
                if (Equal{}(it->first, key)) {
                    // 如果找到键，删除该元素
                    buckets[index].erase(it);
                    --size_;
                    return true;
                }
            }
            return false;
        }

        T &operator[](const Key &key) {
            auto result = insert(value_type(key, T()));
            return result.first->second;
        }
    };

    //———————————————————————————————————————linked_hashmap—————————————————————————————————————————————————//


    template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key> >
    class linked_hashmap : public hashmap<Key, T, Hash, Equal> {
    public:
        friend class hashmap<Key, T, Hash, Equal>;
        typedef pair<const Key, T> value_type;

        // 用于维护插入顺序的双向链表
        double_list<value_type> insert_list;

        // 键到链表节点的映射，额外维护
        hashmap<Key, Node<value_type> *, Hash, Equal> key_to_node;
        // --------------------------
        class const_iterator;

        class iterator {
        public:
            Node<value_type> *current;
            linked_hashmap *map;

            iterator() : current(nullptr), map(nullptr) {
            }

            iterator(Node<value_type> *node, linked_hashmap *m) : current(node), map(m) {
            }

            iterator(const iterator &other) : current(other.current), map(other.map) {
            }

            ~iterator() {
            }

            typename double_list<value_type>::iterator getDoubleListIterator() {
                return typename double_list<value_type>::iterator(current, &map->insert_list);
            }

            //前后置的++--
            iterator operator++(int) {
                iterator temp = *this;
                if (current == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->next;
                }
                return temp;
            }

            iterator &operator++() {
                if (current == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->next;
                }
                return *this;
            }

            iterator operator--(int) {
                iterator temp = *this;
                if (current == nullptr || current->prev == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->prev;
                }
                return temp;
            }

            iterator &operator--() {
                if (current == nullptr || current->prev == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->prev;
                }
                return *this;
            }

            //解引用
            value_type &operator*() const {
                if (current == nullptr) {
                    throw std::runtime_error("star invalid");
                }
                return current->data;
            }

            value_type *operator->() const noexcept {
                return &(current->data);
            }

            bool operator==(const iterator &rhs) const {
                return current == rhs.current && map == rhs.map;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current && map == rhs.map;
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        //const iterator基本同iterator
        class const_iterator {
        public:
            const Node<value_type> *current;
            const linked_hashmap *map;

            const_iterator() : current(nullptr), map(nullptr) {
            }

            const_iterator(const Node<value_type> *node, const linked_hashmap *m) : current(node), map(m) {
            }

            const_iterator(const iterator &other) : current(other.current), map(other.map) {
            }

            const_iterator operator++(int) {
                const_iterator temp = *this;
                if (current == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->next;
                }
                return temp;
            }

            const_iterator &operator++() {
                if (current == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->next;
                }
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator temp = *this;
                if (current == nullptr || current->prev == nullptr) {
                    throw "aaa";
                }
                if (current != nullptr) {
                    current = current->prev;
                }
                return temp;
            }

            const_iterator &operator--() {
                if (current == nullptr || current->prev == nullptr) {
                    throw index_out_of_bound();
                }
                if (current != nullptr) {
                    current = current->prev;
                }
                return *this;
            }

            const value_type &operator*() const {
                if (current == nullptr) {
                    throw std::runtime_error("star invalid");
                }
                return current->data;
            }

            const value_type *operator->() const noexcept {
                return &(current->data);
            }

            bool operator==(const iterator &rhs) const {
                return current == rhs.current && map == rhs.map;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current && map == rhs.map;
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        linked_hashmap() : hashmap<Key, T, Hash, Equal>() {
        }

        linked_hashmap(const linked_hashmap &other) : hashmap<Key, T, Hash, Equal>(other) {
            // 复制插入顺序链表
            for (auto it = other.insert_list.begin(); it != other.insert_list.end(); ++it) {
                insert_list.insert_tail(*it);
                key_to_node.insert({it->first, insert_list.tail});
            }
        }

        ~linked_hashmap() {
            // 清理插入顺序链表
            insert_list.clear();
            key_to_node.clear();
        }

        linked_hashmap &operator=(const linked_hashmap &other) {
            if (this != &other) {
                // 先赋值基类部分
                hashmap<Key, T, Hash, Equal>::operator=(other);
                // 清理当前插入顺序链表
                insert_list.clear();
                key_to_node.clear();
                // 复制新的插入顺序链表
                for (auto it = other.insert_list.begin(); it != other.insert_list.end(); ++it) {
                    insert_list.insert_tail(*it);
                    key_to_node.insert({it->first, it.current});
                }
            }
            return *this;
        }

        T &at(const Key &key) {
            auto it = hashmap<Key, T, Hash, Equal>::find(key);
            if (it == hashmap<Key, T, Hash, Equal>::end()) {
                throw std::out_of_range("Key not found");
            }
            return it->second;
        }

        const T &at(const Key &key) const {
            auto it = hashmap<Key, T, Hash, Equal>::find(key);
            if (it == hashmap<Key, T, Hash, Equal>::end()) {
                throw std::out_of_range("Key not found");
            }
            return it->second;
        }

        T &operator[](const Key &key) {
            auto it = hashmap<Key, T, Hash, Equal>::find(key);
            if (it == hashmap<Key, T, Hash, Equal>::end()) {
                throw std::out_of_range("Key not found");
            }
            auto result = hashmap<Key, T, Hash, Equal>::insert(value_type(key, T()));
            if (result.second) {
                insert_list.insert_tail(*result.first);
                key_to_node.insert({key, insert_list.tail});
            } else {
                // 若键已存在，移动到插入顺序链表头部
                auto list_it = find(key);
                insert_list.erase(list_it.getDoubleListIterator());
                insert_list.insert_head(*result.first);
                key_to_node[key] = insert_list.head;
            }
            return result.first->second;
        }

        const T &operator[](const Key &key) const {
            auto it = hashmap<Key, T, Hash, Equal>::find(key);
            if (it == hashmap<Key, T, Hash, Equal>::end()) {
                throw std::out_of_range("Key not found");
            }
            return it->second;
        }

        iterator begin() {
            return iterator(insert_list.head, this);
        }

        const_iterator cbegin() const {
            return const_iterator(insert_list.head, this);
        }

        iterator end() {
            return iterator(nullptr, this);
        }

        const_iterator cend() const {
            return const_iterator(nullptr, this);
        }

        bool empty() const {
            return insert_list.empty();
        }

        void clear() {
            hashmap<Key, T, Hash, Equal>::clear();
            insert_list.clear();
            key_to_node.clear();
        }

        size_t size() const {
            return insert_list.size;
        }

        //在插入新的键值对时，如果该键是首次插入，会在 insert_list 的尾部插入新节点，
        //同时将该键和对应的节点指针插入到 key_to_node 中。
        //如果键已经存在，需要将对应的节点移动到双向链表的尾部以更新插入顺序，此时可以通过 key_to_node 快速找到该节点。
        pair<iterator, bool> insert(const value_type &value) {
            auto hash_result = hashmap<Key, T, Hash, Equal>::insert(value);
            iterator new_it;
            if (hash_result.second) {
                // 在插入顺序链表插入新元素
                insert_list.insert_tail(value);
                key_to_node[value.first] = insert_list.tail;
                // 找到在插入顺序链表中的位置
                new_it = iterator(insert_list.tail, this);
            } else {
                // 若键已存在，移动到插入顺序链表尾部
                auto it = key_to_node.find(value.first);
                if (it != key_to_node.end()) {
                    Node<value_type>* old_node = it->second;
                    auto list_it = iterator(old_node, this);
                    insert_list.erase(list_it.getDoubleListIterator());
                    insert_list.insert_tail(value);
                    key_to_node[value.first] = insert_list.tail;

                }
                new_it = begin();
            }
            return {new_it, hash_result.second};
        }

        void remove(iterator pos) {
            if (pos.current == nullptr) {
                throw std::runtime_error("Invalid iterator");
            }
            auto x = pos-> first;
            // 从哈希表中删除元素
            hashmap<Key, T, Hash, Equal>::remove(x);
            // 从插入顺序链表中删除元素
            insert_list.erase(pos.getDoubleListIterator());
            key_to_node.remove(x);
        }

        size_t count(const Key &key) const {
            return (hashmap<Key, T, Hash, Equal>::find(key) != hashmap<Key, T, Hash, Equal>::end()) ? 1 : 0;
        }

        iterator find(const Key &key) {
            auto it = key_to_node.find(key);
            if (it == key_to_node.end()) {
                return end();
            }
            return iterator(it->second, this);
        }
    };

    //———————————————————————————————————————lru———————————————————————————————————————————————————————————//

    class lru {
        using lmap = sjtu::linked_hashmap<Integer, Matrix<int>, Hash, Equal>;
        using value_type = sjtu::pair<const Integer, Matrix<int> >;

        int capacity;
        lmap *memory;
    public:
        lru(int size) : capacity(size) {
            memory = new linked_hashmap<Integer, Matrix<int>, Hash, Equal>();
        }

        ~lru() {
            delete memory;
        }

        //插入：查找是否有k，如果没有，检查容量，判断是否删除最早的
        void save(const value_type &v)const {
            auto result = memory->insert(v);
            if (!result.second) {
                // 键已存在，insert函数已处理值的更新和位置移动
            } else {
                // 插入新元素后检查容量
                if (memory->size() > capacity) {
                    auto first = memory->begin();
                    memory->remove(first);
                }
            }
        }

        Matrix<int> *get(const Integer &v) const{
            auto it = memory->find(v);
            if (it != memory->end()) {
                Integer key = it->first;
                Matrix<int> value = it->second;
                memory->remove(it);
                auto newIt = memory->insert({key,std::move(value)}).first; // 获取插入后的新迭代器
                return &(newIt->second);
            }
            return nullptr;
        }

        void print() {
            auto it = memory->begin();
            for (; it != memory->end(); ++it) {
                std::cout << (*it).first.val << " " << (*it).second << std::endl;
            }
        }
    };
}

#endif
