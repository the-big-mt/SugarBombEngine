set(SBE_ROOT ${CMAKE_SOURCE_DIR}/src)

include_directories(
	${SBE_ROOT}
	${SBE_ROOT}/common
	${SBE_ROOT}/common/idlib
)

add_subdirectory(idlib)
add_subdirectory(SbCollision)

add_subdirectory(SbInput)
add_subdirectory(SbUserInterface)