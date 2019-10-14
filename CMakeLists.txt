cmake_minimum_required(VERSION 3.8)
project(SugarBombEngine)

option(SBE_SINGLE_BINARY "Build everything as a single binary" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(SBE_LIBS_PATH
	${CMAKE_SOURCE_DIR}/../SBE-libs
	CACHE PATH
	"Path to engine dependencies"
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

add_subdirectory(sugarbomb)
add_subdirectory(f3goaty)
#add_subdirectory(code)
#add_subdirectory(src)