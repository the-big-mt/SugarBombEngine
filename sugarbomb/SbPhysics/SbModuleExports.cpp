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
/// @brief SbPhysics module exports

//*****************************************************************************

#include <functional>

#include "SbPhysics/SbModuleAPI.hpp"

#include "SbPhysicsSystemBullet.hpp"

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

C_EXPORT sbe::physicsExport_t *GetPhysicsAPI(sbe::physicsImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::PHYSICS_API_VERSION)
	{
		static sbe::ISystem &System = *apModuleImports->sys;
		static sbe::SbPhysics::SbPhysicsSystemBullet PhysicsSystem(System);
		
		static sbe::physicsExport_t ModuleExports;
		
		ModuleExports.version = sbe::PHYSICS_API_VERSION;
		ModuleExports.physicsSystem = std::addressof(PhysicsSystem);
		
		return &ModuleExports;
	};
	
	return nullptr;
};