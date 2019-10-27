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

#include "SbSoundSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

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

void SbSoundSystemExternal::Init(bool abUseCompression, int anMaxSamples)
{
	mpSoundSystem->Init(abUseCompression, anMaxSamples);
};

void SbSoundSystemExternal::Shutdown()
{
	mpSoundSystem->Shutdown();
};

void SbSoundSystemExternal::LoadModule()
{
	mnSoundLib = mSystem.LoadLib("SbSound");
	
	if(!mnSoundLib)
		throw std::runtime_error("Failed to load the sound module!");
	
	using fnGetSoundSystemAPI = ISoundSystem *(*)();
	fnGetSoundSystemAPI pfnGetSoundSystemAPI{mSystem.GetLibSymbol<fnGetSoundSystemAPI>(mnSoundLib, "GetSoundSystemAPI")};
	
	if(!pfnGetSoundSystemAPI)
		throw std::runtime_error("No \"GetSoundSystemAPI\" exported symbol found inside the sound module! Did you forget to export it?");
	
	mpSoundSystem = pfnGetSoundSystemAPI();
	
	if(!mpSoundSystem)
		throw std::runtime_error("Couldn't get a valid pointer to the sound system interface!");
};

void SbSoundSystemExternal::UnloadModule()
{
	mSystem.FreeLib(mnSoundLib);
};

}; // namespace sbe