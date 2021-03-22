ADD_EXECUTABLE(tests ./sample.test.cpp ./ai.test.cpp)

TARGET_LINK_LIBRARIES(tests Entity Pathfinder)