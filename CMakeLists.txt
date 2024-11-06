cmake_minimum_required(VERSION 3.10)
project(MarkerMapper)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find PCL
find_package(PCL 1.8 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})

# Find Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# Find Boost
find_package(Boost REQUIRED)

# Add DodecaTrackerPlugin library
add_library(DodecaTrackerPlugin SHARED 
    ../DodecaTrackerPlugin/DodecaTrackerPlugin.cpp
    ../DodecaTrackerPlugin/DodecaTrackerPlugin.hpp
)

# Add MarkerMapper library
add_library(MarkerMapper 
    ../MarkerMapper/Calibration.cpp
    ../MarkerMapper/Calibration.hpp
    ../MarkerMapper/Camera.cpp
    ../MarkerMapper/Camera.hpp
    ../MarkerMapper/DodecaTracker.cpp
    ../MarkerMapper/DodecaTracker.hpp
    ../MarkerMapper/dodtracmain.cpp
    ../MarkerMapper/main.cpp
    ../MarkerMapper/Pen.cpp
    ../MarkerMapper/Pen.hpp
    ../MarkerMapper/PenDetector.cpp
    ../MarkerMapper/PenDetector.hpp
    ../MarkerMapper/stdafx.h
)

# Link libraries
target_link_libraries(DodecaTrackerPlugin ${OpenCV_LIBS})
target_link_libraries(DodecaTrackerPlugin ${PCL_LIBRARIES})
target_link_libraries(DodecaTrackerPlugin ${CERES_LIBRARIES})
target_link_libraries(DodecaTrackerPlugin ${Boost_LIBRARIES})