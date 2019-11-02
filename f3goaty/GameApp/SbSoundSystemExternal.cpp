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

#include <stdexcept>
#include <functional>

#include "SbSoundSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbSound/ISoundSystem.hpp"
#include "CoreLibs/SbSound/SbModuleAPI.hpp"

namespace sbe
{

SbSoundSystemExternal::SbSoundSystemExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbSoundSystemExternal::~SbSoundSystemExternal()
{
	UnloadModule();
};

void SbSoundSystemExternal::LoadModule()
{
	mnSoundLib = mSystem.LoadLib("SbSound");
	
	if(!mnSoundLib)
		throw std::runtime_error("Failed to load the sound module!");
	
	GetSoundAPI_t pfnGetSoundAPI{mSystem.GetLibSymbol<GetSoundAPI_t>(mnSoundLib, "GetSoundAPI")};
	
	if(!pfnGetSoundAPI)
		throw std::runtime_error("No \"GetSoundAPI\" exported symbol found inside the sound module! Did you forget to export it?");
	
	soundImport_t ModuleImports{};
	ModuleImports.version = SOUND_API_VERSION;
	ModuleImports.sys = std::addressof(mSystem);
	auto ModuleExports{pfnGetSoundAPI(&ModuleImports)};
	
	if(!ModuleExports)
		throw std::runtime_error("");
	
	mpSoundSystem = ModuleExports->soundSystem;
	
	if(!mpSoundSystem)
		throw std::runtime_error("Couldn't get a valid pointer to the sound system interface!");
};

void SbSoundSystemExternal::UnloadModule()
{
	mSystem.FreeLib(mnSoundLib);
};

}; // namespace sbe