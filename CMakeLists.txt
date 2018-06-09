cmake_minimum_required(VERSION 3.9)
project(helloWorldC__)

set(CMAKE_CXX_STANDARD 11)

add_executable(helloWorldC__
        src/hello.cpp
        src/TouchController.cpp
        src/TouchController.h
        src/Touch.h
        src/TouchHistory.cpp
        src/TouchHistory.h)

find_package(X11)
find_package(Xtst)

target_link_libraries(helloWorldC__ X11 Xtst)