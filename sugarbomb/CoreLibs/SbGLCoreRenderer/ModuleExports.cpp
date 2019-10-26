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

#include "SbRenderSystem.hpp"
#include "CoreLibs/SbRenderer/SbModuleAPI.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

C_EXPORT sbe::renderExport_t *GetRenderSystemAPI(sbe::renderImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::RENDER_API_VERSION)
	{
		static sbe::SbRenderer::SbRenderSystem RenderSystem(apModuleImports->sys);
		
		static sbe::renderExport_t ModuleExports;
		
		ModuleExports.version = sbe::RENDER_API_VERSION;
		ModuleExports.renderSystem = &RenderSystem;
		
		return &ModuleExports;
	};
	
	return nullptr;
};