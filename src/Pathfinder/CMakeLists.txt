ADD_LIBRARY(Pathfinder STATIC pathfinder.cpp)

ADD_SUBDIRECTORY(Map)

TARGET_LINK_LIBRARIES(Pathfinder Map MicroPather)
