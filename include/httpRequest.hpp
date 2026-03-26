#pragma once

#include <cstddef>
#include <sstream>
#include <string>

class HttpRequest{
public:
    const std::string& getMethod();
    const std::string& getPath();
    const std::string& getNewTask();

    // constructor
    HttpRequest(const std::string &inputRaw);

private:
    std::string _inputRaw;
    std::string _method;
    std::string _path;
    std::string _newTask;

    void parse();
};