//
// Created by Miguel Rosario on 3/17/26.
//
#include "MergeSort.h"

bool compareProperties(const Property& a, const Property& b, SortAttribute sortBy) {
    switch (sortBy) {
        case SortAttribute::PRICE:      return a.price <= b.price;
        case SortAttribute::BEDS:       return a.beds <= b.beds;
        case SortAttribute::BATHS:      return a.baths <= b.baths;
        case SortAttribute::HOUSE_SIZE: return a.house_size <= b.house_size;
        default:                        return false;
    }
}

void merge(std::vector<Property>& arr, std::vector<Property>& temp, int left, int mid, int right, SortAttribute sortBy) {
    int i = left;    //index for left subarray
    int j = mid + 1;    //index for right subarray
    int k = left;       //starting index to be sorted in temp array

    // compares elements from two halves and place the lesser into temp array
    while (i <= mid && j <= right) {
        if (compareProperties(arr[i], arr[j], sortBy)) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    // copy remaining elements from the left half
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // copy remaining elements from the right half
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // copies the sorted elements back into the original array
    for (i = left; i <= right; ++i) {
        arr[i] = temp[i];
    }
}

void mergeSortHelper(std::vector<Property>& arr, std::vector<Property>& temp, int left, int right, SortAttribute sortBy) {
    if (left < right) {
        // Find the midpoint
        int mid = left + (right - left) / 2;

        // recursively sort the first and second halves
        mergeSortHelper(arr, temp, left, mid, sortBy);
        mergeSortHelper(arr, temp, mid + 1, right, sortBy);

        // merges the sorted halves
        merge(arr, temp, left, mid, right, sortBy);
    }
}

void mergeSort(std::vector<Property>& arr, SortAttribute sortBy) {
    if (arr.empty()) return;
    std::vector<Property> temp = arr;
    mergeSortHelper(arr, temp, 0, arr.size() - 1, sortBy);
}