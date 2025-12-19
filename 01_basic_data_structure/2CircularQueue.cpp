//
// Created by xyx on 2025/9/7.
//
#include <iostream>
#include <string>

using namespace std;

class CircularQueue {
    int *data;
    int m_front;
    int m_rear;
    int capacity;
    int size;

public:
    explicit CircularQueue(int capacity) : capacity(capacity) {
        if (capacity <= 0) {
            throw runtime_error("Capacity must be positive.");
        }
        data = new int[capacity];
        m_front = 0;
        m_rear = -1;
        size = 0;
    }

    ~CircularQueue() {
        delete[] data;
    }

    bool enqueue(int value) {
        if (isFull()) {
            return false;
        }
        m_rear = (m_rear + 1) % capacity;
        data[m_rear] = value;
        size++;
        return true;
    }

    bool dequeue() {
        if (isEmpty()) {
            return false;
        }
        m_front = (m_front + 1) % capacity;
        size--;
        return true;
    }

    int front() {
        if (isEmpty()) {
            return -1;
        }
        return data[m_front];
    }

    int rear() {
        if (isEmpty()) {
            return -1;
        }
        return data[m_rear];
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == capacity;
    }
};


int main() {
    int capacity;
    cin >> capacity;
    CircularQueue queue(capacity);
    string op;
    while (cin >> op) {
        if (op == "dequeue") {
            if (!queue.dequeue()) {
                cout << "Underflow" << endl;
            }
        } else if (op == "front") {
            cout << queue.front() << endl;
        } else if (op == "rear") {
            cout << queue.rear() << endl;
        } else if (op == "isEmpty") {
            if (queue.isEmpty()) {
                cout << "True" << endl;
            } else {
                cout << "False" << endl;
            }
        } else if (op == "isFull") {
            if (queue.isFull()) {
                cout << "True" << endl;
            } else {
                cout << "False" << endl;
            }
        } else {
            int value;
            cin >> value;
            if (!queue.enqueue(value)) {
                cout << "Overflow" << endl;
            }
        }
    }
    return 0;
}

