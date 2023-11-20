/*
*******************************************************************************

Copyright (C) 2020, 2023 SugarBombEngine Developers

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
/// @brief sound emitter interface

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct SbSoundEmitter
{
	///
	virtual void Free(bool abImmediate) = 0;
	
	///
	virtual void Update() = 0;
};

}; // namespace sbe