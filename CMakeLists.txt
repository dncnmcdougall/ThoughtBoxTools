cmake_minimum_required(VERSION 3.10)

project(sqltest)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib src/DB.cpp src/Query.cpp src/Arg.cpp src/CliArgs.cpp src/logging.cpp)

add_executable(read src/read.cpp src/ReadArgs.cpp )
add_executable(write src/write.cpp src/WriteArgs.cpp)

include_directories(src)

target_link_libraries(lib sqlite3)
target_link_libraries(lib stdc++fs)

target_link_libraries(read sqlite3)
target_link_libraries(read stdc++fs)
target_link_libraries(read lib)

target_link_libraries(write sqlite3)
target_link_libraries(write stdc++fs)
target_link_libraries(write lib)
