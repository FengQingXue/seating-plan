cmake_minimum_required(VERSION 3.15)
project(seating_plan)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(seating_plan main.cpp sp.h base.cpp booking.cpp)