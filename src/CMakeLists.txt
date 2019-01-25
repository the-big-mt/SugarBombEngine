set(SBE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	${SBE_ROOT}/common
)

add_subdirectory(idlib)
#add_subdirectory(SbCollision)

add_subdirectory(renderer)
add_subdirectory(SbSound)
add_subdirectory(sys)

#add_subdirectory(framework)

#add_subdirectory(f3)