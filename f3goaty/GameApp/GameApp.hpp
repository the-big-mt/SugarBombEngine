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

#pragma once

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct IGameFramework;
struct IGame;
};

namespace f3goaty
{

class CGameApp final : public SbClientApp
{
public:
	CGameApp(/*sbe::IGameFramework *apGameFramework,*/ sbe::IGame *apGame, int argc, char **argv);
	~CGameApp();
private:
	void RunFrame() override;
	
	void SetMaxUPS(int anUPS); // TODO: SetFrameRate?
	
	float GetTimeStep() const {return 1.0f / mnUPS;} // TODO
private:
	//sbe::IGameFramework *mpFramework{nullptr};
	sbe::IGame *mpGame{nullptr};
	
	int mnUPS{0};
};

}; // namespace f3goaty