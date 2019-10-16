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

#include <cassert>

#include "AppFrameworks/SbApplication/SbApplication.hpp"
#include "SbSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

SbApplication::SbApplication(int argc, char **argv)
{
	Init();
};

SbApplication::~SbApplication()
{
	mpSystem->Shutdown();
	
	// TODO: delete mpSystem; mpSystem = nullptr;
};

void SbApplication::Run()
{
	assert(mbInitialized);

	while(true)
	{
		RunFrame();
	};
};

void SbApplication::Init()
{
	mpSystem = CreateSystem();
	
	mpSystem->Init();
	
	mbInitialized = true;
};

sbe::ISystem *SbApplication::CreateSystem()
{
#ifndef SBE_SINGLE_BINARY
	return new sbe::SbSystemExternal();
#else
	return new sbe::SbSystem::SbSystem();
#endif
};

void SbApplication::RunFrame()
{
};