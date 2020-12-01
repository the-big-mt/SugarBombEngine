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
/// @brief SbGame module exports

//*****************************************************************************

#include <functional>

#include "Game.hpp"
#include "SbGame/SbModuleAPI.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

C_EXPORT sbe::gameExport_t *GetGameAPI(sbe::gameImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::GAME_API_VERSION)
	{
		static f3goaty::CGame Game();
		
		static sbe::gameExport_t ModuleExports;
		
		ModuleExports.version = sbe::GAME_API_VERSION;
		ModuleExports.game = std::addressof(Game);
		
		return &ModuleExports;
	};
	
	return nullptr;
};