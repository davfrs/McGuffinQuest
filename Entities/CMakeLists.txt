add_library(Entities Entity.cpp Entity.h Player.h)
target_include_directories(Entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})