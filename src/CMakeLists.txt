set(SBE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	${SBE_ROOT} # TEMP
	${SBE_ROOT}/common
	${SBE_ROOT}/common/idlib
)

#add_subdirectory(framework)

if(SBE_BUILD_TOOLS)
	add_subdirectory(tools)
endif()