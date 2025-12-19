//
// Created by xyx on 2025/12/14.
//
#include <iostream>
#include <vector>

using namespace std;

class DisjointSet {
    int *parent; // 数组中的值为父节点编号
    int parent_size;
    int *size; // 存储每个子集的大小
    int size_size;

public:
    DisjointSet(int size) {
        parent = new int[size];
        this->size = new int[size](1);
        // 初始时，所有元素都单独成集合
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    ~DisjointSet() {
        delete[] parent;
    }

    int find(int x) {
        // 指向自身的元素为集合的“老大”，其下标为集合的编号
        if (x == parent[x]) {
            return x;
        }
        // 路径压缩优化
        // 每次查找完一个元素，就让该元素直接指向集合编号，从而压缩了查找路径
        // 此处还一次性把路径上的所有元素都压缩了
        return parent[x] = find(parent[x]);
    }

    void unite(int x, int y) {
        // 合并时只需让一个“老大”指向另一个“老大”
        x = find(x);
        y = find(y);

        // unite by size优化：让小集合的“老大”指向大集合的“老大”
        // 这样做可以缩短整体的查找路径
        if (size[x] < size[y]) {
            std::swap(x, y);
        }
        parent[y] = x;
        size[x] += size[y];
    }

    bool united(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N, M;
    cin >> N >> M;
    DisjointSet set(M + 1);

    int op, x, y;
    while (N--) {
        cin >> op >> x >> y;
        switch (op) {
            case 1:
                set.unite(x, y);
                break;
            case 2:
                cout << set.united(x, y) << '\n';
                break;
            default:
                break;
        }
    }

    return 0;
}