/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include "CoreLibs/SbSystem/IFileSystem.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbSystem
{

class SbFileSystem : public IFileSystem
{
public:
	SbFileSystem(ISystem &aSystem);
	
	void Init(const char *asBasePath, bool abFileLogging, bool abUseArchives) override;
	void Shutdown() override;
	
	IFile *OpenFile(const char *asPath) override;
	void CloseFile(IFile *apFile) override;
private:
	ISystem &mSystem;
};

};}; // namespace sbe::SbSystem