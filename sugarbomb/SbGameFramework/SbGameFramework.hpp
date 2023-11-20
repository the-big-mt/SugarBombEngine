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

//*****************************************************************************

#pragma once

#include <functional>

#include "SbGameFramework/IGameFramework.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;
struct IGame;
struct INetworkSystem;
struct IRenderSystem;
struct ISoundSystem;

namespace SbGameFramework
{

class SbGameFramework : public IGameFramework
{
public:
	SbGameFramework(IRenderSystem *apRenderSystem, ISoundSystem *apSoundSystem, INetworkSystem &aNetworkSystem, IGame &aGame, ISystem &aSystem);
	
	void Init(const char *asTitle, const char *asShortTitle, bool abHeadless, int argc, char **argv) override;
	void Shutdown() override;
	
	void Frame(float afTimeStep) override;
	void Draw();
	
	bool IsInitialized() const {return mbInitialized;}
private:
	void CreateMainMenu();
	
	void StartMenu(bool abPlayIntro);
	void ExitMenu();
	
	void CleanupShell();
	
	void LoadDefaultConfig();
	
	bool LoadPrefsConfig();
private:
	std::reference_wrapper<ISystem> mSystem;
	
	IGame &mGame;
	INetworkSystem &mNetworkSystem;
	SbPhysicsSystem &mPhysicsSystem;
	
	IRenderSystem *mpRenderSystem{nullptr};
	ISoundSystem *mpSoundSystem{nullptr};
	
	bool mbInitialized{false};
};

};}; // namespace sbe::SbGameFramework