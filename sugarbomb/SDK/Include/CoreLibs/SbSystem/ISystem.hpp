/*
*******************************************************************************

Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019-2020, 2022 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @brief system module interface

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct SbSystemDesc; // TODO: SbSystemProps?

struct SbSystem
{
	///
	virtual void Init() = 0;

	///
	virtual void Shutdown() = 0;
	

	///
	virtual void Printf(const char *asMsg, ...) = 0;

	///
	virtual void Warning(const char *asMsg, ...) = 0;

	///
	virtual void Error(const char *asMsg, ...) = 0;
	
	///
	virtual void FatalError(const char *asMsg, ...) = 0;

	///
	virtual const SbSystemDesc &GetDesc() const = 0;
};

//#include "SbSystem.inl"

}; // namespace sbe