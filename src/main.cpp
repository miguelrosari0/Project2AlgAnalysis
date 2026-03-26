#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "DataLoader.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Property.h"

void printResults(const std::vector<Property>& arr, int count = 5) {
    std::cout << "--- Top " << count << " Results ---\n";
    for (int i = 0; i < count && i < arr.size(); ++i) {
        std::cout << "$" << std::fixed << std::setprecision(2) << arr[i].price
                  << " | " << arr[i].beds << " Beds | " << arr[i].baths
                  << " Baths | " << arr[i].house_size << " sqft | "
                  << arr[i].city << ", " << arr[i].state << "\n";
    }
    std::cout << "------------------------\n\n";
}

std::string getAttributeName(SortAttribute attr) {
    switch (attr) {
        case SortAttribute::PRICE: return "Price";
        case SortAttribute::BEDS: return "Beds";
        case SortAttribute::BATHS: return "Baths";
        case SortAttribute::HOUSE_SIZE: return "House Size (sqft)";
        default: return "Unknown";
    }
}

int main() {
    std::string filename;
    std::getline(std::cin, filename);
    std::cout << "Loading dataset into memory. Please wait.\n";
    std::vector<Property> originalData = loadData(filename);

    if (originalData.empty()) {
        std::cerr << "Failed to load data. Please check the file path in main.cpp.\n";
        return 1;
    }

    std::cout << "Successfully loaded " << originalData.size() << " properties!\n\n";

    int choice = 0;
    // sets default sort attribute
    SortAttribute currentAttribute = SortAttribute::PRICE;

    while (choice != 4) {
        std::cout << "==================================================\n";
        std::cout << "Sorting Algorithm Analyzer\n";
        std::cout << "==================================================\n";
        std::cout << "Dataset Size: " << originalData.size() << " records\n";
        std::cout << "Currently Sorting By: [" << getAttributeName(currentAttribute) << "]\n";
        std::cout << "Select a Testing Scenario:\n";
        std::cout << "[1] Trial 1: Compare on Randomized Data\n";
        std::cout << "[2] Trial 2: Compare on Pre-Sorted Data (Worst-Case)\n";
        std::cout << "[3] Choose What Attribute to Sort By\n";
        std::cout << "[4] Exit Program\n";
        std::cout << "Enter your choice (1-4): > ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n--- Trial 1: Randomized Data (" << originalData.size() << " records) ---\n";

            // MergeSort test
            std::vector<Property> mergeData = originalData;
            std::cout << "Running Merge Sort...\n";
            auto start1 = std::chrono::high_resolution_clock::now();
            mergeSort(mergeData, currentAttribute);
            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
            std::cout << "Merge Sort completed in: " << duration1.count() << " ms\n";

            //  QuickSort test
            std::vector<Property> quickData = originalData;
            std::cout << "Running Iterative Quick Sort...\n";
            auto start2 = std::chrono::high_resolution_clock::now();
            quickSort(quickData, currentAttribute);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
            std::cout << "Quick Sort completed in: " << duration2.count() << " ms\n";

            printResults(quickData);

        } else if (choice == 2) {
            // For O(N^2) worst-case, 2.2 million rows would take very long to compute
            // Uses smaller dataset for demonstration purposes
            int subsetSize = 100000;
            if (originalData.size() < subsetSize) subsetSize = originalData.size();

            std::cout << "\n--- Trial 2: Pre-Sorted Worst-Case Scenario ---\n";
            std::cout << "Uses a subset of " << subsetSize << " records.\n\n";

            // Grab a subset and pre-sort it using Merge Sort to set up the worst-case scenario
            std::vector<Property> subset(originalData.begin(), originalData.begin() + subsetSize);
            mergeSort(subset, currentAttribute);

            // MergeSort (Sorted)
            std::vector<Property> mergeData = subset;
            std::cout << "Running Merge Sort on presorted data...\n";
            auto start1 = std::chrono::high_resolution_clock::now();
            mergeSort(mergeData, currentAttribute);
            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
            std::cout << "Merge Sort completed in: " << duration1.count() << " ms\n";

            // Quick Sort (Sorted)
            std::vector<Property> quickData = subset;
            std::cout << "Running Quick Sort on presorted data...\n";
            auto start2 = std::chrono::high_resolution_clock::now();
            quickSort(quickData, currentAttribute);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
            std::cout << "Quick Sort completed in: " << duration2.count() << " ms\n";

            printResults(quickData);

        } else if (choice == 3) {
            int attrChoice = 0;
            std::cout << "\nSelect Attribute to Sort By:\n";
            std::cout << "[1] Price\n[2] Beds\n[3] Baths\n[4] House Size\n> ";
            std::cin >> attrChoice;

            switch (attrChoice) {
                case 1: currentAttribute = SortAttribute::PRICE; break;
                case 2: currentAttribute = SortAttribute::BEDS; break;
                case 3: currentAttribute = SortAttribute::BATHS; break;
                case 4: currentAttribute = SortAttribute::HOUSE_SIZE; break;
                default: std::cout << "Invalid choice. Defaulting to Price.\n";
            }
            std::cout << "Attribute updated to: " << getAttributeName(currentAttribute) << "\n\n";
        }
    }

    std::cout << "Exiting program. Goodbye!\n";
    return 0;
}
