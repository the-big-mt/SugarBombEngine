project(SBE-network)

#******************************************************************************

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(WIN32)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/win PROJECT_SOURCES_PLATFORM)
elseif(UNIX)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/posix PROJECT_SOURCES_PLATFORM)
endif()

if(SBE_SINGLE_BINARY)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE MODULE)
endif()

#******************************************************************************

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_SOURCES_PLATFORM}
)

if(WIN32)
if(NOT SBE_SINGLE_BINARY)
	target_link_libraries(${PROJECT_NAME} ws2_32) # Link against the Windows sockets 2.0 library
endif()

	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbNetwork"
	)
	
	#target_link_libraries(${PROJECT_NAME} SBE-main)
endif()