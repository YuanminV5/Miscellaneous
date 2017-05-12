cmake_minimum_required(VERSION 2.6)
set(PROJECT_NAME user_management)

file(GLOB CUT_files ${VRM_MODULE_PATH}/user_management/*.h ${VRM_MODULE_PATH}/user_management/*.cpp)

list(REMOVE_ITEM CUT_files "${VRM_MODULE_PATH}/user_management/VrmUserManagementFacade.h")
list(REMOVE_ITEM CUT_files "${VRM_MODULE_PATH}/user_management/VrmUserManagementFacade.cpp")

include (../CMakeProjectLists.txt)
add_definitions(-DNDEBUG)

# include the following to access test class
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/..)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/../include)