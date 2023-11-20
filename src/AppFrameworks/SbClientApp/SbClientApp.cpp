/*
*******************************************************************************

Copyright (C) 2019-2020, 2023 SugarBombEngine Developers

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

//#include "precompiled.h"
#include <cassert>

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"
#include "AppFrameworks/SbClientApp/SbWindow.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

//*****************************************************************************

namespace sbe
{

SbClientApp::SbClientApp(const char *asWindowTitle, int anWindowWidth, int anWindowHeight, bool abWindowFullScreen,
	IRenderSystem &aRenderSystem, IInputSystem &aInputSystem, SbSystem &aSystem, int argc, char **argv)
	: SbApplication(aSystem, argc, argv), mRenderSystem(aRenderSystem), mInputSystem(aInputSystem)
{
	mpWindow.reset(CreateMainWindow(asWindowTitle, anWindowWidth, anWindowHeight, abWindowFullScreen));
	
	mRenderSystem.Init(*mpWindow.get());
	mInputSystem.Init(*mpWindow.get());
	
	mnUPS = 60;
};

SbClientApp::~SbClientApp()
{
	mInputSystem.Shutdown();
	mRenderSystem.Shutdown();
};

void SbClientApp::Run()
{
	assert(mbInitialized);

	double fOldTime{mSystem.GetRealTime()};
	double fLagTime{0.0};
	
	while(!CloseRequested())
	{
		if(PreFrame()) // Update input
		{
			assert(mnUPS > 0);
			
			double fCurTime{mSystem.GetRealTime()};
			double fElapsedTime{fCurTime - fOldTime};

			// TODO: what if elapsed time is too big?
			
			fLagTime += fElapsedTime;
			
			while(fLagTime >= GetTimeStep())
			{
				RunFrame(GetTimeStep());
				fLagTime -= GetTimeStep();
			};
			
			PostFrame(fLagTime / GetTimeStep()); // Render stuff if needed
			
			fOldTime = fCurTime;
			
			// TODO: sleep?
		};
	};
};

void SbClientApp::PostFrame()
{
	RenderFrame();
};

void SbClientApp::RenderFrame()
{
	if(PreRender())
	{
		Render();
		PostRender();
	};
};

void SbClientApp::SetMaxUPS(int anUPS)
{
	assert(anUPS > 0);
	
	mnUPS = anUPS;
};

}; // namespace sbe