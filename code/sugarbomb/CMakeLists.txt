set(SBE_ROOT ${CMAKE_SOURCE_DIR}/src)

include_directories(
	${SBE_ROOT}
	${SBE_ROOT}/common
	${SBE_ROOT}/common/idlib
)

include_directories(./SDK) # TODO: temp

add_subdirectory(CoreLibs)
add_subdirectory(AppFrameworks)

#add_subdirectory(idlib)

add_subdirectory(SbNetwork)