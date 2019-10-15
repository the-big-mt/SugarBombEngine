/// @file

#include "SbSystem.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::ISystem *GetSystemAPI()
{
	static sbe::SbSystem::SbSystem System;
	return &System;
};