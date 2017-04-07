project(pbas)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_library(_${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(_${PROJECT_NAME} PUBLIC
        ${PYTHON_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/python/pyboostcvconverter/include)
target_link_libraries(_${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        ${OpenCV_LIBS}
        simple_vehicle_counting
        pbcvt)
set_target_properties(_${PROJECT_NAME} PROPERTIES PREFIX "")