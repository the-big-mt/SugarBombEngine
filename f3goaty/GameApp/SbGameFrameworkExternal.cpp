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

//*****************************************************************************

#include <functional>

#include "SbGameFrameworkExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbGameFramework/IGameFramework.hpp"
#include "SbGameFramework/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe
{

SbGameFrameworkExternal::SbGameFrameworkExternal(ISystem &aSystem, IRenderSystem *apRenderSystem, ISoundSystem *apSoundSystem) : mSystem(aSystem)
{
	LoadModule(apRenderSystem, apSoundSystem);
};

SbGameFrameworkExternal::~SbGameFrameworkExternal()
{
	UnloadModule();
};

void SbGameFrameworkExternal::LoadModule(IRenderSystem *apRenderSystem, ISoundSystem *apSoundSystem)
{
	mnFrameworkLib = mSystem.LoadLib("SbGameFramework");
	
	if(!mnFrameworkLib)
		throw std::runtime_error("Failed to load the game framework module!");

	GetGameFrameworkAPI_t pfnGetGameFrameworkAPI{mSystem.GetLibSymbol<GetGameFrameworkAPI_t>(mnFrameworkLib, "GetGameFrameworkAPI")};
	
	if(!pfnGetGameFrameworkAPI)
		throw std::runtime_error("No \"GetGameFrameworkAPI\" exported symbol found inside the game framework module! Did you forget to export it?");
	
	gameFrameworkImport_t ModuleImports{};
	ModuleImports.version = GAMEFRAMEWORK_API_VERSION;
	ModuleImports.sys = std::addressof(mSystem);
	ModuleImports.renderSystem = apRenderSystem;
	ModuleImports.soundSystem = apSoundSystem;
	auto ModuleExports{pfnGetGameFrameworkAPI(&ModuleImports)};
	
	if(!ModuleExports)
		throw std::runtime_error("");
	
	mpFramework = ModuleExports->gameFramework;
	
	if(!mpFramework)
		throw std::runtime_error("Couldn't get a valid pointer to the game framework interface!");
};

void SbGameFrameworkExternal::UnloadModule()
{
	mSystem.FreeLib(mnFrameworkLib);
};

}; // namespace sbe