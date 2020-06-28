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

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbSystem
{

class SbSystemCommon : public ISystem
{
public:
	void Init() override;
	void Shutdown() override;
	
	intptr_t LoadLib(const char *asPath) override;
	void FreeLib(intptr_t anHandle) override;
	void *GetLibSymbol(intptr_t anHandle, const char *asSymbol) const override;
	
	void Printf(const char *asMsg, ...) override;
	
	void Warning(const char *asMsg, ...) override;
	
	void Error(const char *asMsg, ...) override;
	void FatalError(const char *asMsg, ...) override;
	
};

}; // sbe::SbSystem