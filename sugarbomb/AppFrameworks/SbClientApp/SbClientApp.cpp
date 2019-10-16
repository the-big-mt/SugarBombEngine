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

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

SbClientApp::SbClientApp(int argc, char **argv) : SbApplication(argc, argv)
{
	LoadRenderSystemModule();
	
	mpRenderSystem->Init();
	
	LoadInputSystemModule();
	
	mpInputSystem->Init();
};

SbClientApp::~SbClientApp()
{
	mpInputSystem->Shutdown();
	
	UnloadInputSystemModule();
	
	mpRenderSystem->Shutdown();
	
	UnloadRenderSystemModule();
};

void SbClientApp::LoadRenderSystemModule()
{
	mnRenderSystemLib = mpSystem->LoadLib("SbGLCoreRenderer");
	
	if(!mnRenderSystemLib)
		throw std::runtime_error("Failed to load the renderer module!");
	
	using fnGetRenderSystemAPI = sbe::IRenderSystem *(*)();
	fnGetRenderSystemAPI pfnGetRenderSystemAPI{mpSystem->GetLibSymbol<fnGetRenderSystemAPI>(mnRenderSystemLib, "GetRenderSystemAPI")};
	
	if(!pfnGetRenderSystemAPI)
		throw std::runtime_error("");
	
	mpRenderSystem = pfnGetRenderSystemAPI();
	
	if(!mpRenderSystem)
		throw std::runtime_error("");
};

void SbClientApp::UnloadRenderSystemModule()
{
	mpSystem->FreeLib(mnRenderSystemLib);
};

void SbClientApp::LoadInputSystemModule()
{
	mnInputSystemLib = mpSystem->LoadLib("SbInput");
	
	if(!mnInputSystemLib)
		throw std::runtime_error("Failed to load the input module!");
	
	using fnGetInputSystemAPI = sbe::IInputSystem *(*)();
	fnGetInputSystemAPI pfnGetInputSystemAPI{mpSystem->GetLibSymbol<fnGetInputSystemAPI>(mnInputSystemLib, "GetInputSystemAPI")};
	
	if(!pfnGetInputSystemAPI)
		throw std::runtime_error("");
	
	mpInputSystem = pfnGetInputSystemAPI();
	
	if(!mpInputSystem)
		throw std::runtime_error("");
};

void SbClientApp::UnloadInputSystemModule()
{
	mpSystem->FreeLib(mnInputSystemLib);
};