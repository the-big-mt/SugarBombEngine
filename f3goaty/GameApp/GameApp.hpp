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

#pragma once

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct ISoundSystem;
struct IGameFramework;
};

namespace f3goaty
{

class CGameApp final : public SbClientApp
{
public:
	CGameApp(sbe::IGameFramework *apGameFramework, sbe::ISoundSystem *apSoundSystem,
			 sbe::IRenderSystem *apRenderSystem, sbe::IInputSystem *apInputSystem,
			 sbe::ISystem *apSystem, int argc, char **argv);
	~CGameApp();
private:
	void RunFrame() override;
private:
	sbe::ISoundSystem *mpSoundSystem{nullptr};
	sbe::IGameFramework *mpFramework{nullptr};
};

}; // namespace f3goaty