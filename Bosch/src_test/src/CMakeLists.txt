cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# TODO: clean up these options
if(UNIX)
	add_definitions(-DSTD_LINUX)
endif(UNIX)

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x600)
	add_definitions(-DWIN32)
	add_definitions(-D_WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DNOMINMAX)
	add_definitions(-DWIN32_LEAN_AND_MEAN)	
endif(WIN32)

if(MSVC)
	add_definitions(-DUSE_LICPROTECTORDLL)
endif(MSVC)
if(MINGW)
	add_definitions(-D __MSVCRT_VERSION__=0x800)
endif(MINGW)

set(VRM_MODULE_PATH "${CMAKE_SOURCE_DIR}/../src/")

# set target output folder - LIBRARY_OUTPUT_DIRECTORY is deprecated
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# projects have build dependency, specify build order by command target_link_libraries
add_subdirectory(google_test_lib)
add_subdirectory(vrm_mock_lib)
add_subdirectory(user_management)

