cmake_minimum_required(VERSION 3.5)
project(StateMachineLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Main.cpp StateMachine.cpp StateMachine.h ParkingApp.cpp ParkingApp.h)
add_executable(StateMachineLib ${SOURCE_FILES})