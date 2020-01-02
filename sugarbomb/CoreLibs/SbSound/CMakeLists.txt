project(SBE-sound)

option(SBE_USE_OPENAL "Use OpenAL for sound module backend" OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

if(MSVC)
	file(GLOB PROJECT_SOURCES_APISPEC
		${CMAKE_CURRENT_SOURCE_DIR}/xaudio2/*.cpp
	)
elseif(SBE_USE_OPENAL)
	file(GLOB PROJECT_SOURCES_APISPEC
		${CMAKE_CURRENT_SOURCE_DIR}/openal/*.cpp
	)
else()
	file(GLOB PROJECT_SOURCES_APISPEC
		${CMAKE_CURRENT_SOURCE_DIR}/stub/*.cpp
	)
endif()

list(APPEND PROJECT_SOURCES ${PROJECT_SOURCES_APISPEC})

if(SBE_SINGLE_BINARY)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE MODULE)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE} ${PROJECT_SOURCES})

if(SBE_USE_OPENAL)
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE SBE_USE_OPENAL
	)
	
	target_include_directories(${PROJECT_NAME}
		PRIVATE ${SBE_LIBS_PATH}/openal-soft/include
	)

	target_link_directories(${PROJECT_NAME}
		PRIVATE ${SBE_LIBS_PATH}/openal-soft/libs/win32
	)
endif()

if(NOT SBE_SINGLE_BINARY)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbSound"
	)
	
	#target_link_libraries(${PROJECT_NAME} SBE-idlib)
	
	if(SBE_USE_OPENAL)
		target_link_libraries(${PROJECT_NAME} -lOpenAL32)
	endif()
endif()