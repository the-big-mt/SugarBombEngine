project(SBE-application)

#******************************************************************************

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

#******************************************************************************

add_library(${PROJECT_NAME} STATIC)

if(SBE_SYSTEM_HARD_LINKED)
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE SBE_SYSTEM_HARD_LINKED
	)
endif()

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_SOURCES_PLATFORM}
)