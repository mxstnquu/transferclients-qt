# transferclients-qt

A simple Qt-based client for file and message transfer over TCP

### Use the server application to test the client:  

https://github.com/mxstnquu/transferserver-qt


## Features

- Send text messages to the server
- Send files to the server
- Receive and display messages from the server
- Simple GUI


## Dependencies

- Qt6 (Core, Widgets, Network)
- CMake
- g++


## Build

```bash
git clone https://github.com/mxstnquu/transferclients-qt
cd ./transferclients-qt
cmake -S . -B build
cmake --build build
./build/transferclients-qt
```


## Project structure
```
transferclients-qt/
├── CMakeLists.txt
├── include/
│   ├── mainwindow.hpp
├── src/
│   ├── mainwindow.cpp
│   └── mainwindow.ui
└── main.cpp
```
