ADD_LIBRARY(Entity STATIC entity.cpp entity-accessors.cpp entity-loops.cpp)

ADD_SUBDIRECTORY(AI)

TARGET_LINK_LIBRARIES(Entity AI Config World)
