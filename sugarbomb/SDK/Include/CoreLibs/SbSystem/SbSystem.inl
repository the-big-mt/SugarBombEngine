/*
*******************************************************************************

Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2020 SugarBombEngine Developers

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

#pragma once

#ifdef _WIN32
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

//*****************************************************************************

namespace sbe
{

// RB: 64 bit fixes, changed int to intptr_t

#ifdef _WIN32
intptr_t SbSystem::LoadLib(const char *asPath)
{
	return reinterpret_cast<intptr_t>(dlopen(asPath, RTLD_NOW));
};

void SbSystem::FreeLib(intptr_t anHandle)
{
	dlclose(reinterpret_cast<void*>(anHandle));
};

void *SbSystem::GetLibSymbol(intptr_t anHandle, const char *asSymbol)
{
	return dlsym(reinterpret_cast<void *>(anHandle), asSymbol);
};
#else
intptr_t SbSystem::LoadLib(const char *asPath)
{
	return reinterpret_cast<intptr_t>(LoadLibrary(asPath));
};

void SbSystem::FreeLib(intptr_t anHandle)
{
	if(!anHandle)
		return;
	
	FreeLibrary(reinterpret_cast<HMODULE>(anHandle));
};

void *SbSystem::GetLibSymbol(intptr_t anHandle, const char *asSymbol)
{
	return reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(anHandle), asSymbol));
};
#endif

}; // namespace sbe