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

#include "SbPhysicsSystemBullet.hpp"
#include "SbPhysicsWorldBullet.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbPhysics
{

SbPhysicsSystemBullet::SbPhysicsSystemBullet(ISystem &aSystem) : mSystem(aSystem){}

void SbPhysicsSystemBullet::Init()
{
};

void SbPhysicsSystemBullet::Shutdown()
{
};

SbPhysicsWorld *SbPhysicsSystemBullet::AllocWorld()
{
	return new SbPhysicsWorldBullet();
};

void SbPhysicsSystemBullet::FreeWorld(SbPhysicsWorld *apWorld)
{
	if(apWorld)
	{
		delete apWorld;
		apWorld = nullptr;
	};
};

}; // namespace sbe::SbPhysics