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

#include "SbSystemExternal.hpp"
#include "SbLibraryLoader.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

namespace sbe
{

SbSystemExternal::SbSystemExternal()
{
	LoadModule();
};

SbSystemExternal::~SbSystemExternal()
{
	UnloadModule();
};

void SbSystemExternal::Init()
{
	mpSystem->Init();
};

void SbSystemExternal::Shutdown()
{
	mpSystem->Shutdown();
};

int SbSystemExternal::LoadLib(const char *asPath)
{
	return mpSystem->LoadLib(asPath);
};

void SbSystemExternal::FreeLib(int anHandle)
{
	return mpSystem->FreeLib(anHandle);
};

void *SbSystemExternal::GetLibSymbol(int anHandle, const char *asSymbol) const
{
	return mpSystem->GetLibSymbol(anHandle, asSymbol);
};

void SbSystemExternal::LoadModule()
{
	mnSystemLib = SbLibraryLoader::Load("SbSystem");
	
	if(!mnSystemLib)
		throw std::runtime_error("Failed to load the system module!");
	
	using fnGetSystemAPI = ISystem *(*)();
	fnGetSystemAPI pfnGetSystemAPI{SbLibraryLoader::GetSymbol<fnGetSystemAPI>(mnSystemLib, "GetSystemAPI")};
	
	if(!pfnGetSystemAPI)
		throw std::runtime_error("");
	
	mpSystem = pfnGetSystemAPI();
	
	if(!mpSystem)
		throw std::runtime_error("");
};

void SbSystemExternal::UnloadModule()
{
	SbLibraryLoader::Unload(mnSystemLib);
};

}; // namespace sbe