#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>

#include "DataLoader.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Property.h"

struct BenchmarkPoint {
    int size;
    long long merge;
    long long quick;
};

struct PreviewRow {
    double price;
    int beds;
    int baths;
    double houseSize;
    std::string city;
    std::string state;
};

void printResults(const std::vector<Property>& arr, int count = 5) {
    std::cout << "--- Top " << count << " Results ---\n";
    for (int i = 0; i < count && i < static_cast<int>(arr.size()); ++i) {
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
        case SortAttribute::HOUSE_SIZE: return "House Size";
        default: return "Unknown";
    }
}

std::string escapeJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c;
        }
    }
    return out;
}

void capturePreviewRows(const std::vector<Property>& sortedData, std::vector<PreviewRow>& previewRows) {
    previewRows.clear();

    const int previewCount = std::min(5, static_cast<int>(sortedData.size()));
    for (int i = 0; i < previewCount; ++i) {
        const Property& p = sortedData[i];
        previewRows.push_back({
            p.price,
            p.beds,
            p.baths,
            p.house_size,
            p.city,
            p.state
        });
    }
}

void writeBenchmarkJson(
    const std::string& filename,
    const std::map<std::string, std::map<std::string, std::vector<BenchmarkPoint>>>& benchmarkData,
    const std::vector<PreviewRow>& previewRows
) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return;
    }

    const std::vector<std::string> attributes = { "Price", "Beds", "Baths", "House Size" };
    const std::vector<std::string> scenarios = { "randomized", "worstCase" };

    out << "{\n";

    for (size_t i = 0; i < attributes.size(); ++i) {
        const std::string& attr = attributes[i];
        out << "  \"" << attr << "\": {\n";

        for (size_t j = 0; j < scenarios.size(); ++j) {
            const std::string& scenario = scenarios[j];
            out << "    \"" << scenario << "\": [\n";

            auto attrIt = benchmarkData.find(attr);
            if (attrIt != benchmarkData.end()) {
                auto scenarioIt = attrIt->second.find(scenario);
                if (scenarioIt != attrIt->second.end()) {
                    const auto& rows = scenarioIt->second;

                    for (size_t k = 0; k < rows.size(); ++k) {
                        out << "      { "
                            << "\"size\": " << rows[k].size << ", "
                            << "\"merge\": " << rows[k].merge << ", "
                            << "\"quick\": " << rows[k].quick << " }";
                        if (k + 1 < rows.size()) {
                            out << ",";
                        }
                        out << "\n";
                    }
                }
            }

            out << "    ]";
            if (j + 1 < scenarios.size()) {
                out << ",";
            }
            out << "\n";
        }

        out << "  },\n";
    }

    out << "  \"preview\": [\n";
    for (size_t i = 0; i < previewRows.size(); ++i) {
        const PreviewRow& p = previewRows[i];
        out << "    {\n";
        out << "      \"price\": " << p.price << ",\n";
        out << "      \"beds\": " << p.beds << ",\n";
        out << "      \"baths\": " << p.baths << ",\n";
        out << "      \"houseSize\": " << p.houseSize << ",\n";
        out << "      \"city\": \"" << escapeJson(p.city) << "\",\n";
        out << "      \"state\": \"" << escapeJson(p.state) << "\"\n";
        out << "    }";
        if (i + 1 < previewRows.size()) {
            out << ",";
        }
        out << "\n";
    }
    out << "  ]\n";

    out << "}\n";
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

    std::map<std::string, std::map<std::string, std::vector<BenchmarkPoint>>> exportedBenchmarks;
    std::vector<PreviewRow> exportedPreview;

    int choice = 0;
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

            std::vector<Property> mergeData = originalData;
            std::cout << "Running Merge Sort...\n";
            auto start1 = std::chrono::high_resolution_clock::now();
            mergeSort(mergeData, currentAttribute);
            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
            std::cout << "Merge Sort completed in: " << duration1.count() << " ms\n";

            std::vector<Property> quickData = originalData;
            std::cout << "Running Iterative Quick Sort...\n";
            auto start2 = std::chrono::high_resolution_clock::now();
            quickSort(quickData, currentAttribute);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
            std::cout << "Quick Sort completed in: " << duration2.count() << " ms\n";

            exportedBenchmarks[getAttributeName(currentAttribute)]["randomized"].push_back({
                static_cast<int>(originalData.size()),
                duration1.count(),
                duration2.count()
            });

            capturePreviewRows(quickData, exportedPreview);
            printResults(quickData);

        } else if (choice == 2) {
            int subsetSize = 100000;
            if (static_cast<int>(originalData.size()) < subsetSize) {
                subsetSize = static_cast<int>(originalData.size());
            }

            std::cout << "\n--- Trial 2: Pre-Sorted Worst-Case Scenario ---\n";
            std::cout << "Uses a subset of " << subsetSize << " records.\n\n";

            std::vector<Property> subset(originalData.begin(), originalData.begin() + subsetSize);
            mergeSort(subset, currentAttribute);

            std::vector<Property> mergeData = subset;
            std::cout << "Running Merge Sort on presorted data...\n";
            auto start1 = std::chrono::high_resolution_clock::now();
            mergeSort(mergeData, currentAttribute);
            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
            std::cout << "Merge Sort completed in: " << duration1.count() << " ms\n";

            std::vector<Property> quickData = subset;
            std::cout << "Running Quick Sort on presorted data...\n";
            auto start2 = std::chrono::high_resolution_clock::now();
            quickSort(quickData, currentAttribute);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
            std::cout << "Quick Sort completed in: " << duration2.count() << " ms\n";

            exportedBenchmarks[getAttributeName(currentAttribute)]["worstCase"].push_back({
                subsetSize,
                duration1.count(),
                duration2.count()
            });

            capturePreviewRows(quickData, exportedPreview);
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
                default:
                    currentAttribute = SortAttribute::PRICE;
                    std::cout << "Invalid choice. Defaulting to Price.\n";
                    break;
            }

            std::cout << "Attribute updated to: " << getAttributeName(currentAttribute) << "\n\n";
        } else if (choice != 4) {
            std::cout << "Invalid menu choice. Please try again.\n\n";
        }
    }

    writeBenchmarkJson("../frontend/my-app/public/data/benchmark.json", exportedBenchmarks, exportedPreview);
    std::cout << "benchmark.json written successfully.\n";
    std::cout << "Exiting program. Goodbye!\n";

    return 0;
}
