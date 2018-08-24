cmake_minimum_required (VERSION 2.6)
project (AVLTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${PROJECT_BINARY_DIR}")

add_executable(AVLTree avl_tree.cpp)