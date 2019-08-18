project(vaultserver)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)