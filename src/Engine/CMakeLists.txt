ADD_LIBRARY(Engine STATIC engine.cpp)

TARGET_LINK_LIBRARIES(Engine Config World sfml-graphics sfml-window)
