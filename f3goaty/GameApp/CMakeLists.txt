project(F3GOATY-gameapp)

add_executable(${PROJECT_NAME})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} SBE-clientapp SBE-application)

if(NOT SBE_SINGLE_BINARY)
	target_link_libraries(${PROJECT_NAME} SBE-libraryloader)

	if(UNIX)
		target_link_libraries(${PROJECT_NAME} -ldl)
	endif()
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "F3GOATY"
)