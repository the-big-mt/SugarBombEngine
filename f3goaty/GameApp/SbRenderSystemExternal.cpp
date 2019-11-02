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
#include "CoreLibs/SbRenderer/SbModuleAPI.hpp"

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
	mnRenderLib = mSystem.LoadLib("SbGLCoreRenderer");
	
	if(!mnRenderLib)
		throw std::runtime_error("Failed to load the renderer module!");
	
	GetRendererAPI_t pfnGetRendererAPI{mSystem.GetLibSymbol<GetRendererAPI_t>(mnRenderLib, "GetRendererAPI")};
	
	if(!pfnGetRendererAPI)
		throw std::runtime_error("");
	
	rendererImport_t ModuleImports{};
	ModuleImports.version = RENDERER_API_VERSION;
	auto ModuleExports{pfnGetRendererAPI(&ModuleImports)};
	
	if(!ModuleExports)
		throw std::runtime_error("");
	
	mpRenderSystem = ModuleExports->renderSystem;
	
	if(!mpRenderSystem)
		throw std::runtime_error("");
};

void SbRenderSystemExternal::UnloadModule()
{
	mSystem.FreeLib(mnRenderLib);
};

}; // namespace sbe