/*
*******************************************************************************

Copyright (C) 2019-2020, 2022 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @brief client-side application framework

//*****************************************************************************

#pragma once

#include <string>
#include <memory>

#include "AppFrameworks/SbApplication/SbApplication.hpp"

//*****************************************************************************

namespace sbe
{

struct SbSystem;
struct IRenderSystem;
struct IInputSystem;
struct IWindow;

class SbClientApp : public SbApplication
{
public:
	SbClientApp(const char *asWindowTitle, int anWindowWidth, int anWindowHeight, bool abWindowFullScreen,
	IRenderSystem &aRenderSystem, IInputSystem &aInputSystem, SbSystem &aSystem, int argc, char **argv);
	virtual ~SbClientApp();
	
	void Run() override;
protected:
	/**
	 * This method is getting called before the start of each frame,
	 * override it if you want to prevent the frame execution under some conditions
	 *
	 * @return true if the frame is allowed to be executed, false otherwise
	 */
	//virtual bool PreFrame(){return true;}

	/// This method should contain stuff that needs to be executed each frame
	//virtual void RunFrame(){}

	/// This method can be used to do things at the end of each frame
	//virtual void PostFrame(){}
private:
	IWindow *CreateMainWindow(const std::string &asTitle, int anWidth, int anHeight, bool abFullScreen); // TODO: make title use wide string

	bool PreFrame();
	//void RunFrame();
	void PostFrame();
	
	void RenderFrame();
	
	bool PreRender(){return true;}
	void Render(){}
	void PostRender(){}
	
	float GetTimeStep() const {return 1.0f / mnUPS;}
private:
	std::unique_ptr<IWindow> mpWindow;

	IRenderSystem &mRenderSystem;
	IInputSystem &mInputSystem;
	
	int mnUPS{0};
};

}; // namespace sbe