#include "storage.hpp"
#include "handler.hpp"

// in file: Task.id|Task.title|Task.isDone
void saveTasksInFile(const std::string &filename, const std::vector<Task> &Tasks){
    std::ofstream file(filename);

    for (const auto &Task : Tasks)
    {
        file << Task.id << "|" << Task.title << "|" << (Task.isDone ? "DONE" : "NOT DONE") << "\n";
    }
} 

// 42|go for a walk|DONE -> in vec Tasks
std::vector<Task> loadTasksFromFile(const std::string &filename){
    std::ifstream file(filename);
    std::vector<Task> Tasks{};

    if (!file.is_open()) return Tasks;

    std::string buf;

    while (getline(file, buf))
    {
        std::stringstream ss(buf);
        
        Task curTask;
        
        // id
        std::string idBuf;
        std::getline(ss, idBuf, '|');
        curTask.id = std::stoi(idBuf);

        // title
        std::getline(ss, curTask.title, '|');
        
        // isDone
        std::string isDoneBuf;
        std::getline(ss, isDoneBuf);    
        
        // FIX \r
        if (!isDoneBuf.empty() && isDoneBuf.back() == '\r')
        {
            isDoneBuf.pop_back();
        }

        curTask.isDone = (isDoneBuf == "DONE");

        Tasks.push_back(curTask);
    }
    
    return Tasks;
}

int getNextId(const std::vector<Task> &Tasks){
    if (Tasks.empty()) return 42;

    int maxId = -1;

    for (const auto &Task : Tasks)
    {
        if (Task.id > maxId)
        {
            maxId = Task.id;
        }
    }

    return maxId + 1;
}

