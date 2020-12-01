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
#include <functional>

#include "SbInputSystemExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"
#include "AppFrameworks/UtilityLibs/SbInput/SbModuleAPI.hpp"

namespace sbe
{

SbInputSystemExternal::SbInputSystemExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbInputSystemExternal::~SbInputSystemExternal()
{
	UnloadModule();
};

void SbInputSystemExternal::LoadModule()
{
	mnInputLib = mSystem.LoadLib("SbInput");
	
	if(!mnInputLib)
		throw std::runtime_error("Failed to load the input module!");
	
	GetInputAPI_t pfnGetInputAPI{mSystem.GetLibSymbol<GetInputAPI_t>(mnInputLib, "GetInputAPI")};
	
	if(!pfnGetInputAPI)
		throw std::runtime_error("");
	
	inputImport_t ModuleImports{};
	ModuleImports.version = INPUT_API_VERSION;
	ModuleImports.sys = std::addressof(mSystem);
	auto ModuleExports{pfnGetInputAPI(&ModuleImports)};
	
	if(!ModuleExports)
		throw std::runtime_error("");
	
	mpInputSystem = ModuleExports->inputSystem;
	
	if(!mpInputSystem)
		throw std::runtime_error("");
};

void SbInputSystemExternal::UnloadModule()
{
	mSystem.FreeLib(mnInputLib);
};

}; // namespace sbe