//
// Created by Miguel Rosario on 3/17/26.
//
#ifndef PROPERTY_H
#define PROPERTY_H
#pragma once

#include <string>
enum class SortAttribute {
    PRICE,
    BEDS,
    BATHS,
    HOUSE_SIZE
};

struct Property {
    double price;
    double house_size;
    int beds;
    int baths;
    std::string city;
    std::string state;
    Property(double p, double size, int bd, int bth, std::string c, std::string s) {
        price = p;
        house_size = size;
        beds = bd;
        baths = bth;
        city = c;
        state = s;
    }
};




#endif //PROPERTY_H
