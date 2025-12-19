//
// Created by xyx on 2025/12/12.
//
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

template<class K>
class AVLTree {
    struct AVLNode {
        K key;
        AVLNode *left;
        AVLNode *right;
        int height;

        template<class T>
            requires std::constructible_from<K, T &&>
        AVLNode(T &&key, AVLNode *left = nullptr, AVLNode *right = nullptr) : key(std::forward<T>(key)), left(left),
                                                                              right(right), height(1) {
        }
    };

    AVLNode *root;

public:
    AVLTree() : root(nullptr) {
    }

    ~AVLTree() {
        post_order(root, [](AVLNode *node) {
            delete node;
        });
    }

    template<class T>
        requires std::constructible_from<K, T &&>
    void put(T &&key) {
        root = put(root, std::forward<T>(key));
    }

    K remove(const K &key) {
        AVLNode *node = nullptr;
        root = remove(root, key, node);
        if (node == nullptr) {
            throw std::runtime_error("key not found!");
        }

        K ret = node->key;
        delete node;
        return ret;
    }

    bool contains(const K &key) {
        AVLNode *cur = root;
        while (cur != nullptr) {
            if (key < cur->key) {
                cur = cur->left;
            } else if (cur->key < key) {
                cur = cur->right;
            } else {
                return true;
            }
        }
        return false;
    }

    int greater_count(const K &low) {
        return greater_count(root, low);
    }

private:
    int greater_count(AVLNode *node, const K &low) {
        if (node == nullptr) return 0;
        if (node->key <= low) return greater_count(node->right, low);
        return 1 + greater_count(node->left, low) + greater_count(node->right, low);
    }

    void post_order(AVLNode *node, const std::function<void(AVLNode *)> &func) {
        if (node == nullptr) {
            return;
        }
        post_order(node->left, func);
        post_order(node->right, func);
        func(node);
    }

    AVLNode *min(AVLNode *node) {
        if (node == nullptr) {
            throw std::runtime_error("tree is empty!");
        }
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    int get_height(AVLNode *node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    void update_height(AVLNode *node) {
        if (node == nullptr) {
            return;
        }
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    }

    // 平衡因子
    int balance_factor(AVLNode *node) {
        return get_height(node->left) - get_height(node->right);
    }

    // 基础右旋
    // 时间复杂度：O(1)
    AVLNode *right_rotate(AVLNode *node) {
        AVLNode *root = node->left;
        node->left = root->right;
        root->right = node;
        update_height(node);
        update_height(root);
        return root;
    }

    // 基础左旋
    // 时间复杂度：O(1)
    AVLNode *left_rotate(AVLNode *node) {
        AVLNode *root = node->right;
        node->right = root->left;
        root->left = node;
        update_height(node);
        update_height(root);
        return root;
    }

    // 先左旋左子树，再右旋根节点
    AVLNode *left_right_rotate(AVLNode *node) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // 先右旋右子树，再左旋根节点
    AVLNode *right_left_rotate(AVLNode *node) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    AVLNode *balance(AVLNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        int bf = balance_factor(node);
        if (bf > 1) {
            if (balance_factor(node->left) < 0) {
                // LR
                node = left_right_rotate(node);
            } else {
                // LL
                node = right_rotate(node);
            }
        } else if (bf < -1) {
            if (balance_factor(node->right) > 0) {
                // RL
                node = right_left_rotate(node);
            } else {
                // RR
                node = left_rotate(node);
            }
        }
        return node;
    }

    // 从插入节点开始，向上更新高度，并在第一个失衡节点进行旋转
    template<class T>
        requires std::constructible_from<K, T &&>
    AVLNode *put(AVLNode *node, T &&key) {
        if (node == nullptr) {
            return new AVLNode(std::forward<T>(key));
        }

        if (node->key == key) {
            return node;
        }

        if (key < node->key) {
            node->left = put(node->left, std::forward<T>(key));
        } else {
            node->right = put(node->right, std::forward<T>(key));
        }
        update_height(node);
        return balance(node);
    }

    // 从被删除节点开始，向上更新高度，并对每个失衡节点进行旋转，最多旋转次数为O(log(n))
    AVLNode *remove(AVLNode *node, const K &key, AVLNode *&del) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            node->left = remove(node->left, key, del);
        } else if (node->key < key) {
            node->right = remove(node->right, key, del);
        } else {
            del = node;
            if (node->left == nullptr) {
                node = node->right;
            } else if (node->right == nullptr) {
                node = node->left;
            } else {
                AVLNode *successor = min(node->right);
                AVLNode *s_del;
                successor->right = remove(node->right, successor->key, s_del);
                successor->left = node->left;
                node = successor;
            }
        }

        update_height(node);
        return balance(node);
    }
};

