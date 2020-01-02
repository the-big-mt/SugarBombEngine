cmake_minimum_required(VERSION 3.8)
project(SugarBombEngine)

option(SBE_SINGLE_BINARY "Build everything as a single binary" OFF)
option(SBE_USE_SDL "Use SDL2 for crossplatform support" OFF)

# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Set compiled binaries output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(SBE_LIBS_PATH
	${CMAKE_SOURCE_DIR}/../SBE-libs
	CACHE PATH
	"Path to engine dependencies"
)

set(SBE_SDL_PATH
	${CMAKE_SOURCE_DIR}/../SDL2
	CACHE PATH
	"Path to SDL2"
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/libs
	${SBE_LIBS_PATH}
	${SBE_LIBS_PATH}/jpeg-6
	${SBE_LIBS_PATH}/png
	${SBE_LIBS_PATH}/glew/include
	${SBE_LIBS_PATH}/rapidjson/include
)

if(MINGW)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/mingw-hacks)
endif()

add_subdirectory(libs)

add_subdirectory(sugarbomb)
add_subdirectory(f3goaty)
#add_subdirectory(code)
#add_subdirectory(src)