cmake_minimum_required(VERSION 3.0.0)
project(findContour VERSION 0.1.0)

find_package(OpenCV 4 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(./include)

file(GLOB SOURCES "src/*.cpp")

add_executable(findContour main.cpp ${SOURCES})
target_link_libraries(findContour ${OpenCV_LIBS})

