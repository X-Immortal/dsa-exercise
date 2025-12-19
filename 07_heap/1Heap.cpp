//
// Created by xyx on 2025/12/9.
//
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MinHeap {
    vector<int *> array;
    vector<int *> push_seq;

public:
    MinHeap() = default;

    ~MinHeap() {
        for (int *p : push_seq) {
            delete p;
        }
    }

    int pop() {
        if (isEmpty()) {
            throw std::out_of_range("Heap is empty");
        }
        swap(0, array.size() - 1);
        int ret = *array.back();
        array.pop_back();
        down(0);
        return ret;
    }

    int top() const {
        if (isEmpty()) {
            throw std::out_of_range("Heap is empty");
        }
        return *array[0];
    }

    void push(int value) {
        int *p = new int(value);
        array.push_back(p);
        up(array.size()- 1);
        push_seq.push_back(p);
    }

    bool isEmpty() const {
        return array.empty();
    }

    int size() const {
        return array.size();
    }

    void decrease(int index, int k) {
        *push_seq[index - 1] = k;
        up(find(array.begin(), array.end(), push_seq[index - 1]) - array.begin());
    }

private:
    void up(int index) {
        for (int parent_i = parent(index);
             parent_i >= 0 && *array[parent_i] > *array[index];
             index = parent_i, parent_i = parent(index)) {
            swap(parent_i, index);
        }
    }

    void down(int index) {
        int left_i, right_i;
        int max_i = index;
        while (true) {
            left_i = left(index);
            right_i = right(index);
            if (left_i >= 0 && *array[left_i] < *array[max_i]) {
                max_i = left_i;
            }
            if (right_i >= 0 && *array[right_i] < *array[max_i]) {
                max_i = right_i;
            }
            if (max_i == index) {
                break;
            }
            swap(index, max_i);
            index = max_i;
        }
    }

    void swap(int i, int j) {
        int *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    int parent(int index) const {
        return index == 0 ? -1 : (index - 1) / 2;
    }

    int left(int index) const {
        int ret = index * 2 + 1;
        return ret >= array.size()? -1 : ret;
    }

    int right(int index) const {
        int ret = index * 2 + 2;
        return ret >= array.size() ? -1 : ret;
    }
};

int main() {
    string command;
    MinHeap heap;
    while (cin >> command) {
        if (command == "push") {
            int x;
            cin >> x;
            heap.push(x);
        } else if (command == "pop") {
            heap.pop();
        } else if (command == "top") {
            cout << heap.top() << endl;
        } else if (command == "size") {
            cout << heap.size() << endl;
        }else if (command == "decrease") {
            int i, k;
            cin >> i >> k;
            heap.decrease(i, k);
        }
    }

    return 0;
}
