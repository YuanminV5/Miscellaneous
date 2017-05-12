cmake_minimum_required(VERSION 2.6)
set(PROJECT_NAME vrm_mock_lib)

# add a cached variable which will be used in each unit test project
set(VRMMockLib ${PROJECT_NAME} CACHE STRING "VRM Mock Lib Name")

file(GLOB project_header_files include/*.h)
file(GLOB project_source_files src/*.cpp)
file(GLOB vcs_files include/vcslib/*.* include/vcslib/*/*.* src/vcslib/*.* src/vcslib/*/*.*)
file(GLOB rcpp_files include/rcpp/*.* src/rcpp/*.*)
file(GLOB vrm_files include/vrmsrc/*/*.* src/vrmsrc/*.*)

source_group("Header Files" FILES ${project_header_files}) #--> regular expression recommonded
source_group("Source Files" FILES ${project_source_files})
source_group("VCSLIB" FILES ${vcs_files})
source_group("RCPP" FILES ${rcpp_files})
source_group("VRMSRC" FILES ${vrm_files})

add_library(${PROJECT_NAME} SHARED ${project_header_files} ${project_source_files} ${vcs_files} ${rcpp_files} ${vrm_files})

# add include directories, which will be evaluated as 

# VRM/src_test/googletest/googlemock/include
# VRM/src_test/googletest/googlemock
# VRM/src_test/googletest/googletest/include
# VRM/src_test/googletest/googletest

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/googletest/googlemock/include> 
										   PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/googletest/googletest/include>)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/../include)

add_definitions(-DEXPORT_MOCKS=1)	
									   
# specify dependencies - implicitly specify build order
target_link_libraries(${PROJECT_NAME} ${GoogleTestLib})

add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY=1)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "/wd4251;/wd4275")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_OUTPUT_NAME ${PROJECT_NAME}_d)