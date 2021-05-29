ADD_EXECUTABLE(pandemy-tests sample.test.cpp config.test.cpp pathfinder.test.cpp)

TARGET_LINK_LIBRARIES(pandemy-tests Config Pathfinder)

SET_TARGET_PROPERTIES(pandemy-tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
