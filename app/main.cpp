#include "server.hpp"
#include "storage.hpp"

int main(){
    signal(SIGINT, handleSignal);

    int server_fd = createServer();
    if (server_fd < 0) return 1;

    server_fd_global = server_fd;

    std::cout << "Server started on http://localhost:9090\n";

    std::vector<Task> tasks = loadTasksFromFile("tasks.txt");
    int id = getNextId(tasks);

    while(true)
    {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }

        handleClient(new_socket, tasks, id);
    }

    close(server_fd);
    return 0;
}