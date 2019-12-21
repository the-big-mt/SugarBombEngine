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

/// @file
/// @brief SbGameFramework module exports

//*****************************************************************************

#include "SbGameFramework.hpp"
#include "SbGameFramework/SbModuleAPI.hpp"
#include "SbGameExternal.hpp"
#include "SbNetworkExternal.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

sbe::IGame *CreateGame(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbGameFramework::SbGameExternal SbGameModule(aSystem);
	return SbGameModule.GetGame();
#else
	return new sbe::SbGame::SbGame();
#endif
};

sbe::INetworkSystem *CreateNetworkSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbGameFramework::SbNetworkExternal SbNetworkModule(aSystem);
	return SbNetworkModule.GetNetworkSystem();
#else
	return new sbe::SbNetwork::SbNetworkSystem();
#endif
};

C_EXPORT sbe::gameFrameworkExport_t *GetGameFrameworkAPI(sbe::gameFrameworkImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::GAMEFRAMEWORK_API_VERSION)
	{
		static sbe::IGame *pGame = CreateGame(*apModuleImports->sys);
		static sbe::INetworkSystem *pNetworkSystem = CreateNetworkSystem(*apModuleImports->sys);
		static sbe::SbGameFramework::SbGameFramework GameFramework(*pNetworkSystem, *pGame, *apModuleImports->sys);
		
		static sbe::gameFrameworkExport_t ModuleExports;
		
		ModuleExports.version = sbe::GAMEFRAMEWORK_API_VERSION;
		ModuleExports.gameFramework = &GameFramework;
		
		return &ModuleExports;
	};
	
	return nullptr;
};