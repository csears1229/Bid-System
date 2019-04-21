cmake_minimum_required(VERSION 3.13)
project(DS)

set(CMAKE_CXX_STANDARD 11)

add_executable(DS main.cpp
        CSVparser.cpp
        CSVparser.hpp
        eBid_Monthly_Sales_Dec_2016.csv)