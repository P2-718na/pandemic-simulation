ADD_EXECUTABLE(seeder seeder.cpp)

TARGET_LINK_LIBRARIES(seeder Entity Pathfinder)
TARGET_LINK_LIBRARIES(seeder sfml-graphics)
