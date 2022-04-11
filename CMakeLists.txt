cmake_minimum_required(VERSION 3.9)
project(LaneDetection)
set(OpenCV_DIR /usr/share/opencv4/)
find_package(OpenCV 4.1.1 REQUIRED)
include_directories(
	./
	${OpenCV_INCLUDE_DIRS}
)
add_executable(LaneDetection main.cpp)
target_link_libraries(LaneDetection ${OpenCV_LIBS})
