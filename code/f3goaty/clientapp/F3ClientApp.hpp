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
/// @brief F3BFG client application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct IGameFramework;
};

struct idSoundSystem;

namespace f3bfg
{

class CClientApp final : public SbClientApp
{
public:
	CClientApp(int argc, const char * const *argv);
	~CClientApp();
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	void LogicUpdate() override;
	
	void InitSoundSystem();
	void ShutdownSoundSystem();
	
	void InitFrameworkModule();
	void ShutdownFrameworkModule();
	
	sbe::IGameFramework *mpFramework{nullptr};
	idSoundSystem *soundSystem{nullptr};
	
	int soundDLL{0};
	int frameworkDLL{0};
};

}; // namespace f3bfg