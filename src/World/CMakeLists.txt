ADD_LIBRARY(World STATIC world.cpp world-loops.cpp)

ADD_SUBDIRECTORY(Parser)

TARGET_LINK_LIBRARIES(World sfml-graphics Entity Config Parser)
