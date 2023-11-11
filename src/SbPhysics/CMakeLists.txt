project(SBE-physics)

#******************************************************************************

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(SBE_PHYSICS_HARD_LINKED)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE MODULE)
endif()

#******************************************************************************

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE ${BULLET_INCLUDE_DIR}
)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

if(NOT SBE_PHYSICS_HARD_LINKED)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbPhysics"
	)
	
	#target_link_libraries(${PROJECT_NAME} SBE-main)
	
	target_link_libraries(${PROJECT_NAME} ${BULLET_LIBRARIES})
endif()