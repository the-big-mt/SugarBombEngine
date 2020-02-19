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

#include <stdexcept>
#include "IWindow.hpp"

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

//*****************************************************************************

SbClientApp::SbClientApp(const char *asWindowTitle, int anWindowWidth, int anWindowHeight, bool abWindowFullScreen,
	sbe::IRenderSystem *apRenderSystem, sbe::IInputSystem *apInputSystem, sbe::ISystem *apSystem, int argc, char **argv)
	: SbApplication(apSystem, argc, argv), mpRenderSystem(apRenderSystem), mpInputSystem(apInputSystem)
{
	mpWindow = CreateMainWindow(asWindowTitle, anWindowWidth, anWindowHeight, abWindowFullScreen);
	
	mpRenderSystem->Init();
	mpInputSystem->Init();
};

SbClientApp::~SbClientApp()
{
	mpInputSystem->Shutdown();
	mpRenderSystem->Shutdown();
	
	delete mpWindow;
	mpWindow = nullptr;
};