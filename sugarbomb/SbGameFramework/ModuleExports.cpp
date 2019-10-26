/*
*******************************************************************************

Copyright (C) 2019 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

//*****************************************************************************

#include "SbGameFramework.hpp"
#include "SbGameFramework/SbModuleAPI.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

C_EXPORT sbe::gameFrameworkExport_t *GetGameFrameworkAPI(gameFrameworkImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::GAMEFRAMEWORK_API_VERSION)
	{
		static sbe::SbGameFramework::SbGameFramework GameFramework(*apModuleImports->sys);
		
		static sbe::gameFrameworkExport_t ModuleExports;
		
		ModuleExports.version = sbe::GAMEFRAMEWORK_API_VERSION;
		ModuleExports.gameFramework = &GameFramework;
		
		return &ModuleExports;
	};
	
	return nullptr;
};