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

#pragma once

//*****************************************************************************

namespace sbe
{

constexpr auto PHYSICS_API_VERSION{1};

struct ISystem;
struct SbPhysicsSystem;

struct physicsImport_t
{
	int version{-1};
	
	ISystem *sys{nullptr};
};

struct physicsExport_t
{
	int version{-1};
	
	SbPhysicsSystem *physicsSystem{nullptr};
};

extern "C" using GetPhysicsAPI_t = physicsExport_t *(*)(physicsImport_t *apModuleImports);

}; // namespace sbe