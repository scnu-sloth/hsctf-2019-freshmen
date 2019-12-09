cmake_minimum_required(VERSION 3.15)
project(Crack_Password)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(includes)
AUX_SOURCE_DIRECTORY(modes APP_SRCS)
AUX_SOURCE_DIRECTORY(sm3 APP_SRCS)
AUX_SOURCE_DIRECTORY(sm4 APP_SRCS)

add_executable(backup backup.cpp ${APP_SRCS})
add_executable(recover recover.cpp ${APP_SRCS})
