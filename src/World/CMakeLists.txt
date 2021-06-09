ADD_LIBRARY(World STATIC world.cpp)

ADD_SUBDIRECTORY(Parser)

TARGET_LINK_LIBRARIES(World Entity Config Parser sfml-graphics)
