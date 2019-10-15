project(F3GOATY-gameapp)

add_executable(${PROJECT_NAME})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} SBE-clientapp SBE-application)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "F3GOATY"
)