cmake_minimum_required(VERSION 3.9)
project(helloWorldC__)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
file(GLOB srcFiles src/*.cpp)
add_executable(helloWorldC__ ${srcFiles})

target_include_directories(helloWorldC__ PRIVATE /usr/include/libevdev-1.0)

target_link_libraries(helloWorldC__ X11 Xtst evdev)
