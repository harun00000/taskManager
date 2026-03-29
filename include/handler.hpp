#pragma once

#include <string>
#include <vector>
#include <cstddef>

struct Task{
    size_t id;
    std::string title;
};

std::string requestHandler(const std::string &request, std::vector<Task> &Tasks, size_t &id);
std::string handleGetHello();
std::string handleGetTasks(const std::vector<Task>& tasks);
std::string handleGetMain();
std::string handlePostTasks(std::vector<Task>& tasks, size_t& id, const std::string& title);
std::string handleDeleteTask(std::vector<Task>& Tasks, const std::string& path);
std::string handleError();