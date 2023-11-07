project(SBE-network)

#******************************************************************************

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(SBE_NETWORK_HARD_LINKED)
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
)

if(WIN32)
	add_subdirectory(win)
	
	target_link_libraries(${PROJECT_NAME} ws2_32) # Link against the Windows sockets 2.0 library
elseif(UNIX)
	#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/posix PROJECT_SOURCES_PLATFORM)
	#add_subdirectory(posix)
endif()

if(NOT SBE_NETWORK_HARD_LINKED)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbNetwork"
	)
	
	#target_link_libraries(${PROJECT_NAME} SBE-main)
endif()