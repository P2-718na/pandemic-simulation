ADD_EXECUTABLE(pandemic-tests sample.test.cpp config.test.cpp pathfinder.test.cpp)

TARGET_LINK_LIBRARIES(pandemic-tests Config Pathfinder)

SET_TARGET_PROPERTIES(pandemic-tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
