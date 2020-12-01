/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

#include <iniparser.h>

#include "Game.hpp"

//#include "CoreLibs/SbSystem/ISystem.hpp"

//#include "CoreLibs/SbSound/ISoundWorld.hpp"

namespace f3goaty
{

CGame::CGame()
{
	Clear();
};

void CGame::Init()
{
	auto pDict{iniparser_load("FalloutPrefs.ini")}; // Fallout_default
	
	const char *sWindowTitle{"F3GOATY"};
	int nWindowWidth{1280};
	int nWindowHeight{600};
	bool bWindowFullScreen{false};
	
	if(pDict != nullptr)
	{
		nWindowWidth = iniparser_getint(pDict, "Display:iSize W", 1280);
		nWindowHeight = iniparser_getint(pDict, "Display:iSize H", 600);
		bWindowFullScreen = iniparser_getboolean(pDict, "Display:bFull Screen", false);
		bWindowBorder = iniparser_getboolean(pDict, "Display:bBorder", true);
	};
	
	//sbe::IGameFramework *pGameFramework = CreateGameFramework(System);
	
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
	if(mpGameRenderWorld == nullptr)
		return;
};

void CGame::ClientRunFrame(sbe::idUserCmdMgr &aCmdMgr, bool abLastPredictFrame, sbe::gameReturn_t &aGameReturn)
{
};

bool CGame::Draw(int anClient)
{
	if(anClient == -1)
		return false;

	return true;
};

void CGame::MapShutdown()
{
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
{
	if(mpShellHandler == nullptr)
		return;
	
	switch(mpSession->GetState())
	{
	};
};

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