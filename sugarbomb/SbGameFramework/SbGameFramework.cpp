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

#include "SbGameFramework.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbGame/IGame.hpp"

namespace sbe::SbGameFramework
{

SbGameFramework::SbGameFramework(ISystem &aSystem) : mSystem(aSystem){}

void SbGameFramework::Init()
{
	LoadGameModule();
	
	mpGame->Init();
};

void SbGameFramework::Shutdown()
{
	mpGame->Shutdown();
	
	UnloadGameModule();
};

void SbGameFramework::LoadGameModule()
{
	mnGameLib = mSystem.get().LoadLib("SbGame");
	
	if(!mnGameLib)
		return;
	
	using fnGetGameAPI = IGame *(*)();
	fnGetGameAPI pfnGetGameAPI{mSystem.get().GetLibSymbol<fnGetGameAPI>(mnGameLib, "GetGameAPI")};
	
	if(!pfnGetGameAPI)
		return;
	
	mpGame = pfnGetGameAPI();
};

void SbGameFramework::UnloadGameModule()
{
	mSystem.get().FreeLib(mnGameLib);
};

}; // namespace sbe::SbGameFramework