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

#include <cstdio>

#include "Game.hpp"

namespace f3goaty
{

void CGame::Init()
{
	mSystem.Printf("Hello Game!\n");
	
	Clear();
	
	InitConsoleCommands();
};

void CGame::Shutdown()
{
	Shell_Cleanup();
	
	MapShutdown();
	
	ShutdownConsoleCommands();
	
	// free memory allocated by class objects
	Clear();

#ifdef GAME_DLL
	// remove auto-completion function pointers pointing into this DLL
	cvarSystem->RemoveFlaggedAutoCompletion( CVAR_GAME );
	
	// enable leak test
	Mem_EnableLeakTest( "game" );
	
	// shutdown idLib
	idLib::ShutDown();
#endif
};

void CGame::RunFrame(sbe::idUserCmdMgr &aCmdMgr, sbe::gameReturn_t &aGameReturn)
{
};

void CGame::ClientRunFrame(sbe::idUserCmdMgr &aCmdMgr, bool abLastPredictFrame, sbe::gameReturn_t &aGameReturn)
{
};

bool CGame::Draw(int anClientNum)
{
	return false;
};

void CGame::Shell_CreateMenu(bool abInGame)
{
};

void CGame::Shell_Cleanup()
{
};

void CGame::Shell_Show(bool abShow)
{
};

void CGame::Shell_SyncWithSession()

/*
===================
idGameLocal::LoadMap

Initializes all map variables common to both save games and spawned games.
===================
*/
void CGame::LoadMap(const char *asMapName, int anRandSeed)
{
	// clear the sound system
	mpGameSoundWorld->ClearAllSoundEmitters();
};

int CGame::GetNextClientNum(int anCurrent) const
{
};

idPlayer *CGame::GetClientByNum(int anCurrent) const
{
};

idPlayer *CGame::GetLocalPlayer() const
{
};

// PRIVATE METHODS START

void CGame::Clear()
{
};

void CGame::SpawnMapEntities()
{
};

void CGame::MapPopulate()
{
	// parse the key/value pairs and spawn entities
	SpawnMapEntities();
};

void CGame::MapClear(bool abClearClients)
{
};

void CGame::SpawnPlayer(int anClient)
{
};

}; // namespace f3goaty