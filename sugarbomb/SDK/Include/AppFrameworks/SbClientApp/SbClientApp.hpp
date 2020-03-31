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

#pragma once

#include <string>

#include "AppFrameworks/SbApplication/SbApplication.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;
struct IRenderSystem;
struct IInputSystem;
struct IWindow;

class SbClientApp : public SbApplication
{
public:
	SbClientApp(const char *asWindowTitle, int anWindowWidth, int anWindowHeight, bool abWindowFullScreen,
	IRenderSystem &aRenderSystem, IInputSystem &aInputSystem, ISystem &aSystem, int argc, char **argv);
	virtual ~SbClientApp();
private:
	IWindow *CreateMainWindow(const std::string &asTitle, int anWidth, int anHeight, bool abFullScreen); // TODO: make title use wide string

	bool PreFrame() override;
private:
	sbe::IWindow *mpWindow{nullptr};

	IRenderSystem &mRenderSystem;
	IInputSystem &mInputSystem;
};

}; // namespace sbe