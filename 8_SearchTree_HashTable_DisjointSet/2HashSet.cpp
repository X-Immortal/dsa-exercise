//
// Created by xyx on 2025/12/12.
//
#include <iostream>
#include <functional>

using namespace std;

template<class K>
concept HashKey = requires(K &a, K &b) {
    { a == b } -> std::convertible_to<bool>;
    { std::hash<K>()(a) } -> std::convertible_to<std::size_t>;
};

template<HashKey K>
class HashTable {
    struct Entry {
        int hash; // 哈希码
        K key;
        Entry *next;
        Entry *input_next;

        template<class T>
            requires std::constructible_from<K, T&&>
        Entry(int hash, T &&key) :
            hash(hash), key(std::forward<T>(key)), next(nullptr), input_next(nullptr) {}
    };

    int m_size = 0;
    int capacity = 16; // 数组长度为2的幂，可以将一些运算转化为位运算，提高效率
    Entry **table = new Entry *[capacity](nullptr);
    Entry *head = nullptr;
    Entry *tail = nullptr;
    const double load_factor = 0.75; // 负载因子
    int threshold = capacity * load_factor; // 扩容阈值

public:
    HashTable() = default;

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Entry *entry = table[i];
            while (entry != nullptr) {
                Entry *next = entry->next;
                delete entry;
                entry = next;
            }
        }
    }

    template<class T>
        requires std::constructible_from<K, T&&>
    void put(T &&key) {
        return put(std::hash<K>()(key), std::forward<T>(key));
    }

    void print() {
        Entry *entry = head;
        while (entry != nullptr) {
            cout << entry->key;
            if (entry != tail) {
                cout << " ";
            }
            entry = entry->input_next;
        }
        cout << endl;
    }

private:
    int hash(const K &key) {
        int hash = std::hash<K>()(key);
        // 高低位异或，使低位部分包含高位信息，减少数组长度很小时发生冲突的几率
        return hash ^ (hash >> 16);
    }

    template<class T>
        requires std::constructible_from<K, T&&>
    void put(int hash, T &&key) {
        int index = hash & capacity - 1;
        Entry *new_entry = nullptr;
        if (table[index] == nullptr) {
            new_entry = table[index] = new Entry(hash, std::forward<T>(key));
        } else {
            Entry *entry = table[index];
            while (true) {
                if (entry->key == key) {
                    return;
                }
                if (entry->next == nullptr) {
                    new_entry = entry->next = new Entry(hash, std::forward<T>(key));
                    break;
                }
                entry = entry->next;
            }
        }

        if (head == nullptr) {
            tail = head = new_entry;
        } else {
            tail->input_next = new_entry;
            tail = new_entry;
        }

        m_size++;
        if (m_size > threshold) {
            expand();
        }
    }

    void expand() {
        Entry **new_table = new Entry *[capacity << 1](nullptr);
        for (int i = 0; i < capacity; i++) {
            Entry *entry = table[i];
            Entry *list1 = nullptr, *list2 = nullptr;
            Entry *p1 = nullptr, *p2 = nullptr;
            while (entry != nullptr) {
                if ((entry->hash & capacity) == 0) {
                    if (list1 == nullptr) {
                        list1 = entry;
                    } else {
                        p1->next = entry;
                    }
                    p1 = entry;
                } else {
                    if (list2 == nullptr) {
                        list2 = entry;
                    } else {
                        p2->next = entry;
                    }
                    p2 = entry;
                }
                entry = entry->next;
            }

            if (p1 != nullptr) {
                p1->next = nullptr;
                new_table[i] = list1;
            }
            if (p2 != nullptr) {
                p2->next = nullptr;
                new_table[i | capacity] = list2;
            }
        }

        delete[] table;
        table = new_table;
        capacity <<= 1;
        threshold = capacity * load_factor;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int T, n, a;
    cin >> T;
    while (T--) {
        cin >> n;
        HashTable<int> set;
        while (n--) {
            cin >> a;
            set.put(a);
        }
        set.print();
    }
    return 0;
}