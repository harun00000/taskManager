#include "httpRequest.hpp"

HttpRequest::HttpRequest(const std::string &inputRaw) : _inputRaw(inputRaw){
    parse();
}

void HttpRequest::parse(){
    // 1) parse first line to get method and path
    size_t firstLineEnd = _inputRaw.find("\r\n");
    std::string firstLine = _inputRaw.substr(0, firstLineEnd);

    std::stringstream ss(firstLine);
    ss >> _method >> _path;

    // 2) parse newTask
    if (size_t pos = _inputRaw.find("\r\n\r\n"); pos != std::string::npos)
    {
        _newTask = _inputRaw.substr(pos + 4);
    }
}

const std::string& HttpRequest::getMethod(){
    return _method;   
}

const std::string& HttpRequest::getPath(){
    return _path;
}

const std::string& HttpRequest::getNewTask(){
    return _newTask;
}

