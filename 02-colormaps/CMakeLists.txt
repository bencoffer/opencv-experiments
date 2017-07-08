cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project( prog )

find_package( OpenCV REQUIRED )
message( "OpenCV_FOUND = ${OpenCV_FOUND}" )
message( "OpenCV_CONSIDERED_CONFIGS = ${OpenCV_CONSIDERED_CONFIGS}" )
message( "OpenCV_CONSIDERED_VERSIONS = ${OpenCV_CONSIDERED_VERSIONS}" )
message( "OpenCV_VERSION = ${OpenCV_VERSION}" )
message( "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}" )

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( prog main.cpp )
target_link_libraries( prog ${OpenCV_LIBS} )
