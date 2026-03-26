//
// Created by Miguel Rosario on 3/17/26.
//
#include "QuickSort.h"
#include <stack>
#include <utility>

static bool comparePropertiesQS(const Property& a, const Property& b, SortAttribute sortBy) {
    switch (sortBy) {
        case SortAttribute::PRICE:      return a.price <= b.price;
        case SortAttribute::BEDS:       return a.beds <= b.beds;
        case SortAttribute::BATHS:      return a.baths <= b.baths;
        case SortAttribute::HOUSE_SIZE: return a.house_size <= b.house_size;
        default:                        return false;
    }
}

static int partition(std::vector<Property>& arr, int low, int high, SortAttribute sortBy) {
    Property pivot = arr[high]; // uses the last element as the pivot
    int i = (low - 1);          // index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        // if the curr element is smaller/equal to the pivot
        if (comparePropertiesQS(arr[j], pivot, sortBy)) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    //  move pivot in its final sorted position
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<Property>& arr, SortAttribute sortBy) {
    if (arr.empty()) return;
    std::stack<std::pair<int, int>> stack;
    stack.push({0, arr.size() - 1});

    // Keep popping from the stack while it is not empty
    while (!stack.empty()) {
        int low = stack.top().first;
        int high = stack.top().second;
        stack.pop();

        // Set pivot element at its correct position in sorted array
        int pivot_index = partition(arr, low, high, sortBy);

        // If there are elements on the left side of the pivot, push left side to stack
        if (pivot_index - 1 > low) {
            stack.push({low, pivot_index - 1});
        }

        // If there are elements on the right side of the pivot, push right side to stack
        if (pivot_index + 1 < high) {
            stack.push({pivot_index + 1, high});
        }
    }
}