ADD_EXECUTABLE(lab main.cpp)

INCLUDE_DIRECTORIES(
        lab PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/Bitmap"
        "${PROJECT_SOURCE_DIR}/src/Pathfinder"
        "${PROJECT_SOURCE_DIR}/src/Entity"
)

ADD_SUBDIRECTORY(Bitmap)
ADD_SUBDIRECTORY(Pathfinder)
ADD_SUBDIRECTORY(Entity)

TARGET_LINK_LIBRARIES(lab Bitmap Pathfinder Entity)
TARGET_LINK_LIBRARIES(lab sfml-graphics sfml-window sfml-audio)