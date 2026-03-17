//
// Created by Miguel Rosario on 3/17/26.
//
#ifndef DATALOADER_H
#define DATALOADER_H
#pragma once
#include "Property.h"
#include <vector>
#include <string>

std::vector<Property> loadData(const std::string& filename);
#endif //DATALOADER_H
