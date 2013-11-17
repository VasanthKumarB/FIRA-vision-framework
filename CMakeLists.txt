
cmake_minimum_required(VERSION 2.6)

project(main)

FIND_PACKAGE(OpenCV REQUIRED)

add_library(fira arena.cpp Ball.cpp colors.cpp contours.cpp Robot.cpp
        Environment.cpp OpponentRobot.cpp geometry.cpp Ximea.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  fira ${OpenCV_LIBS} m3api serial  )

