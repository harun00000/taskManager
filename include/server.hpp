#pragma once

#include <cstddef>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "handler.hpp"
#include <signal.h>

extern int server_fd_global;

int createServer();
void handleClient(int new_socket, std::vector<Task>& tasks, int& id);
void handleSignal(int signal);