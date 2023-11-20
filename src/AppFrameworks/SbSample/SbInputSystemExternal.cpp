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

//*****************************************************************************

#include <stdexcept>
#include <functional>

#include "SbInputSystemExternal.hpp"

#include "CoreLibs/SbSystem/SbLibrary.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"
#include "AppFrameworks/UtilityLibs/SbInput/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe
{

SbInputSystemExternal::SbInputSystemExternal()
{
	LoadModule();
};

SbInputSystemExternal::~SbInputSystemExternal() = default;

void SbInputSystemExternal::LoadModule()
{
	mpInputLib = std::make_unique<SbLibrary>("SbInput");
	
	if(!mpInputLib)
		throw std::runtime_error("Failed to load the input module!");
	
	auto pfnGetInputAPI{mpInputLib->GetSymbol<GetInputAPI_t>("GetInputAPI")};
	
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

}; // namespace sbe