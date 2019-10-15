/// @file

#include "SbInputSystem.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::IInputSystem *GetInputSystemAPI()
{
	static sbe::SbInput::SbInputSystem InputSystem;
	return &InputSystem;
};