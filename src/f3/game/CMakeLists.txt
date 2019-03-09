project(SBE-f3) # SBE-game?

file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

if(SBE_SINGLE_BINARY)
	set(LIB_TYPE STATIC)
else()
	set(LIB_TYPE MODULE)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE} ${PROJECT_SOURCES})

if(NOT SBE_SINGLE_BINARY)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "SbGame"
	)
	
	target_link_libraries(${PROJECT_NAME} SBE-idlib)
endif()