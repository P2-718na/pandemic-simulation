ADD_LIBRARY(World STATIC world.cpp world-loops.cpp world-parsers.cpp)

TARGET_LINK_LIBRARIES(World sfml-graphics Entity)
