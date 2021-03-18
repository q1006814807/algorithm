#include<iostream>
#include<vector>
using namespace std;

int binarySearch(vector<int> nums, int target, bool isLowerBound = true) {
    if (isLowerBound) return binarySearch_lowerBound(nums, target);
    return binarySearch_upperBound(nums, target);
}

// 第一次出现的target位置
int binarySearch_lowerBound(vector<int> nums, int target) {
    int left = -1;
    int right = nums.size();

    while (left+1 != right) {
        int mid = (left + right) / 2;
        if (nums[mid] >= target) {
            right = mid;
        }
        else left = mid;
    }

    return right;
}

// 最后一次出现的target位置
int binarySearch_upperBound(vector<int> nums, int target) {
    int left = -1;
    int right = nums.size();

    while (left+1 != right) {
        int mid = (left + right) / 2;
        if (nums[mid] > target) {
            right = mid;
        }
        else left = mid;
    }

    return left;
}