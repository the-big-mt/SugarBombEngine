add_subdirectory(SbMain)

add_subdirectory(SbSystem)

add_subdirectory(SbAnimation)
add_subdirectory(SbCollision)
add_subdirectory(SbFloodgate)
add_subdirectory(SbParticle)
#add_subdirectory(SbPortal)

add_subdirectory(SbGLCoreRenderer)

if(WIN32)
	#add_subdirectory(SbD3D11Renderer) # TODO
endif()

add_subdirectory(SbSound)