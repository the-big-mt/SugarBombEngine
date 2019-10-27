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

#include <functional>

#include "SbGameFrameworkExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbGameFramework/IGameFramework.hpp"

namespace sbe
{

SbGameFrameworkExternal::SbGameFrameworkExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbGameFrameworkExternal::~SbGameFrameworkExternal()
{
	UnloadModule();
};

void SbGameFrameworkExternal::Init()
{
	mpFramework->Init();
};

void SbGameFrameworkExternal::Shutdown()
{
	mpFramework->Shutdown();
};

void SbGameFrameworkExternal::LoadModule()
{
	mnFrameworkLib = mSystem.LoadLib("SbGameFramework");
	
	if(!mnFrameworkLib)
		throw std::runtime_error("Failed to load the game framework module!");
	
	using fnGetGameFrameworkAPI = IGameFramework *(*)(ISystem *apSystem);
	fnGetGameFrameworkAPI pfnGetGameFrameworkAPI{mSystem.GetLibSymbol<fnGetGameFrameworkAPI>(mnFrameworkLib, "GetGameFrameworkAPI")};
	
	if(!pfnGetGameFrameworkAPI)
		throw std::runtime_error("No \"GetGameFrameworkAPI\" exported symbol found inside the game framework module! Did you forget to export it?");
	
	mpFramework = pfnGetGameFrameworkAPI(std::addressof(mSystem));
	
	if(!mpFramework)
		throw std::runtime_error("Couldn't get a valid pointer to the game framework interface!");
};

void SbGameFrameworkExternal::UnloadModule()
{
	mSystem.FreeLib(mnFrameworkLib);
};

}; // namespace sbe