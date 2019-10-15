/// @file

#include "SbRenderSystem.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::IRenderSystem *GetRenderSystemAPI()
{
	static sbe::SbRenderer::SbRenderSystem RenderSystem;
	return &RenderSystem;
};