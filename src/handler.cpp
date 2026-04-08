#include "handler.hpp"
#include "httpRequest.hpp"
#include <cstddef>
#include <vector>

std::string requestHandler(const std::string &request, std::vector<Task>& Tasks, int &id){
    if (request.size() == 0) return "";

    HttpRequest req(request);

    // GET  /tasks  ->  tasks page
    if (req.getMethod() == "GET" && req.getPath() == "/tasks") return handleGetTasks(Tasks);

    // GET  /  ->  main page
    if (req.getMethod() == "GET" && req.getPath() == "/") return handleGetMain();

    // POST /tasks  ->  add task
    if (req.getMethod() == "POST" && req.getPath() == "/tasks") 
    {
        std::string title = req.getNewTask();
        if (title.find("task=") != std::string::npos)
        {
            title = title.substr(5);
        }

        for (char& ch : title)
        {
            if (ch == '+')
                ch = ' ';
        }

        if (title.empty()) return "HTTP/1.1 400 Bad Request\r\n\r\nEmpty";

        return handlePostTasks(Tasks, id, title);
    }

    // POST /delete?id=42 -> delete task by id
    if (req.getMethod() == "POST" && req.getPath().find("/delete") != std::string::npos)
    {
        return handleDeleteTask(Tasks, req.getPath());
    }

    // POST /update?id=42 -> update task by id
    if (req.getMethod() == "POST" && req.getPath().find("/update") != std::string::npos)
    {
        std::string title = req.getNewTask();
        if (title.find("task=") != std::string::npos)
        {
            title = title.substr(5); // task=
        }

        for (char& ch : title)
        {
            if (ch == '+')
                ch = ' ';
        }

        if (title.empty()) return "HTTP/1.1 400 Bad Request\r\n\r\nEmpty";
        
        return handleUpdateTasks(title, Tasks, req.getPath());
    }

    // POST /done?id=42 -> make task done
    if (req.getMethod() == "POST" && req.getPath().find("/done") != std::string::npos)
    {
        return handleTaskDone(Tasks, req.getPath());
    }

    return handleError();
}

std::string handleGetTasks(const std::vector<Task>& Tasks){
    std::string html = "<h1>Tasks</h1><ul>";    // creating page, ul - list

    for (const auto& task : Tasks)
    {
        html += "<li>";                         // li - elem of list (open)
        html += std::to_string(task.id) + " - " + task.title;

        if (task.isDone == true)
        {
            html += " [DONE]";
        }

        // ------------ formule of delete-----------------
        html += "<form method='POST' action='/delete?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<button>Delete</button>";
        html += "</form>";
        // ------------ formule of delete-----------------


        // ------------ formule of update-----------------
        html += "<form method='POST' action='/update?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<input name='task' placeholder='Edit'>";
        html += "<button>Edit</button>";
        html += "</form>";
        // ------------ formule of update-----------------


        // ------------ formule of done-----------------
        html += "<form method='POST' action='/done?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<button>Done</button>";
        html += "</form>";
        // ------------ formule of done-----------------
        
        html += "</li>";                        // close elem of list
    }

    html += "</ul>";                            // close list
    html += "<br><a href='/'>Back</a>";         // add "back"

    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n" +
        html;
}

std::string handleGetMain(){
    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Task Manager</h1>"

        "<form method='POST' action='/tasks'>"          
        "<input name='task' placeholder='New task'>"
        "<button>Add</button>"
        "</form>"

        "<br>"
        "<a href='/tasks'>View tasks</a>";
}

std::string handlePostTasks(std::vector<Task>& Tasks, int& id, const std::string& title){
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

std::string handleDeleteTask(std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos) 
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";        
                                            // id= .....
    int id = std::stoi(path.substr(idPos + 3));
    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {                                                  
        if (Tasks[idx].id == id){
            Tasks.erase(Tasks.begin() + idx);   // cause we need iterator for erase
            break;
        }
    }

    return 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Deleted</h1>";
}

std::string handleUpdateTasks(std::string& title, std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos) 
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";

    int id = std::stoi(path.substr(idPos + 3));
    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {
        if (Tasks[idx].id == id)
        {
            Tasks[idx].title = title;
            break;
        }
    }

    return 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Updated</h1>";
}

std::string handleTaskDone(std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos)
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";

    int id = std::stoi(path.substr(idPos + 3));

    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {
        if (Tasks[idx].id == id)
        {
            Tasks[idx].isDone = true;
            break;
        }
    }
    
    return 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Marked as done</h1>";
}


std::string handleError(){
    return 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Not found</h1>";
}










