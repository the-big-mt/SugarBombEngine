set(SBE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	${SBE_ROOT}
	${SBE_ROOT}/common
	${SBE_ROOT}/common/idlib
	${SBE_ROOT}/idlib
)

add_subdirectory(idlib)
#add_subdirectory(SbCollision)

add_subdirectory(renderer)
##add_subdirectory(SbSound)
add_subdirectory(SbSystem)
#add_subdirectory(SbNetwork)

#add_subdirectory(framework)

#add_subdirectory(f3)
#add_subdirectory(f3launcher)

#add_subdirectory(launcher)

if(SBE_BUILD_TOOLS)
	add_subdirectory(tools)
endif()