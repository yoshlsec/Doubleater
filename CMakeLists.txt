cmake_minimum_required(VERSION 3.10)

project(Doubleater)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_STANDARD 11)
add_definitions(-DUNICODE -D_UNICODE)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_executable(Doubleater ${SOURCES})