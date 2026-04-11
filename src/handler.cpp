#include "handler.hpp"
#include "storage.hpp"

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
    std::string html =
        "<html><head>"
        "<meta charset='UTF-8'>"    
        "<style>"

        "body { font-family: Arial; background: #f5f5f5; text-align: center; }"

        ".flag { width: 100%; height: 60px; }"
        ".white { background: white; height: 20px; }"
        ".blue { background: blue; height: 20px; }"
        ".red { background: red; height: 20px; }"

        // cards
        ".task { "
        "background: white; "
        "margin: 10px auto; "
        "padding: 10px; "
        "width: 400px; "
        "border-radius: 8px; "
        "box-shadow: 0 0 5px rgba(0,0,0,0.1); "
        "}"

        // buttons
        ".controls { margin-top: 5px; }"
        "button { margin-left: 5px; padding: 5px 10px; }"

        "</style>"
        "</head><body>"

        // secret fitches
        "<div class='flag'>"
        "<div class='white'></div>"
        "<div class='blue'></div>"
        "<div class='red'></div>"
        "</div>"

        "<h1>Task Manager</h1>";

    for (const auto& task : Tasks)
    {
        html += "<div class='task'>";
        html += std::to_string(task.id) + " - " + task.title;

        if (task.isDone == true)
        {
            html += " <span style='color:green'>✔</span>";
        }

        html += "<div class='controls'>";

        // DELETE
        html += "<form method='POST' action='/delete?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<button>Delete</button>";
        html += "</form>";

        // DONE
        html += "<form method='POST' action='/done?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<button>Done</button>";
        html += "</form>";

        // UPDATE
        html += "<form method='POST' action='/update?id=";
        html += std::to_string(task.id);
        html += "' style='display:inline'>";
        html += "<input name='task' placeholder='Edit'>";
        html += "<button>Edit</button>";
        html += "</form>";

        html += "</div>";   // close controls
        html += "</div>";   // close tasks
    }

    html += "<br><a href='/'>Back</a>";
    html += "</body></html>";

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

    // limit
    std::string newTitle = title;
    if (title.size() > 100){                                 
        newTitle = title.substr(0, 100);
    }
    
    Tasks.push_back({id++, newTitle, false});
    saveTasksInFile("tasks.txt", Tasks);

    return 
        "HTTP/1.1 303 See Other\r\n"
        "Location: /tasks\r\n"
        "Connection: close\r\n"
        "\r\n";
}

std::string handleDeleteTask(std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos) 
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";        
                                            
    int id{};
    try {                                 // id= ...        
        id = std::stoi(path.substr(idPos + 3));
    } catch (...){                                          // input not int
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid id";
    }

    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {                                                  
        if (Tasks[idx].id == id){
            Tasks.erase(Tasks.begin() + idx);   // cause we need iterator for erase
            saveTasksInFile("tasks.txt", Tasks);
            break;
        }
    }

    return  
        "HTTP/1.1 303 See Other\r\n"
        "Location: /tasks\r\n"
        "Connection: close\r\n"
        "\r\n";
}

std::string handleUpdateTasks(std::string& title, std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos) 
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";

    int id{};
    try {                                           
        id = std::stoi(path.substr(idPos + 3));
    } catch (...){                                          // input not int
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid id";
    }

    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {
        if (Tasks[idx].id == id)
        {
            Tasks[idx].title = title;
            saveTasksInFile("tasks.txt", Tasks);
            break;
        }
    }

    return 
        "HTTP/1.1 303 See Other\r\n"
        "Location: /tasks\r\n"
        "Connection: close\r\n"
        "\r\n";
}

std::string handleTaskDone(std::vector<Task>& Tasks, const std::string& path){
    size_t idPos = path.find("id=");
    if (idPos == std::string::npos)
        return "HTTP/1.1 400 Bad Request\r\n\r\nNo id";

    int id{};
    try {                                           
        id = std::stoi(path.substr(idPos + 3));
    } catch (...){                                          // input not int
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid id";
    }

    for (size_t idx = 0; idx < Tasks.size(); ++idx)
    {
        if (Tasks[idx].id == id)
        {
            Tasks[idx].isDone = !Tasks[idx].isDone;
            saveTasksInFile("tasks.txt", Tasks);
            break;
        }
    }
    
    return 
        "HTTP/1.1 303 See Other\r\n"
        "Location: /tasks\r\n"
        "Connection: close\r\n"
        "\r\n";
}


std::string handleError(){
    return 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Not found</h1>";
}










