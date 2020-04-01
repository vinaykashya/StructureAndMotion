cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project( SFM )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

link_directories(${OpenCV_LIBRARY_DIRS})

find_package( OpenCV REQUIRED )

set(SOURCES source/SFM.cpp source/PoseEstimator.cpp)

add_executable( SFM ${SOURCES})
target_link_libraries( SFM LINK_PUBLIC ${OpenCV_LIBS})

