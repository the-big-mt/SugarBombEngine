/// @file

#include "SbSoundSystem.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::ISoundSystem *GetSoundSystemAPI()
{
	static sbe::SbSound::SbSoundSystem SoundSystem;
	return &SoundSystem;
};