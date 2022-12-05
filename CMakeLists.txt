cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mein_maze)

if (WIN32)
	set(ARCH "x86")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(ARCH "x64")
	endif()
endif()

set(CMAKE_BUILD_TYPE Debug)

if (WIN32)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/cmake/SDL2")
	set(SDL2_DIR "${CMAKE_SOURCE_DIR}/external/SDL2")
	set(SDL2TTFDIR "${CMAKE_SOURCE_DIR}/external/SDL2_ttf")
	set(SDL2TTF_LIBRARY "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/lib/${ARCH}/SDL2_ttf.lib")
	set(SDL2TTF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/include/")
endif()

find_package(SDL2 REQUIRED)

add_definitions(-DDEBUG)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

file(GLOB SOURCE "./src/*.cpp" "./src/*.h" "./src/*.cxx")

add_executable(mein_maze ${SOURCE})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mein_maze)
set_property(TARGET mein_maze PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:mein_maze>")

target_link_libraries(mein_maze ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
target_precompile_headers(mein_maze PRIVATE src/pch.h)

if (WIN32)
	add_custom_command(TARGET mein_maze POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${SDL2_DIR}/lib/${ARCH}/SDL2.dll"
		$<TARGET_FILE_DIR:mein_maze>)

	add_custom_command(TARGET mein_maze POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${SDL2TTFDIR}/lib/${ARCH}/SDL2_ttf.dll"
		$<TARGET_FILE_DIR:mein_maze>)
endif()

