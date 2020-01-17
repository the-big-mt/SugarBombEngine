/*
*******************************************************************************

Copyright (C) 2020 SugarBombEngine Developers

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

#include <functional>

#include "SbPhysicsExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbPhysics/SbPhysicsSystem.hpp"
#include "SbPhysics/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe::SbGameFramework
{

SbPhysicsExternal::SbPhysicsExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbPhysicsExternal::~SbPhysicsExternal()
{
	UnloadModule();
};

void SbPhysicsExternal::LoadModule()
{
	mnPhysicsLib = mSystem.LoadLib("SbPhysics");
	
	if(!mnPhysicsLib)
		return;
	
	GetPhysicsAPI_t pfnGetPhysicsAPI{mSystem.GetLibSymbol<GetPhysicsAPI_t>(mnPhysicsLib, "GetPhysicsAPI")};
	
	if(!pfnGetPhysicsAPI)
		return;
	
	physicsImport_t ModuleImports{};
	ModuleImports.version = PHYSICS_API_VERSION;
	ModuleImports.sys = std::addressof(mSystem);
	auto ModuleExports{pfnGetPhysicsAPI(&ModuleImports)};
	
	if(!ModuleExports)
		return;
	
	mpPhysicsSystem = ModuleExports->physicsSystem;
	
	if(!mpPhysicsSystem)
		return;
};

void SbPhysicsExternal::UnloadModule()
{
	mSystem.FreeLib(mnPhysicsLib);
};

}; // namespace sbe::SbGameFramework