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

#include "GameApp.hpp"

#include "SbGameFramework/IGameFramework.hpp"

namespace f3goaty
{

CGameApp::CGameApp(sbe::IGameFramework *apGameFramework, int argc, char **argv)
	: SbClientApp(argc, argv),
	mpFramework(apGameFramework)
{
	mpFramework->Init();
	//if(!mpFramework->Init())
		//throw std::runtime_error("Couldn't initialize the game framework!");
};

CGameApp::~CGameApp()
{
	mpFramework->Shutdown();
};

void CGameApp::RunFrame()
{
	mpFramework->Frame(GetTimeStep());
};

}; // namespace f3goaty