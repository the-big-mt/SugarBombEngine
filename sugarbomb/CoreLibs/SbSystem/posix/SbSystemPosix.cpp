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

#include <dlfcn.h>

#include "SbSystem.hpp"

namespace sbe::SbSystem
{

int SbSystem::LoadLib(const char *asPath)
{
	return dlopen(asPath, RTLD_NOW);
};

void SbSystem::FreeLib(int anHandle)
{
	dlclose(anHandle);
};

void *SbSystem::GetLibSymbol(int anHandle, const char *asSymbol) const
{
	return dlsym(anHandle, asSymbol);
};

}; // namespace sbe::SbSystem