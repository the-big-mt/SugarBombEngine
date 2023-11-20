/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

//*****************************************************************************

#include "SbGameExternal.hpp"

#include "SbGame/IGame.hpp"
#include "SbGame/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe::SbGameFramework
{

SbGameExternal::SbGameExternal()
{
	LoadModule();
};

SbGameExternal::~SbGameExternal()
{
	UnloadModule();
};

void SbGameExternal::LoadModule()
{
	mnGameLib = CSystem::LoadLib("SbGame");
	
	if(!mnGameLib)
		return;
	
	auto pfnGetGameAPI{CSystem::GetLibSymbol<GetGameAPI_t>(mnGameLib, "GetGameAPI")};
	
	if(!pfnGetGameAPI)
		return;
	
	gameImport_t ModuleImports{};
	ModuleImports.version = GAME_API_VERSION;
	auto ModuleExports{pfnGetGameAPI(&ModuleImports)};
	
	if(!ModuleExports)
		return;
	
	mpGame = ModuleExports->game;
	
	if(!mpGame)
		return;
};

void SbGameExternal::UnloadModule()
{
	CSystem::FreeLib(mnGameLib);
};

}; // namespace sbe::SbGameFramework