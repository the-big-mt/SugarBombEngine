set(SBE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	${SBE_ROOT} # TEMP
	${SBE_ROOT}/common
	${SBE_ROOT}/common/idlib
)

add_subdirectory(CoreLibs)

add_subdirectory(SbNetwork)
add_subdirectory(SbPhysics)

#add_subdirectory(framework)
add_subdirectory(SbGameFramework)

if(SBE_BUILD_TOOLS)
	add_subdirectory(tools)
endif()