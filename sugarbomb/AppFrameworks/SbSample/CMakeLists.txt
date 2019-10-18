project(SBE-sampleapp)

file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} SBE-clientapp SBE-application)

if(NOT SBE_SINGLE_BINARY)
	target_link_libraries(${PROJECT_NAME} SBE-libraryloader)

	if(UNIX)
		target_link_libraries(${PROJECT_NAME} -ldl)
	endif()
endif()
endif()