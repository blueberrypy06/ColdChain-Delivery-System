cmake_minimum_required(VERSION 4.2)
project(project2025112431)

set(CMAKE_CXX_STANDARD 20)

add_executable(project2025112431 cpp/main.cpp
        cpp/Package.cpp
        cpp/Package.h
        cpp/Truck.cpp
        cpp/Truck.h
        cpp/FileHandler.cpp
        cpp/FileHandler.h
        cpp/DeliveryManager.cpp
        cpp/DeliveryManager.h
)
