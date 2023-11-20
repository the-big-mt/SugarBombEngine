/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @brief SbInput module exports

//*****************************************************************************

#include <functional>

#include "SbInputSystem.hpp"
#include "SbGamepad.hpp"

#ifdef SBE_USE_SDL
#	include "sdl2/SbInputSDL2.hpp"
#elif defined(_WIN32)
#	include "win/SbInputWin.hpp"
#endif

#include "AppFrameworks/UtilityLibs/SbInput/SbModuleAPI.hpp"
#include "AppFrameworks/UtilityLibs/SbInput/SbKeyboard.hpp"
#include "AppFrameworks/UtilityLibs/SbInput/SbMouse.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

C_EXPORT sbe::inputExport_t *GetInputAPI(sbe::inputImport_t *apModuleExports)
{
	if(apModuleExports->version == sbe::INPUT_API_VERSION)
	{
		static sbe::ISystem &System = *apModuleExports->sys;
#ifdef SBE_USE_SDL
		static sbe::SbInput::SbInputSDL2 Impl(System);
#elif defined(_WIN32)
		static sbe::SbInput::SbInputWin Impl(System);
#endif
		static sbe::SbInput::SbInputSystem InputSystem(System, Impl);
		
		static sbe::inputExport_t ModuleExports;
		
		ModuleExports.version = sbe::INPUT_API_VERSION;
		ModuleExports.inputSystem = std::addressof(InputSystem);
		
		return std::addressof(ModuleExports);
	};
	
	return nullptr;
};