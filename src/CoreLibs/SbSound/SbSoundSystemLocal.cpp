/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2020, 2023 SugarBombEngine Developers

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

//#pragma hdrstop

//#include "precompiled.h"

#include "SbSoundSystemLocal.hpp"
#include "SbSoundWorldLocal.hpp"
#include "SbSoundHardware.hpp"

#include <CoreLibs/SbSystem/SbSystem.hpp>

//*****************************************************************************

namespace sbe::SbSound
{

/*
*******************************************************************************

idSoundSystemLocal

*******************************************************************************
*/

SbSoundSystemLocal::SbSoundSystemLocal(SbSystem &aSystem, SbSoundHardware &aHardware)
	: mSystem(aSystem), mHardware(aHardware){}

/*
========================
idSoundSystemLocal::Init

Initialize the SoundSystem.
========================
*/
void SbSoundSystemLocal::Init(bool abUseCompression, int anMaxSamples)
{
	mSystem.Printf( "----- Initializing Sound System ------\n" );
	
#ifndef SBE_SINGLE_BINARY
	// initialize idLib
	//idLib::Init(); // TODO

	// register static cvars declared in the module
	//idCVar::RegisterStaticVars();
#endif
	
	//soundTime = Sys_Milliseconds();
	//random.SetSeed(soundTime);
	
	//if(!s_noSound.GetBool())
	{
		mHardware.Init();
		//InitStreamBuffers();
	};
	
	//mpCmdSystem->AddCommand( "testSound", TestSound_f, 0, "tests a sound", idCmdSystem::ArgCompletion_SoundName );
	//mpCmdSystem->AddCommand( "s_restart", RestartSound_f, 0, "restart sound system" );
	//mpCmdSystem->AddCommand( "listSamples", ListSamples_f, 0, "lists all loaded sound samples" );
	
	mSystem.Printf( "sound system initialized.\n" );
	mSystem.Printf( "--------------------------------------\n" );
};

/*
========================
idSoundSystemLocal::Shutdown
========================
*/
void SbSoundSystemLocal::Shutdown()
{
	mHardware.Shutdown();
	//FreeStreamBuffers();
	//samples.DeleteContents(true);
	//sampleHash.Free();
};

/*
========================
idSoundSystemLocal::Render
========================
*/
void SbSoundSystemLocal::Update(float afTimeStep)
{
	//if(s_noSound.GetBool()) // TODO
		//return; // TODO
	
	//if(needsRestart) // TODO
	{
		//needsRestart = false; // TODO
		//Restart(); // TODO
	};
	
	//SCOPED_PROFILE_EVENT( "SoundSystem::Render" ); // TODO
	
	if(mpActiveWorld)
		mpActiveWorld->Update(afTimeStep);
	
	mHardware.Update();
	
	// The sound system doesn't use game time or anything like that because the sounds are decoded in real time.
	//soundTime = Sys_Milliseconds(); // TODO
};

SbSoundWorld *SbSoundSystemLocal::AllocWorld()
{
	auto pWorld{new SbSoundWorldLocal(mSystem)};
	mlstWorlds.push_back(pWorld);
	return pWorld;
};

void SbSoundSystemLocal::FreeWorld(SbSoundWorld *apWorld)
{
	if(apWorld)
	{
		delete apWorld;
		apWorld = nullptr;
	};
};

void SbSoundSystemLocal::SetPlayingWorld(SbSoundWorld *apWorld)
{
	if(apWorld)
		mpActiveWorld = dynamic_cast<SbSoundWorldLocal*>(apWorld);
};

SbSoundWorld *SbSoundSystemLocal::GetPlayingWorld() const
{
	return mpActiveWorld;
};

void SbSoundSystemLocal::BeginLevelLoad()
{
};

void SbSoundSystemLocal::EndLevelLoad()
{
};

}; // namespace sbe::SbSound