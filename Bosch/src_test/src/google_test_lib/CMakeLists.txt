cmake_minimum_required(VERSION 2.6)
set(PROJECT_NAME google_test_lib)

# add a cached variable which will be used in each unit test project
set(GoogleTestLib ${PROJECT_NAME} CACHE STRING "Google Test Lib Name")

source_group("Header Files" *.h)
source_group("Source Files" *.cpp)

file(GLOB project_header_files *.h)
file(GLOB project_source_files *.cpp)
add_library(${PROJECT_NAME} SHARED ${project_header_files} ${project_source_files})

# add include directories, which will be evaluated as 

# VRM/src_test/googletest/googlemock/include
# VRM/src_test/googletest/googlemock
# VRM/src_test/googletest/googletest/include
# VRM/src_test/googletest/googletest

# in following command both path format are valid
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/googletest/googlemock/include 
                                           PUBLIC ${CMAKE_SOURCE_DIR}/googletest/googlemock
										   PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/googletest/googletest/include>
										   PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/googletest/googletest>)

add_definitions(-DGTEST_CREATE_SHARED_LIBRARY=1)

# warning caused by standard library can be ignored
# COMPILE_OPTIONS is a list, but COMPILE_FLAGS is a string.
# set_target_properties(target PROPERTIES COMPILE_OPTIONS "/option=test1;/option2=test2")
# set_target_properties(target PROPERTIES COMPILE_FLAGS "/option=test1 /option2=test2")
# can also use target_compile_options
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "/wd4251;/wd4275")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_OUTPUT_NAME ${PROJECT_NAME}_d)