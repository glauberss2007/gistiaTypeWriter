cmake_minimum_required(VERSION 3.16)
project(test)

set(CMAKE_CXX_STANDARD 14)

add_executable(assessment_results main.cpp exercises/deserealize_csv_datastore.cpp)

