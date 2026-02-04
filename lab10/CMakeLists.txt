cmake_minimum_required(VERSION 3.20)
project(lab10)

set(CMAKE_CXX_STANDARD 17)

find_package(cpr CONFIG REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(lab10
        main.cpp City.cpp City.h
        Weather.h)

target_link_libraries(lab10 PRIVATE cpr::cpr)

target_link_libraries(lab10 PRIVATE nlohmann_json::nlohmann_json)