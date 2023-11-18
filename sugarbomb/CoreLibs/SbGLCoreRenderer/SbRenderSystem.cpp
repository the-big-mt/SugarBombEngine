/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include "SbRenderSystem.hpp"
#include "SbRenderWorld.hpp"


//*****************************************************************************

namespace sbe::SbRenderer
{

void SbRenderSystem::Init()
{
	InitOpenGL();
};

void SbRenderSystem::Shutdown()
{
	ShutdownOpenGL();
};

IRenderWorld *SbRenderSystem::AllocWorld()
{
	return new SbRenderWorld();
};

void SbRenderSystem::FreeWorld(IRenderWorld *apWorld)
{
	if(apWorld)
	{
		delete apWorld;
		apWorld = nullptr;
	};
};

void SbRenderSystem::InitOpenGL()
{
	//GLimp_Init(); // TODO
};

void SbRenderSystem::ShutdownOpenGL()
{
	//GLimp_Shutdown(); // TODO
};

void SbRenderSystem::BeginLevelLoad()
{
};

void SbRenderSystem::EndLevelLoad()
{
};

}; // namespace sbe::SbRenderer