ADD_LIBRARY(World STATIC world.cpp)

ADD_SUBDIRECTORY(Parser)

TARGET_LINK_LIBRARIES(World sfml-graphics Map Entity Config Parser)
