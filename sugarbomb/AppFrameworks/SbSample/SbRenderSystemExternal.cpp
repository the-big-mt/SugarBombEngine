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

#include "SbRenderSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

namespace sbe
{

SbRenderSystemExternal::SbRenderSystemExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbRenderSystemExternal::~SbRenderSystemExternal()
{
	UnloadModule();
};

void SbRenderSystemExternal::LoadModule()
{
	mnRenderSystemLib = mSystem.LoadLib("SbGLCoreRenderer");
	
	if(!mnRenderSystemLib)
		throw std::runtime_error("Failed to load the renderer module!");
	
	using fnGetRenderSystemAPI = IRenderSystem *(*)();
	fnGetRenderSystemAPI pfnGetRenderSystemAPI{mSystem.GetLibSymbol<fnGetRenderSystemAPI>(mnRenderSystemLib, "GetRenderSystemAPI")};
	
	if(!pfnGetRenderSystemAPI)
		throw std::runtime_error("");
	
	mpRenderSystem = pfnGetRenderSystemAPI();
	
	if(!mpRenderSystem)
		throw std::runtime_error("");
};

void SbRenderSystemExternal::UnloadModule()
{
	mSystem.FreeLib(mnRenderSystemLib);
};

}; // namespace sbe