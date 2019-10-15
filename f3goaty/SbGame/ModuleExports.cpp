/// @file

#include "Game.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::IGame *GetGameAPI()
{
	static f3goaty::CGame Game;
	return &Game;
};