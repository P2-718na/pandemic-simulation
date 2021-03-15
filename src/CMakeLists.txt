ADD_EXECUTABLE(lab main.cpp)

INCLUDE_DIRECTORIES(
        lab PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/Pathfinder"
        "${PROJECT_SOURCE_DIR}/src/Entity"
        "${PROJECT_SOURCE_DIR}/src/World"
)

ADD_SUBDIRECTORY(Pathfinder)
ADD_SUBDIRECTORY(Entity)
ADD_SUBDIRECTORY(World)

TARGET_LINK_LIBRARIES(lab Pathfinder Entity World)
TARGET_LINK_LIBRARIES(lab sfml-graphics sfml-window sfml-audio)