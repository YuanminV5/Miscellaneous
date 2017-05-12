cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configuration types" FORCE)

message(STATUS ${CMAKE_SOURCE_DIR})

project(vrm_unit_test)

add_subdirectory(src)