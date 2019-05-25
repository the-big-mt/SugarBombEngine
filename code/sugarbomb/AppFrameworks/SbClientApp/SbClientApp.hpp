/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

#include "SbApplication/SbApplication.hpp"

#ifdef _WIN32
using HWND = void*;
#endif

namespace sbe
{
struct IRenderSystem;
struct IInputSystem;
};

struct sysEvent_t;

// event generation
void Sys_GenerateEvents();
sysEvent_t Sys_GetEvent();
void Sys_ClearEvents();

class SbClientApp : public SbApplication
{
public:
	SbClientApp();
	virtual ~SbClientApp();
private:
	static constexpr auto sDefaultAppTitle{"SugarBombEngine Client Application"};
	static constexpr auto nDefaultWindowSizeW{1280};
	static constexpr auto nDefaultWindowSizeH{600};
	
	bool Init() override;
	
	void Frame() override;
	
	void CreateMainWindow(int anWidth = nDefaultWindowSizeW, int anHeight = nDefaultWindowSizeH, const char *asTitle = sDefaultAppTitle, bool abFullScreen = false);
	
	void InitRenderSystem();
	void ShutdownRenderSystem();
	
	void InitInputSystem();
	void ShutdownInputSystem();
	
	sbe::IRenderSystem *renderSystem{nullptr};
	sbe::IInputSystem *inputSystem{nullptr};
	
	int renderDLL{0};
	int inputDLL{0};
	
#ifdef _WIN32
	HWND mhWnd{nullptr};
#else
	void *mpWindow{nullptr};
#endif
protected:
	virtual bool PreInputUpdate(){return true;}
	virtual void InputUpdate();
	virtual void PostInputUpdate(){}
	
	virtual bool PreLogicUpdate(){return true;}
	virtual void LogicUpdate(){}
	virtual void PostLogicUpdate(){}
	
	virtual bool PreRender();
	virtual void Render();
	virtual void PostRender(){}
};