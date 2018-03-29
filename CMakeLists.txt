cmake_minimum_required(VERSION 3.5)
project(McGuffinQuest)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Entities)
add_subdirectory(Inventory)
add_subdirectory(Map)

add_executable(McGuffinQuest
        Main.cpp
        Game.h
        Game.cpp
        Graphics.cpp
        Graphics.h
        )

TARGET_LINK_LIBRARIES(McGuffinQuest Map Inventory Entities)
