# Simple C++ Task Manager (HTTP Server)

This is a simple HTTP server written in C++ using sockets.
It allows you to manage tasks in a browser.

## Features

* Add new task
* Delete task
* Edit task
* Mark task as done
* Save tasks to file (tasks.txt)
* Load tasks after restart
* Basic error handling (invalid input, empty task, etc.)

## How it works

The server receives HTTP requests from the browser, parses them manually and returns HTML responses.

All tasks are stored in memory and also saved to a file.

## How to run

```bash
mkdir build
cd build
cmake ..
make
./server
```

Open in browser:
http://localhost:9090

To stop the server:
Press **Ctrl + C** in the terminal.

## Project structure

* `server.cpp` — server and socket logic
* `handler.cpp` — request processing and routes
* `storage.cpp` — file save/load
* `httpRequest.cpp` — request parsing

## Notes

* Only basic HTTP is implemented
* No frameworks are used
* Designed for learning purposes

---

Made as a learning project in C++
