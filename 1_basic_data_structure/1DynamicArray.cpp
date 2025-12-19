//
// Created by xyx on 2025/9/5.
//
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class DynamicArray {
    int m_capacity = 10;
    int m_size = 0;
    int *array = new int[m_capacity];

public:
    DynamicArray() = default;

    ~DynamicArray() {
        delete[] array;
    }

    void append(int value) {
        if (m_size >= m_capacity) {
            grow();
        }
        array[m_size++] = value;
    }

    void set(int index, int value) {
        if (index < 0 || index >= m_size) {
            throw runtime_error("index out of range");
        }
        array[index] = value;
    }

    void erase(int index) {
        if (index < 0 || index >= m_size) {
            throw runtime_error("index out of range");
        }
        memmove(array + index, array + index + 1, (m_size - index - 1) * sizeof(int));
        m_size--;
        if (m_size < m_capacity / 2) {
            shrink();
        }
    }

    int get(int index) {
        if (index < 0 || index >= m_size) {
            throw runtime_error("index out of range");
        }

        return array[index];
    }

    int size() {
        return m_size;
    }

    int capacity() {
        return m_capacity;
    }

    void print() {
        for (int i = 0; i < m_size; i++) {
            cout << array[i];
            if (i < m_size - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }

private:
    void grow() {
        int *newArr = new int[m_capacity * 2];
        memcpy(newArr, array, m_size * sizeof(int));
        delete[] array;
        array = newArr;
        m_capacity *= 2;
    }

    void shrink() {
        int *newArr = new int[m_capacity / 2];
        memcpy(newArr, array, m_size * sizeof(int));
        delete[] array;
        array = newArr;
        m_capacity /= 2;
    }
};

int main() {
    string instruction;
    DynamicArray arr;
    while (cin >> instruction) {
        int x, y;
        if (instruction == "print") {
            arr.print();
        } else if (instruction == "size") {
            cout << arr.size() << endl;
        } else if (instruction == "capacity") {
            cout << arr.capacity() << endl;
        } else if (instruction == "append") {
            cin >> x;
            arr.append(x);
        } else if (instruction == "set") {
            cin >> x >> y;
            arr.set(x, y);
        } else if (instruction == "erase") {
            cin >> x;
            arr.erase(x);
        } else if (instruction == "get") {
            cin >> x;
            cout << arr.get(x) << endl;
        }
    }
    return 0;
}