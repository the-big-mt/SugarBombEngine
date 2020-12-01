/*
*******************************************************************************

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

//*****************************************************************************

#include <cstdlib>

#include "GameApp.hpp"
#include "SbGameExternal.hpp"
#include "SbGameFrameworkExternal.hpp"

//*****************************************************************************

// TODO: delete mpFramework; mpFramework = nullptr;

sbe::IGameFramework *CreateGameFramework()
{
#ifndef SBE_GAMEFRAMEWORK_HARD_LINKED
	static sbe::SbGameFrameworkExternal SbGameFrameworkModule();
	return SbGameFrameworkModule.GetGameFramework();
#else
	return new sbe::SbGameFramework::SbGameFramework();
#endif
};

sbe::IGame *CreateGame()
{
#ifndef SBE_GAME_HARD_LINKED
	static sbe::SbGameExternal SbGameModule();
	return SbGameModule.GetGame();
#else
	return new sbe::SbGame::SbGame();
#endif
};

int sbe::SbApplication::Main(int argc, char **argv)
{
	sbe::IGame &Game = CreateGame();
	sbe::IGameFramework &GameFramework = CreateGameFramework();
	
	f3goaty::CGameApp App(/*GameFramework,*/ Game, argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};