/*
*******************************************************************************

Copyright (C) 2012-2014 Robert Beckebans
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

#include <windows.h>

#include "AppFrameworks/SbLibraryLoader/SbLibraryLoader.hpp"

//*****************************************************************************

namespace sbe
{

// RB: 64 bit fixes, changed int to intptr_t

intptr_t SbLibraryLoader::Load(const char *asPath)
{
	return reinterpret_cast<intptr_t>(LoadLibrary(asPath));
};

void SbLibraryLoader::Unload(intptr_t anHandle)
{
	FreeLibrary(reinterpret_cast<HMODULE>(anHandle));
};

void *SbLibraryLoader::GetSymbol(intptr_t anHandle, const char *asSymbol)
{
	return reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(anHandle), asSymbol));
};

}; // namespace sbe