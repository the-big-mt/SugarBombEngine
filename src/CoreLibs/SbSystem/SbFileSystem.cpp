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

#include "SbFileSystem.hpp"
#include "SbFile.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbSystem
{

SbFileSystem::SbFileSystem(ISystem &aSystem) : mSystem(aSystem){}

void SbFileSystem::Init(const char *asBasePath, bool abFileLogging, bool abUseArchives)
{
};

void SbFileSystem::Shutdown()
{
};

IFile *SbFileSystem::OpenFile(const char *asPath)
{
	return nullptr;
};

void SbFileSystem::CloseFile(IFile *apFile)
{
	if(!apFile)
		return;
};

}; // namespace sbe::SbSystem