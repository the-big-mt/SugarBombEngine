/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 SugarBombEngine Developers

This file is part of SugarBombEngine.

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

#include "framework/IGameFramework.hpp"

namespace sbe
{
struct ISys;
struct IFileSystem;
struct idCmdSystem;
struct idCVarSystem;
struct ISoundSystem;
};

class SbGameFramework : public sbe::IGameFramework
{
public:
	//SbGameFramework(sbe::ISys *apSys, sbe::IFileSystem *apFileSystem);
	
	void Init() override;
	void Shutdown() override;
	
	void Frame() override;
	
	void Quit() override;
	
	bool IsInitialized() const override;
	bool IsShuttingDown() const override;
private:
	void CreateMainMenu();
	
	void InitGameModule();
	void ShutdownGameModule();
	
	void BusyWait();
	
	void WriteConfigToFile(const char *asFileName);
	void WriteConfiguration();
	
	sbe::ISys *mpSys{nullptr};
	sbe::IFileSystem *mpFileSystem{nullptr};
	
	sbe::idCmdSystem *mpCmdSystem{nullptr};
	sbe::idCVarSystem *mpCvarSystem{nullptr};
	
	sbe::ISoundSystem *mpSoundSystem{nullptr};
};