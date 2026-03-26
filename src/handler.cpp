#include "handler.hpp"
#include "httpRequest.hpp"

std::string requestHandler(const std::string &request, std::vector<Task>& Tasks, size_t &id){
    if (request.size() == 0) return "";

    HttpRequest req(request);

    // GET  /hello  ->  hello page    
    if (req.getMethod() == "GET" && req.getPath() == "/hello") return handleGetHello();

    // GET  /tasks  ->  tasks page
    else if (req.getMethod() == "GET" && req.getPath() == "/tasks") return handleGetTasks(Tasks);

    // GET  /  ->  main page
    else if (req.getMethod() == "GET" && req.getPath() == "/") return handleGetMain();

    // POST /tasks  ->  add task
    else if (req.getMethod() == "POST" && req.getPath() == "/tasks") 
    {
        std::string title = req.getNewTask();
        return handlePostTasks(Tasks, id, title);
    }

    else return handleError();
}

std::string handleGetHello(){
    return 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Hello page</h1>";
}

std::string handleGetTasks(const std::vector<Task>& Tasks){
    std::string json = "[";

    for (size_t idx = 0; idx < Tasks.size(); idx++)
    {
        json.append("{\"id\":");
        json.append(std::to_string(Tasks[idx].id));
        json.append(",\"title\":\"");
        json.append(Tasks[idx].title);
        json.append("\"}");

        if (idx != Tasks.size() - 1)
        {
            json.append(",");
        }
    }

    json.append("]");

    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n"
        "\r\n" +
        json;
}

std::string handleGetMain(){
    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Main page</h1>";
}

std::string handlePostTasks(std::vector<Task>& Tasks, size_t& id, const std::string& title){
    if (title.empty())
    {
        return 
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1>Empty task</h1>";
    }

    Tasks.push_back({id++, title});

    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Task added</h1>";
}

std::string handleError(){
    return 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Not found</h1>";
}