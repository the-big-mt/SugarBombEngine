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

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"
#include "SbRenderSystemExternal.hpp"
#include "SbInputSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

SbClientApp::SbClientApp(ISystem *apSystem, int argc, char **argv) : SbApplication(apSystem, argc, argv)
{
	mpRenderSystem = CreateRenderSystem();
	
	mpRenderSystem->Init();
	
	mpInputSystem = CreateInputSystem();
	
	mpInputSystem->Init();
};

SbClientApp::~SbClientApp()
{
	mpInputSystem->Shutdown();
	
	// TODO: delete mpInputSystem; mpInputSystem = nullptr;
	
	mpRenderSystem->Shutdown();
	
	// TODO: delete mpRenderSystem; mpRenderSystem = nullptr;
};

sbe::IRenderSystem *SbClientApp::CreateRenderSystem()
{
#ifndef SBE_SINGLE_BINARY
	return new sbe::SbRenderSystemExternal(*mpSystem);
#else
	return new sbe::SbRenderer::SbRenderSystem();
#endif
};

sbe::IInputSystem *SbClientApp::CreateInputSystem()
{
#ifndef SBE_SINGLE_BINARY
	return new sbe::SbInputSystemExternal(*mpSystem);
#else
	return new sbe::SbInput::SbInputSystem();
#endif
};