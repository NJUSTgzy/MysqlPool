cmake_minimum_required(VERSION 3.24)
project(MysqlPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(MysqlPool main.cpp MysqlConnection.cpp MysqlConnection.h SqlPool.cpp SqlPool.h)

#target_link_libraries()
