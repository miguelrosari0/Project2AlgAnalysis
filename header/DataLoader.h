//
// Created by Miguel Rosario on 3/17/26.
//
#include "DataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

std::vector<Property> loadData(const std::string& filename) {
    std::vector<Property> properties;
    std::ifstream file(filename);
    std::string line, word;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << "\n";
        return properties;
    }

    // Skip the header row
    std::getline(file, line);

    // Read line by line
    while (std::getline(file, line)) {
        std::stringstream s(line);
        std::vector<std::string> row;

        // Break line into columns by commas
        while (std::getline(s, word, ',')) {
            row.push_back(word);
        }

        try {
            // Extract strings
            std::string city = row.size() > 7 ? row[7] : "Unknown";
            std::string state = row.size() > 8 ? row[8] : "Unknown";

            // Extract and convert numbers
            double price = (row.size() > 2 && !row[2].empty()) ? std::stod(row[2]) : 0.0;
            int beds = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : 0;
            int baths = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : 0;
            double house_size = (row.size() > 10 && !row[10].empty()) ? std::stod(row[10]) : 0.0;

            // Only add properties that have a valid price
            if (price > 0.0) {
                properties.emplace_back(price, house_size, beds, baths, city, state);
            }
        } catch (const std::exception& e) {
            // Skip rows with bad formatting
            continue;
        }
    }

    file.close();
    return properties;
}
