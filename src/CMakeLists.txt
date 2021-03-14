ADD_EXECUTABLE(lab main.cpp)

INCLUDE_DIRECTORIES(
        lab PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/Bitmap"
        "${PROJECT_SOURCE_DIR}/src/Pathfinder"
)

ADD_SUBDIRECTORY(Bitmap)
ADD_SUBDIRECTORY(Pathfinder)

TARGET_LINK_LIBRARIES(lab Bitmap Pathfinder)