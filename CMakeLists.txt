cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME project)

file(GLOB_RECURSE SOURCES	src/*.cpp src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})