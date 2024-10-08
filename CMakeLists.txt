cmake_minimum_required(VERSION 3.28)

project(rdkafka-producer)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_FLAGS "-std=c++20 -fno-omit-frame-pointer -fsanitize=memory")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RD_KAFKA_STATIC_LIB_PATH "")
option(RD_KAFKA_INCLUDE_PATH "")

message(STATUS "RdKafka static lib: ${RD_KAFKA_STATIC_LIB_PATH}")
message(STATUS "RdKafka include directory: ${RD_KAFKA_INCLUDE_PATH}")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE "${RD_KAFKA_STATIC_LIB_PATH}")
target_include_directories(${PROJECT_NAME} PRIVATE "${RD_KAFKA_INCLUDE_PATH}")
