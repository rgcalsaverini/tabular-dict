cmake_minimum_required(VERSION 3.13)
project(flatten_json)

set(CMAKE_CXX_FLAGS "-O3")

include_directories(include)
include_directories(external)

add_subdirectory(external/json)
add_subdirectory(external/pybind11)

set(CMAKE_CXX_STANDARD 20)

pybind11_add_module(
        flatten_json
        external/pybind11_json/pybind11_json.h
        src/flatten.cpp include/flatten.h
        src/py_module.cpp
)

target_link_libraries(flatten_json PRIVATE nlohmann_json::nlohmann_json)

