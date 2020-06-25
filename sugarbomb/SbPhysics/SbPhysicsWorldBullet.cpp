/*
*******************************************************************************

Copyright (C) 2008-2018 OpenMW contributors
Copyright (C) 2020 SugarBombEngine Developers

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

#include "SbPhysicsWorldBullet.hpp"

#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

//*****************************************************************************

namespace sbe::SbPhysics
{

SbPhysicsWorldBullet::SbPhysicsWorldBullet()
{
	mpCollisionConfiguration = new btDefaultCollisionConfiguration();
	mpDispatcher = new btCollisionDispatcher(mpCollisionConfiguration);
	mpBroadphase = new btDbvtBroadphase();

	mpCollisionWorld = new btCollisionWorld(mpDispatcher, mpBroadphase, mpCollisionConfiguration);
};

SbPhysicsWorldBullet::~SbPhysicsWorldBullet()
{
	delete mpCollisionWorld;
	delete mpBroadphase;
	delete mpDispatcher;
	delete mpCollisionConfiguration;
};

}; // namespace sbe::SbPhysics