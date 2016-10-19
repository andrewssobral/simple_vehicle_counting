cmake_minimum_required(VERSION 2.8)

project(simple_vehicle_counting)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

IF(UNIX)
  # add some standard warnings
  ADD_DEFINITIONS(-Wno-variadic-macros -Wno-long-long -Wall -Wextra -Winit-self -Woverloaded-virtual -Wsign-promo -Wno-unused-parameter -pedantic -Woverloaded-virtual -Wno-unknown-pragmas)

  # -ansi does not compile with sjn module
  #ADD_DEFINITIONS(-ansi)

  # if you like to have warinings about conversions, e.g. double->int or double->float etc., or float compare
  #ADD_DEFINITIONS(-Wconversion -Wfloat-equal)
endif(UNIX)

find_package(OpenCV REQUIRED)

if(${OpenCV_VERSION} VERSION_LESS 2.3.1)
  message (FATAL_ERROR "OpenCV version is not compatible: ${OpenCV_VERSION}")
endif()

file(GLOB demo Demo.cpp)


file(GLOB_RECURSE analysis package_analysis/*.cpp)
file(GLOB_RECURSE bgs package_bgs/*.cpp package_bgs/*.c)
file(GLOB_RECURSE bgs_include package_bgs/*.h)
file(GLOB_RECURSE tracking package_tracking/*.cpp package_tracking/*.c)
file(GLOB_RECURSE tracking_include package_tracking/*.h)

include_directories(${CMAKE_SOURCE_DIR})

add_library(simple_vehicle_counting SHARED ${tracking} ${bgs} ${analysis})
target_link_libraries(simple_vehicle_counting ${OpenCV_LIBS})
set_property(TARGET simple_vehicle_counting PROPERTY PUBLIC_HEADER ${bgs_include} ${tracking_include})

add_executable(simple_vehicle_counting_bin ${demo})
target_link_libraries(simple_vehicle_counting_bin ${OpenCV_LIBS} simple_vehicle_counting)
set_target_properties(simple_vehicle_counting_bin
  PROPERTIES OUTPUT_NAME simple_vehicle_counting)

