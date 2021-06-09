CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

PROJECT(
        pandemic-simulation
        VERSION 1.0
        DESCRIPTION "Graphical pandemic simulator"
        HOMEPAGE_URL https://github.com/P2-718na/pandemic-simulation
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FIND_PACKAGE(SFML 2.5 COMPONENTS graphics window REQUIRED)

INCLUDE_DIRECTORIES(
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/Pathfinder"
        "${PROJECT_SOURCE_DIR}/src/Engine"
        "${PROJECT_SOURCE_DIR}/src/Entity"
        "${PROJECT_SOURCE_DIR}/src/World"
        "${PROJECT_SOURCE_DIR}/src/Config"
        "${PROJECT_SOURCE_DIR}/dist"
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(seeder)

CONFIGURE_FILE(assets/background.sample.bmp "${PROJECT_BINARY_DIR}" COPYONLY)
CONFIGURE_FILE(scripts/quick-run.sh "${PROJECT_BINARY_DIR}" COPYONLY)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

MESSAGE("-- Building using ${CMAKE_BUILD_TYPE} configuration")

SET(CMAKE_CXX_FLAGS  "-Wall -Wextra")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=\"address\"")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
