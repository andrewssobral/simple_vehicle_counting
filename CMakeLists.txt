cmake_minimum_required(VERSION 2.8)

project(simple_vehicle_counting)

# cmake -D BUILD_PYTHON_SUPPORT=ON ..
if(NOT DEFINED BUILD_PYTHON_SUPPORT)
	set(BUILD_PYTHON_SUPPORT OFF)
elseif()
	# add_definitions(-DBUILD_PYTHON_SUPPORT)
endif()
message(STATUS "WITH PYTHON SUPPORT: ${BUILD_PYTHON_SUPPORT}")

# compilation mode setup
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
	set(CMAKE_MACOSX_RPATH 1)
endif(UNIX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(WIN32)

set(out_dir ".")
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${out_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${out_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${out_dir})
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${out_dir})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${out_dir})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${out_dir})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

IF (UNIX)
    # add some standard warnings
    ADD_DEFINITIONS(-Wno-variadic-macros -Wno-long-long -Wall -Wextra -Winit-self -Woverloaded-virtual -Wsign-promo -Wno-unused-parameter -pedantic -Woverloaded-virtual -Wno-unknown-pragmas)

    # -ansi does not compile with sjn module
    #ADD_DEFINITIONS(-ansi)

    # if you like to have warinings about conversions, e.g. double->int or double->float etc., or float compare
    #ADD_DEFINITIONS(-Wconversion -Wfloat-equal)
endif (UNIX)

find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if (${OpenCV_VERSION} VERSION_LESS 2.3.1)
    message(FATAL_ERROR "OpenCV version is not compatible: ${OpenCV_VERSION}")
endif ()

file(GLOB demo Demo.cpp)

file(GLOB_RECURSE bgs_src package_bgs/*.cpp package_bgs/*.c)
file(GLOB_RECURSE bgs_inc package_bgs/*.h)
file(GLOB_RECURSE tracking_src package_tracking/*.cpp package_tracking/*.c)
file(GLOB_RECURSE tracking_inc package_tracking/*.h)
file(GLOB_RECURSE analysis_src package_analysis/*.cpp)
file(GLOB_RECURSE analysis_inc package_analysis/*.h)

include_directories(${CMAKE_SOURCE_DIR})

add_library(simple_vehicle_counting STATIC ${bgs_src} ${tracking_src} ${analysis_src})
target_link_libraries(simple_vehicle_counting ${OpenCV_LIBS})
set_property(TARGET simple_vehicle_counting PROPERTY PUBLIC_HEADER ${bgs_inc} ${tracking_inc} ${analysis_inc})
if(WIN32)
	set_property(TARGET simple_vehicle_counting PROPERTY SUFFIX ".lib")
endif()

add_executable(simple_vehicle_counting_bin ${demo})
target_link_libraries(simple_vehicle_counting_bin ${OpenCV_LIBS} simple_vehicle_counting)
set_target_properties(simple_vehicle_counting_bin
        PROPERTIES OUTPUT_NAME simple_vehicle_counting)

if(BUILD_PYTHON_SUPPORT)
  set(PYTHON_DESIRED_VERSION "2.X" CACHE STRING "Use Python 2.")
  add_subdirectory(python/pyboostcvconverter)

  find_package(Boost COMPONENTS python REQUIRED)
  message("Include dirs of boost: " ${Boost_INCLUDE_DIRS})
  message("Libs of boost: " ${Boost_LIBRARIES})
  find_package(PythonLibs 2.7 REQUIRED)
  message("Include dirs of Python: " ${PYTHON_INCLUDE_DIRS})
  message("Libs of Python: " ${PYTHON_LIBRARIES})

  add_subdirectory(python/tracking)
  add_subdirectory(python/cvb)
  add_subdirectory(python/bgs)
  add_subdirectory(python/analysis)
endif()
