#include<iostream>
#include<vector>
#include<time.h>
#include<math.h>
using namespace std;


class QuickSort {
private:
    
    int Recursion_threshold = 20;
    static const int Inersetion_threshold = 16;

    void quickSort(vector<int>& nums, int left, int right) {
        Recursion_threshold = (int)log2(nums.size()) + 1;
        _quickSort(nums, left, right, 0);
    }

    void _quickSort(vector<int>& nums, int left, int right, int depth) {
        if (right-left < Inersetion_threshold) {
            if (left > 0) return insSort_unguarded(nums, left-1, right);
            else          return insSort(nums, left, right);
        }
            
        if (depth > Recursion_threshold) 
            return heapSort(nums, left, right);

        int mid = partition(nums, left, right);
        _quickSort(nums, left,  mid-1, depth+1);
        _quickSort(nums, mid+1, right, depth+1);
    }

    static int partition(vector<int>& nums, int left, int right) {
        selectPivotMedian(nums, left, right);

        int pivot = nums[left];

        while (left < right) {
            while (left < right && nums[right] > pivot) --right;
            nums[left]  = nums[right];
            while (left < right && nums[left] <= pivot) ++left;
            nums[right] = nums[left];
        }

        nums[left] = pivot;
        return left;
    }    


    static void insSort(vector<int>& nums, int left, int right) {
        for (int i = left+1; i <= right; ++i) {
            int num = nums[i];
            while (i > left && num < nums[i-1]) {
                nums[i] = nums[i-1];
                --i;
            }
            nums[i] = num;
        }
    }

    static void insSort_unguarded(vector<int>& nums, int left, int right) {
        for (int i = left+1; i <= right; ++i) {
            int num = nums[i];
            while (num < nums[i-1]) {
                nums[i] = nums[i-1];
                --i;
            }
            nums[i] = num;
        }
    }

    static void selectPivotMedian(vector<int>& nums, int left, int right) {
        int mid = (left + right) >> 1;

        if (nums[left] > nums[mid])   swap(nums[left], nums[mid]);
        if (nums[mid]  > nums[right]) swap(nums[mid], nums[right]);
        if (nums[left] < nums[mid])   swap(nums[left], nums[mid]);
    }

    static void heapSort(vector<int>& nums, int left, int right) {
        if (left >= right) return;

        int mid = left + ((right - left - 1) >> 1);
        for (int i = mid; i >= left; --i) {
            adjustMax(nums, i, right+1, left);
        }

        for (int end = right+1; end > left+1; --end) {
            popSort(nums, left, end);
        }
    }

    static void adjustMax(vector<int>& nums, int begin, int end, int left) {
        while (1) {
            int lc = begin * 2 + 1 - left;
            int rc = begin * 2 + 2 - left;
            if (lc >= end) return;
            if (rc >= end) rc = lc;

            int maxc = nums[lc] > nums[rc] ? lc : rc;

            if (nums[begin] >= nums[maxc]) return;
            swap(nums[begin], nums[maxc]);
            begin = maxc;
        }
    }

    static void popSort(vector<int>& nums, int left, int end) {
        int returner = nums[left];
        int replacer = nums[end-1];

        nums[left] = replacer;
        adjustMax(nums, left, end-1, left);
        nums[end-1] = returner;
    }


public:
    QuickSort(vector<int>& nums, int begin, int end) {
        QuickSort::quickSort(nums, begin, end-1);
    }
};

int main() {
    vector<int> nums = {2,5,7,1,4,6,1,2,3};
    QuickSort (nums, 0, nums.size());

    for (int num : nums) 
        cout << num << ", ";

    cout << endl;

    system("pause");
}