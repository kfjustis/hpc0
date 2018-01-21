cmake_minimum_required(VERSION 3.5)

if (UNIX)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_COMPILER "g++")
endif()

# compile flags
set(CMAKE_CXX FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# create project
project(homework_0)

add_subdirectory(src)
