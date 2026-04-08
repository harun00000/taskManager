#include "server.hpp"

// create server socket
int createServer(){
    // create socket               IPv4          TCP         automatic prot
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        perror("socket failed");
        return -1;
    }

    // for non-creating new host every time
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // struct for IPv4 (addresses, IP, port)
    sockaddr_in address{};
    address.sin_family = AF_INET;              // IPv4
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9090); // host to network short

    // tie server_fd to address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return -1;
    }

    // now socket waits inner connections
    if (listen(server_fd, 10) < 0) 
    {
        perror("listen failed");
        return -1;
    }

    return server_fd;
}

// handle one client
void handleClient(int new_socket, std::vector<Task>& Tasks, int& id){
    char buffer[1024] = {0};              // for reading
    ssize_t bytes = read(new_socket, buffer, 1023);

    if (bytes <= 0) 
    {
        close(new_socket);
        return;
    }

    buffer[bytes] = '\0';

    std::string request(buffer);               // doing string from data socket
    std::string answer = requestHandler(request, Tasks, id);

    // send answer to client (where, what, how many bytes, flag)
    send(new_socket, answer.c_str(), answer.size(), 0);

    close(new_socket);                    // connection with client
}

int server_fd_global = -1;

void handleSignal(int signal){
    std::cout << "\nShutting down server...\n";
    if (server_fd_global != -1) close(server_fd_global);
    exit(0);
}