#include "SbGameFramework.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::IGameFramework *GetGameFrameworkAPI(sbe::ISystem *apSystem)
{
	static sbe::SbGameFramework::SbGameFramework GameFramework(*apSystem);
	return &GameFramework;
};