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

#include <cstdio> // TODO: temp
#include <cstdlib> // TODO: temp
#include <cstdarg>

#include "SbSystem.hpp"

#include "AppFrameworks/SbLibraryLoader/SbLibraryLoader.hpp"

//*****************************************************************************

namespace sbe::SbSystem
{

void SbSystem::Init()
{
};

void SbSystem::Shutdown()
{
};

intptr_t SbSystem::LoadLib(const char *asPath)
{
	return SbLibraryLoader::Load(asPath);
};

void SbSystem::FreeLib(intptr_t anHandle)
{
	SbLibraryLoader::Free(anHandle);
};

void *SbSystem::GetLibSymbol(intptr_t anHandle, const char *asSymbol) const
{
	return SbLibraryLoader::GetSymbol(anHandle, asSymbol);
};

void SbSystem::Printf(const char *asMsg, ...)
{
	va_list lstArgs{};
	char sFmtMsg[1024]{}; // TODO: fix magic
	
	va_start(lstArgs, asMsg);
	vsprintf(sFmtMsg, asMsg, lstArgs);
	va_end(lstArgs);
	
	//for(auto It : mvOutputHandlers)
		//It->Print(sFmtMsg);
	
	// TODO: temp
	printf(sFmtMsg);
};

void SbSystem::Warning(const char *asMsg, ...)
{
	// TODO
};

void SbSystem::Error(const char *asMsg, ...)
{
	// TODO
};

void SbSystem::FatalError(const char *asMsg, ...)
{
	Shutdown();
	exit(EXIT_FAILURE);
};

}; // sbe::SbSystem