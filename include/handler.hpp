#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include "httpRequest.hpp"

struct Task{
    int id;
    std::string title;
    bool isDone = false;
};

std::string requestHandler(const std::string &request, std::vector<Task> &Tasks, int &id);
std::string handleGetTasks(const std::vector<Task>& tasks);
std::string handleGetMain();
std::string handlePostTasks(std::vector<Task>& tasks, int& id, const std::string& title);
std::string handleDeleteTask(std::vector<Task>& Tasks, const std::string& path);
std::string handleUpdateTasks(std::string& title, std::vector<Task>& Tasks, const std::string& path);
std::string handleTaskDone(std::vector<Task>& Tasks, const std::string& path);
std::string handleError();