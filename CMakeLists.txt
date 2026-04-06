cmake_minimum_required(VERSION 3.16)
project(transferclients-qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

qt_standard_project_setup()

qt_add_executable(transferclients-qt  
    src/mainwindow.cpp 
    main.cpp
    src/mainwindow.ui  
    include/mainwindow.hpp
)

target_include_directories(transferclients-qt
    PRIVATE include
)

target_link_libraries(transferclients-qt
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Gui
        Qt6::Widgets
)