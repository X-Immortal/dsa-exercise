//
// Created by xyx on 2025/10/28.
//
#include <iostream>
#include <vector>

using namespace std;

// 选定一个 pivot 并将 left 到 right 之间的元素通过 pivot 划分成两部分，然后返回 pivot 的下标
int partition (vector<int>& nums, int left, int right) {
    if (left > right) {
        return -1;
    }
    int pivot = nums[left];
    while (left < right) {
        while (left < right && nums[right] >= pivot) {
            right--;
        }
        nums[left] = nums[right];

        while (left < right && nums[left] < pivot) {
            left++;
        }
        nums[right] = nums[left];
    }
    nums[left] = pivot;
    return left;
}

// 将 nums 通过_partition 划分成两部分，对每个部分调用_quick_sort
void quick_sort (vector<int>& nums, int left, int right) {
    //write ur code here.
    if (left >= right) {
        return;
    }

    int pivot_index = partition(nums, left, right);
    quick_sort(nums, left, pivot_index - 1);
    quick_sort(nums, pivot_index + 1, right);
}

void QuickSort (vector<int>& nums) {
    quick_sort (nums, 0, nums.size ()-1);
}

int main() {
    int N;
    cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }
    QuickSort(a);
    for (int i = 0; i < N; i++) {
        cout << a[i];
        if (i < N - 1) {
            cout << " ";
        }
    }
    cout << endl;
    return 0;
}