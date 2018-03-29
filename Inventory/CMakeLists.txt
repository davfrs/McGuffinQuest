add_library(Inventory Consumables.h Inventory.h Inventory.cpp Item.cpp Item.h)
target_include_directories(Inventory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})