template<class K>
concept Comparable = requires(const K &a, const K &b)
{
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template<Comparable K>
class RedBlackTree {
    enum Color { RED, BLACK };

    struct Node {
        K key;
        Node *left;
        Node *right;
        Node *parent;
        Color color = RED; // 新节点为红色

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        Node(TK &&key, Node *parent, Node *left = nullptr,
             Node *right = nullptr) : key(std::forward<TK>(key)), parent(parent),
                                      left(left), right(right) {
        }

        bool is_left_child() {
            return parent != nullptr && parent->left == this;
        }

        // 叔叔节点：父节点的兄弟节点
        Node *uncle() {
            if (parent == nullptr) {
                return nullptr;
            }
            return parent->sibling();
        }

        // 兄弟节点
        Node *sibling() {
            if (parent == nullptr) {
                return nullptr;
            }
            if (is_left_child()) {
                return parent->right;
            } else {
                return parent->left;
            }
        }
    };

    Node *root = nullptr;

public:
    template<typename TK>
        requires std::constructible_from<K, TK &&>
    void put(TK &&key) {
        Node *cur = root;
        Node *parent = nullptr;
        while (cur != nullptr) {
            parent = cur;
            if (key < cur->key) {
                cur = cur->left;
            } else if (cur->key < key) {
                cur = cur->right;
            } else {
                return;
            }
        }

        Node *new_node = new Node(std::forward<TK>(key), parent);
        if (parent == nullptr) {
            root = new_node;
        } else if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

        fix_red_red(new_node);
    }

    void remove(const K &key) {
        remove(get_node(key));
    }

    bool contains(const K &key) {
        return get_node(key) != nullptr;
    }

    int greater_count(const K &low) {
        return greater_count(root, low);
    }

private:
    int greater_count(Node *node, const K &low) {
        if (node == nullptr) return 0;
        if (node->key <= low) return greater_count(node->right, low);
        return 1 + greater_count(node->left, low) + greater_count(node->right, low);
    }

    Node *get_node(const K &key) {
        Node *cur = root;
        while (cur != nullptr) {
            if (key < cur->key) {
                cur = cur->left;
            } else if (cur->key < key) {
                cur = cur->right;
            } else {
                break;
            }
        }
        return cur;
    }

    bool is_red(Node *node) {
        return node != nullptr && node->color == RED;
    }

    // 空节点为黑色
    bool is_black(Node *node) {
        return node == nullptr || node->color == BLACK;
    }

    // 右旋
    void right_rotate(Node *node) {
        Node *root = node->left;
        Node *change = root->right;
        Node *parent = node->parent;

        node->left = change;
        if (change != nullptr) {
            change->parent = node;
        }

        if (parent == nullptr) {
            this->root = root;
        } else if (node->is_left_child()) {
            parent->left = root;
        } else {
            parent->right = root;
        }
        root->parent = parent;

        root->right = node;
        node->parent = root;
    }


    // 左旋
    void left_rotate(Node *node) {
        Node *root = node->right;
        Node *change = root->left;
        Node *parent = node->parent;

        node->right = change;
        if (change != nullptr) {
            change->parent = node;
        }

        if (parent == nullptr) {
            this->root = root;
        } else if (node->is_left_child()) {
            parent->left = root;
        } else {
            parent->right = root;
        }
        root->parent = parent;

        root->left = node;
        node->parent = root;
    }

    // 插入节点时，检查是否触发双红，即违反红色节点不能相邻的规则
    void fix_red_red(Node *node) {
        // 插入的节点为根节点，直接变为黑色
        if (node == root) {
            node->color = BLACK;
            return;
        }

        // 插入节点的父节点为黑色，无需调整
        if (is_black(node->parent)) {
            return;
        }

        // 插入节点的父节点为红色，违反了红色节点不能相邻的规则
        // 父节点为红色，说明父节点一定不是根节点，因此一定有祖父节点
        Node *parent = node->parent;
        Node *uncle = node->uncle();
        Node *grandparent = parent->parent;

        // 叔叔节点为红色
        if (is_red(uncle)) {
            // 叔叔节点为红色，说明叔叔节点不为空
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            fix_red_red(grandparent);
            return;
        }

        // 叔叔节点为黑色
        if (parent->is_left_child()) {
            if (node->is_left_child()) {
                parent->color = BLACK;
            } else {
                left_rotate(parent);
                node->color = BLACK;
            }
            // 提取公共部分
            grandparent->color = RED;
            right_rotate(grandparent);
        } else {
            // 与上述情况对称
            if (!node->is_left_child()) {
                // RR
                parent->color = BLACK;
            } else {
                // RL
                right_rotate(parent);
                node->color = BLACK;
            }
            grandparent->color = RED;
            left_rotate(grandparent);
        }
    }

    // 删除节点时，触发双黑，表示某条路径上缺少一个黑色节点
    void fix_black_black(Node *node) {
        if (node == root) {
            return;
        }

        Node *parent = node->parent;
        // 当前节点是黑色，一定有兄弟节点
        Node *sibling = node->sibling();

        // 被调整节点的兄弟节点为红色
        // 一定有两个黑色侄子节点，可通过旋转使黑色侄子成为新兄弟，转为其他情况
        if (is_red(sibling)) {
            if (node->is_left_child()) {
                left_rotate(parent);
            } else {
                right_rotate(parent);
            }
            // 更新颜色，暂时维持平衡
            parent->color = RED;
            sibling->color = BLACK;

            parent = node->parent;
            sibling = node->sibling();
        }


        // 被调整节点兄弟为黑色

        // 两个侄子节点均为黑色
        if (is_black(sibling->left) && is_black(sibling->right)) {
            sibling->color = RED;
            if (is_red(parent)) {
                parent->color = BLACK;
            } else {
                fix_black_black(parent);
            }
            return;
        }


        // 至少有一个侄子节点为红色
        if (sibling->is_left_child()) {
            if (is_red(sibling->left)) {
                // LL
                sibling->color = parent->color;
                sibling->left->color = BLACK;
            } else {
                // LR
                sibling->right->color = parent->color;
                left_rotate(sibling);
            }
            // 提取公共部分
            parent->color = BLACK;
            right_rotate(parent);
        } else {
            // 与上述情况对称
            if (is_red(sibling->right)) {
                // RR
                sibling->color = parent->color;
                sibling->right->color = BLACK;
            } else {
                sibling->left->color = parent->color;
                right_rotate(sibling);
            }
            parent->color = BLACK;
            left_rotate(parent);
        }
    }

    void remove(Node *node) {
        if (node == nullptr) {
            return;
        }

        // 待删除节点有两个孩子，转化成其他情况
        if (node->left != nullptr && node->right != nullptr) {
            Node *cur = node->right;
            while (cur->left != nullptr) {
                cur = cur->left;
            }
            // 只交换键值，而不是整个节点，可以简化操作
            std::swap(node->key, cur->key);
            node = cur;
        }

        // 待删除节点为根节点，单独处理
        if (node == root) {
            if (root->left == nullptr) {
                root = root->right;
            } else {
                root = root->left;
            }
            if (root != nullptr) {
                // 根节点只有一个孩子，说明这个孩子一定是红色叶节点，直接调整属性即可
                root->parent = nullptr;
                root->color = BLACK;
            }
            delete node;
            return;
        }

        Node *parent = node->parent;

        // 删除叶节点
        if (node->left == nullptr && node->right == nullptr) {
            // 删除黑色叶节点，会导致当前路径缺少一个黑色节点，需要调整平衡
            if (is_black(node)) {
                fix_black_black(node);
            }
            // 红色叶节点直接删除

            if (node->is_left_child()) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
            return;
        }

        // 删除带一个孩子的节点
        Node *replace;
        if (node->left == nullptr) {
            replace = node->right;
        } else {
            replace = node->left;
        }

        if (node->is_left_child()) {
            parent->left = replace;
        } else {
            parent->right = replace;
        }
        replace->parent = parent;

        replace->color = BLACK;
        delete node;
    }
};

template<class K>
class BTree {
    struct Node {
        // 孩子的数量比key的数量多1
        vector<K> keys;
        vector<Node *> children;

        bool leaf() {
            return children.empty();
        }

        Node *get(const K &key) {
            int i = get_index(key);
            if (i < keys.size() && keys[i] == key) {
                return this;
            }
            if (leaf()) {
                return nullptr;
            }
            return children[i]->get(key);
        }

        int get_index(const K &key) {
            // keys数组为升序，可用二分查找提高效率
            auto itr = std::lower_bound(keys.begin(), keys.end(), key);
            return itr - keys.begin();
        }

        Node *move_right_half(int from) {
            Node *new_node = new Node;
            new_node->keys.assign(keys.begin() + from, keys.end());
            keys.resize(from);
            if (!leaf()) {
                new_node->children.assign(children.begin() + from, children.end());
                children.resize(from);
            }
            return new_node;
        }

        void append(Node *node) {
            keys.insert(keys.end(), node->keys.begin(), node->keys.end());
            children.insert(children.end(), node->children.begin(), node->children.end());
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_key(int index, TK &&key) {
            keys.insert(keys.begin() + index, std::forward<TK>(key));
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_first_key(TK &&key) {
            insert_key(0, std::forward<TK>(key));
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_last_key(TK &&key) {
            keys.push_back(std::forward<TK>(key));
        }

        void insert_child(int index, Node *child) {
            children.insert(children.begin() + index, child);
        }

        void insert_first_child(Node *child) {
            insert_child(0, child);
        }

        void insert_last_child(Node *child) {
            children.push_back(child);
        }

        K remove_key(int index) {
            K key = keys[index];
            keys.erase(keys.begin() + index);
            return key;
        }

        K remove_first_key() {
            return remove_key(0);
        }

        K remove_last_key() {
            return remove_key(keys.size() - 1);
        }

        Node *remove_child(int index) {
            Node *child = children[index];
            children.erase(children.begin() + index);
            return child;
        }

        Node *remove_first_child() {
            return remove_child(0);
        }

        Node *remove_last_child() {
            return remove_child(children.size() - 1);
        }

        Node *child_left_sibling(int index) {
            return index > 0 ? children[index - 1] : nullptr;
        }

        Node *child_right_sibling(int index) {
            return index < children.size() - 1 ? children[index + 1] : nullptr;
        }
    };

    Node *root;
    const int min_degree;
    const int MAX_KEY_NUM;
    const int MIN_KEY_NUM;

public:
    explicit BTree(int min_degree = 2) : min_degree(min_degree), MAX_KEY_NUM(2 * min_degree - 1),
                                         MIN_KEY_NUM(min_degree - 1) {
        if (min_degree < 2) {
            throw std::invalid_argument("min_degree must be >= 2");
        }
        root = new Node;
    }

    bool contains(const K &key) {
        return root->get(key) != nullptr;
    }

    template<typename TK>
        requires std::constructible_from<K, TK &&>
    void put(TK &&key) {
        put(root, std::forward<TK>(key), nullptr, 0);
    }

    void remove(const K &key) {
        remove(root, key, nullptr, 0);
    }

    int greater_count(const K &key) {
        return greater_count(root, key);
    }

private:
    int greater_count(Node *node, const K &key) {
        int index = node->get_index(key);
        if (index < node->keys.size() && node->keys[index] == key) {
            ++index;
        }

        int count = node->keys.size() - index;
        for (int i = index; i < node->children.size(); ++i) {
            count += greater_count(node->children[i], key);
        }
        return count;
    }

    template<typename TK>
        requires std::constructible_from<K, TK &&>
    void put(Node *node, TK &&key, Node *parent, int index_in_parent) {
        int index = node->get_index(key);
        if (index < node->keys.size() && node->keys[index] == key) {
            return;
        }
        if (node->leaf()) {
            node->insert_key(index, std::forward<TK>(key));
        } else {
            // 从根向叶递归插入
            put(node->children[index], std::forward<TK>(key), node, index);
        }

        // 从叶向根递归分裂
        if (overflow(node)) {
            split(node, parent, index_in_parent);
        }
    }

    bool overflow(Node *node) {
        return node->keys.size() > MAX_KEY_NUM;
    }

    bool underflow(Node *node) {
        return node->keys.size() < MIN_KEY_NUM;
    }

    bool enough(Node *node) {
        return node->keys.size() > MIN_KEY_NUM;
    }

    // 将节点一分为三
    void split(Node *node, Node *parent, int index) {
        if (node == root) {
            Node *new_root = new Node;
            new_root->insert_child(index = 0, node);
            root = parent = new_root;
        }

        // 中间元素并入父节点
        parent->insert_key(index, node->keys[min_degree - 1]);
        // 后半部分成为右侧新兄弟，前半部分保持不变
        Node *new_node = node->move_right_half(min_degree); // 新节点和原节点在同一层
        parent->insert_child(index + 1, new_node);
        node->keys.resize(min_degree - 1);
    }

    void remove(Node *node, const K &key, Node *parent, int index_in_parent) {
        int index = node->get_index(key);
        if (node->leaf()) {
            if (index < node->keys.size() && node->keys[index] == key) {
                // 叶节点：直接删除
                node->remove_key(index);
            } else {
                return;
            }
        } else {
            if (index < node->keys.size() && node->keys[index] == key) {
                // 非叶节点：将当前key替换为后继key，再递归删除后继key
                Node *succ = min(node->children[index + 1]);
                node->keys[index] = succ->keys[0];
                remove(node->children[index + 1], succ->keys[0], node, index + 1);
            } else {
                // 递归向下删除
                remove(node->children[index], key, node, index);
            }
        }

        // 递归向上调整
        balance(node, parent, index_in_parent);
    }

    void balance(Node *node, Node *parent, int index_in_parent) {
        if (!underflow(node)) {
            return;
        }

        if (node == root) {
            // 若根节点为空且有孩子，则让孩子成为新根
            if (!node->leaf() && node->keys.empty()) {
                root = node->children[0];
                delete node;
            }
            // 根节点可以无视下溢
            return;
        }

        Node *left = parent->child_left_sibling(index_in_parent);
        Node *right = parent->child_right_sibling(index_in_parent);
        // 左边兄弟富裕，右旋
        if (left != nullptr && enough(left)) {
            right_rotate(parent, index_in_parent - 1);
            return;
        }

        // 右边兄弟富裕，左旋
        if (right != nullptr && enough(right)) {
            left_rotate(parent, index_in_parent);
            return;
        }

        // 两边都不够，合并
        if (left != nullptr) {
            // 与左兄弟合并
            merge(parent, index_in_parent - 1);
        } else {
            // 与右兄弟合并
            merge(parent, index_in_parent);
        }
    }

    void left_rotate(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->children[index + 1];
        left->insert_last_key(node->keys[index]);
        node->keys[index] = right->remove_first_key();
        if (!right->leaf()) {
            left->insert_last_child(right->remove_first_child());
        }
    }

    void right_rotate(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->children[index + 1];
        right->insert_first_key(node->keys[index]);
        node->keys[index] = left->remove_last_key();
        if (!left->leaf()) {
            right->insert_first_child(left->remove_last_child());
        }
    }

    // 把父key、左孩子、右孩子合并
    void merge(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->remove_child(index + 1);
        left->insert_last_key(node->remove_key(index));
        left->append(right);
        delete right;
    }

    Node *min(Node *node) {
        while (!node->leaf()) {
            node = node->children[0];
        }
        return node;
    }
};

template<class K>
class BPlusTree {
    struct Node {
        // 孩子的数量比key的数量多1
        std::vector<K> keys;
        std::vector<Node *> children;
        // 用于叶子节点链表
        Node *next;
        Node *prev;

        explicit Node(Node *next = nullptr, Node *prev = nullptr) : next(next), prev(prev) {
        }

        bool leaf() const {
            return children.empty();
        }

        Node *get(const K &key) {
            if (leaf()) {
                return this;
            }
            return children[get_index(key)]->get(key);
        }

        int get_index(const K &key) const {
            // keys数组为升序，可用二分查找提高效率
            if (leaf()) {
                auto itr = std::lower_bound(keys.begin(), keys.end(), key);
                return itr - keys.begin();
            } else {
                // 非叶子节点的key为副本，即使相等也应该跳过，继续在子树中查找
                auto itr = std::upper_bound(keys.begin(), keys.end(), key);
                return itr - keys.begin();
            }
        }

        Node *move_right_half(int from) {
            Node *new_node = new Node;
            new_node->keys.assign(keys.begin() + from, keys.end());
            keys.resize(from);
            if (!leaf()) {
                new_node->children.assign(children.begin() + from, children.end());
                children.resize(from);
            } else {
                Node *next = this->next;
                new_node->next = next;
                new_node->prev = this;
                this->next = new_node;
                if (next != nullptr) {
                    next->prev = new_node;
                }
            }
            return new_node;
        }

        void append(Node *node) {
            keys.insert(keys.end(), node->keys.begin(), node->keys.end());
            if (leaf()) {
                next = node->next;
                if (next != nullptr) {
                    next->prev = this;
                }
            } else {
                children.insert(children.end(), node->children.begin(), node->children.end());
            }
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_key(int index, TK &&key) {
            keys.insert(keys.begin() + index, std::forward<TK>(key));
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_first_key(TK &&key) {
            insert_key(0, std::forward<TK>(key));
        }

        template<typename TK>
            requires std::constructible_from<K, TK &&>
        void insert_last_key(TK &&key) {
            keys.push_back(std::forward<TK>(key));
        }

        void insert_child(int index, Node *child) {
            children.insert(children.begin() + index, child);
        }

        void insert_first_child(Node *child) {
            insert_child(0, child);
        }

        void insert_last_child(Node *child) {
            children.push_back(child);
        }

        K remove_key(int index) {
            K key = keys[index];
            keys.erase(keys.begin() + index);
            return key;
        }

        K remove_first_key() {
            return remove_key(0);
        }

        K remove_last_key() {
            K key = keys.back();
            keys.pop_back();
            return key;
        }

        Node *remove_child(int index) {
            Node *child = children[index];
            children.erase(children.begin() + index);
            return child;
        }

        Node *remove_first_child() {
            return remove_child(0);
        }

        Node *remove_last_child() {
            return remove_child(children.size() - 1);
        }

        Node *child_left_sibling(int index) {
            return index > 0 ? children[index - 1] : nullptr;
        }

        Node *child_right_sibling(int index) {
            return index < children.size() - 1 ? children[index + 1] : nullptr;
        }
    };

    Node *root;
    const int min_degree;
    const int MAX_KEY_NUM;
    const int MIN_KEY_NUM;

public:
    ~BPlusTree() {
        post_order(root, [](Node *node) { delete node; });
    }

    explicit BPlusTree(int min_degree = 2) : min_degree(min_degree), MAX_KEY_NUM(2 * min_degree - 1),
                                             MIN_KEY_NUM(min_degree - 1) {
        if (min_degree < 2) {
            throw std::invalid_argument("min_degree must be >= 2");
        }
        root = new Node;
    }

    bool contains(const K &key) {
        Node *node = root->get(key);
        int index = node->get_index(key);
        return index < node->keys.size() && node->keys[index] == key;
    }

    template<typename TK>
        requires std::constructible_from<K, TK &&>
    void put(TK &&key) {
        put(root, std::forward<TK>(key), nullptr, 0);
    }

    void remove(const K &key) {
        remove(root, key, nullptr, 0);
    }

    int greater_count(const K &key) {
        Node *cur = root->get(key);
        int index = cur->get_index(key);
        if (index < cur->keys.size() && cur->keys[index] == key) {
            ++index;
        }

        int count = cur->keys.size() - index;
        cur = cur->next;
        while (cur != nullptr) {
            count += cur->keys.size();
            cur = cur->next;
        }
        return count;
    }

private:
    void post_order(Node *node, const std::function<void(Node *)> &func) {
        for (auto child: node->children) {
            post_order(child, func);
        }
        func(node);
    }

    template<typename TK>
        requires std::constructible_from<K, TK &&>
    void put(Node *node, TK &&key, Node *parent, int index_in_parent) {
        int index = node->get_index(key);
        if (index < node->keys.size() && node->keys[index] == key) {
            return;
        }
        if (node->leaf()) {
            node->insert_key(index, std::forward<TK>(key));
        } else {
            // 从根向叶递归插入
            put(node->children[index], std::forward<TK>(key), node, index);
        }

        // 从叶向根递归分裂
        if (overflow(node)) {
            split(node, parent, index_in_parent);
        }
    }

    bool overflow(Node *node) {
        return node->keys.size() > MAX_KEY_NUM;
    }

    bool underflow(Node *node) {
        return node->keys.size() < MIN_KEY_NUM;
    }

    bool enough(Node *node) {
        return node->keys.size() > MIN_KEY_NUM;
    }

    // 将节点一分为三
    void split(Node *node, Node *parent, int index) {
        if (node == root) {
            Node *new_root = new Node;
            new_root->insert_child(index = 0, node);
            root = parent = new_root;
        }

        // 后半部分成为右侧新兄弟，前半部分保持不变
        Node *new_node = node->move_right_half(min_degree); // 新节点和原节点在同一层
        parent->insert_child(index + 1, new_node);
        if (node->leaf()) {
            // 叶子节点，将右侧新兄弟第一个key复制到父节点
            parent->insert_key(index, new_node->keys[0]);
        } else {
            // 非叶子节点，处理方式与B-树相同，中间元素并入父节点
            parent->insert_key(index, node->remove_last_key());
        }
    }

    void remove(Node *node, const K &key, Node *parent, int index_in_parent) {
        int index = node->get_index(key);
        if (node->leaf()) {
            if (index < node->keys.size() && node->keys[index] == key) {
                node->remove_key(index);
            } else {
                return;
            }
        } else {
            // 只能在叶子节点中删除
            remove(node->children[index], key, node, index);
        }

        // 递归向上调整
        balance(node, parent, index_in_parent);
    }

    void balance(Node *node, Node *parent, int index_in_parent) {
        if (!underflow(node)) {
            return;
        }

        if (node == root) {
            // 若根节点为空且有孩子，则让孩子成为新根
            if (!node->leaf() && node->keys.empty()) {
                root = node->children[0];
                delete node;
            }
            // 根节点可以无视下溢
            return;
        }

        Node *left = parent->child_left_sibling(index_in_parent);
        Node *right = parent->child_right_sibling(index_in_parent);
        // 左边兄弟富裕，右旋
        if (left != nullptr && enough(left)) {
            right_rotate(parent, index_in_parent - 1);
            return;
        }

        // 右边兄弟富裕，左旋
        if (right != nullptr && enough(right)) {
            left_rotate(parent, index_in_parent);
            return;
        }

        // 两边都不够，合并
        if (left != nullptr) {
            // 与左兄弟合并
            merge(parent, index_in_parent - 1);
        } else {
            // 与右兄弟合并
            merge(parent, index_in_parent);
        }
    }

    void left_rotate(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->children[index + 1];
        if (left->leaf()) {
            // 涉及叶子节点
            // 将右孩子的第一个key挪到左孩子，父节点的key更新为右孩子第一个key
            left->insert_last_key(right->remove_first_key());
            node->keys[index] = right->keys[0];
        } else {
            // 不涉及叶子节点，处理方式与B-树相同
            left->insert_last_key(node->keys[index]);
            node->keys[index] = right->remove_first_key();
            left->insert_last_child(right->remove_first_child());
        }
    }

    void right_rotate(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->children[index + 1];
        if (left->leaf()) {
            // 涉及叶子节点
            // 将左孩子的最后一个key挪到右孩子，父节点的key更新为右孩子第一个key
            right->insert_first_key(left->remove_last_key());
            node->keys[index] = right->keys[0];
        } else {
            // 不涉及叶子节点，处理方式与B-树相同
            right->insert_first_key(node->keys[index]);
            node->keys[index] = left->remove_last_key();
            right->insert_first_child(left->remove_last_child());
        }
    }

    // 把父key、左孩子、右孩子合并
    void merge(Node *node, int index) {
        Node *left = node->children[index];
        Node *right = node->remove_child(index + 1);
        if (left->leaf()) {
            // 涉及叶子节点
            // 父节点的key直接删除即可，无需并入左孩子
            node->remove_key(index);
        } else {
            // 不涉及叶子节点，处理方式与B-树相同
            left->insert_last_key(node->remove_key(index));
        }
        left->append(right);
        delete right;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N;
    cin >> N;

    BPlusTree<int> set(1000);
    int op, x;
    while (N--) {
        cin >> op >> x;
        switch (op) {
            case 1:
                set.put(x);
                break;
            case 2:
                cout << !set.contains(x) << '\n';
                break;
            case 3:
                set.remove(x);
                break;
            case 4:
                cout << set.greater_count(x) << '\n';
                break;
            default:
                break;
        }
    }
    return 0;
}
