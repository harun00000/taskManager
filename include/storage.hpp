#pragma once

#include "handler.hpp"
#include <string>
#include <fstream>
#include <sstream>

void saveTasksInFile(const std::string &fillename, const std::vector<Task> &Tasks);
std::vector<Task> loadTasksFromFile(const std::string &filename);
int getNextId(const std::vector<Task> &Tasks);