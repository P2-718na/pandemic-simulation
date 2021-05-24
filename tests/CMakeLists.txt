ADD_EXECUTABLE(pandemy-tests ./sample.test.cpp ./ai.test.cpp ./pathfinder.test.cpp)

TARGET_LINK_LIBRARIES(pandemy-tests Entity Pathfinder Map)

SET_TARGET_PROPERTIES(pandemy-tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

ADD_SUBDIRECTORY(assets)
