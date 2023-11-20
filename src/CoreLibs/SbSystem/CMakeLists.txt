project(SBE-system)

if(SBE_SINGLE_BINARY)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE MODULE)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_SOURCES_PLATFORM}
)

if(WIN32)
	add_subdirectory(win)
elseif(UNIX)
	add_subdirectory(posix)
endif()

if(NOT SBE_SINGLE_BINARY)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbSystem"
	)
	
	#target_link_libraries(${PROJECT_NAME} SBE-idlib)
endif()