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

#pragma once

#include "SbGame/IGame.hpp"

namespace sbe
{
struct ISystem;
struct IRenderWorld;
struct ISoundWorld;
};

namespace f3goaty
{

class idPlayer;

class CGame : public sbe::IGame
{
public:
	CGame();
	
	// ---------------------- Public IGame Interface -------------------
	
	void Init() override;
	void Shutdown() override;
	
	void RunFrame(sbe::idUserCmdMgr &aCmdMgr, sbe::gameReturn_t &aGameReturn) override;
	void ClientRunFrame(sbe::idUserCmdMgr &aCmdMgr, bool abLastPredictFrame, sbe::gameReturn_t &aGameReturn) override;
	
	bool Draw(int anClientNum) override;
	
	void MapShutdown() override;
	
	void Shell_CreateMenu(bool abInGame) override;
	void Shell_Cleanup() override;
	void Shell_Show(bool abShow) override;
	void Shell_SyncWithSession() override;
	
	// ---------------------- End of IGame Interface -------------------

private: // TODO: these are private for now, make them public if they're called from outside
	// Initializes all map variables common to both save games and spawned games
	void LoadMap(const char *asMapName, int anRandSeed);
	
	int GetNextClientNum(int anCurrent) const;
	idPlayer *GetClientByNum(int anCurrent) const;
	
	idPlayer *GetLocalPlayer() const;
	
	/// Used to allow entities to know if they're being spawned during the initial spawn
	gameState_t GetGameState() const {return meState;}
	
	bool CheatsOk(bool abRequirePlayer = true);
private:
	void Clear();
	
	// spawn entities from the map file
	void SpawnMapEntities();
	
	// commons used by init, shutdown, and restart
	void MapPopulate();
	void MapClear(bool abClearClients);
	
	void SpawnPlayer(int anClient);
private:
	sbe::ISystem &mSystem;
	sbe::IRenderSystem &mRenderSystem;
	sbe::ISoundSystem &mSoundSystem;
	
	IRenderWorld *mpGameRenderWorld{nullptr}; ///< All drawing is done to this world
	ISoundWorld *mpGameSoundWorld{nullptr}; ///< All audio goes to this world
	
	gameState_t meState{0};
};

}; // namespace f3goaty