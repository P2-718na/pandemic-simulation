ADD_EXECUTABLE(test sample.test.cpp config.test.cpp pathfinder.test.cpp)

TARGET_LINK_LIBRARIES(test Config Pathfinder)

SET_TARGET_PROPERTIES(test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
