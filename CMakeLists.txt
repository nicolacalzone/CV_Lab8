cmake_minimum_required(VERSION 3.1)
project(lab8)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task2 task2.cpp functions/utils functions/calibrationUtils)
target_link_libraries(task2 ${OpenCV_LIBS})