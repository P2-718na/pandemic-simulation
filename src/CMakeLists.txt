ADD_EXECUTABLE(lab main.cpp)

ADD_SUBDIRECTORY(Config)
ADD_SUBDIRECTORY(Pathfinder)
ADD_SUBDIRECTORY(Entity)
ADD_SUBDIRECTORY(World)

TARGET_LINK_LIBRARIES(lab Entity Pathfinder World)
TARGET_LINK_LIBRARIES(lab sfml-graphics sfml-window sfml-audio)
