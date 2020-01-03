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

//*****************************************************************************

namespace sbe
{

struct ISystem
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual int LoadLib(const char *asPath) = 0;
	
	///
	virtual void FreeLib(int anHandle) = 0;
	
	///
	virtual void *GetLibSymbol(int anHandle, const char *asSymbol) const = 0;
	
	///
	template<typename T>
	T GetLibSymbol(int anHandle, const char *asSymbol) const {return reinterpret_cast<T>(GetLibSymbol(anHandle, asSymbol));}
	
	///
	virtual void Printf(const char *asMsg, ...) = 0;
	
	///
	virtual void Warning(const char *asMsg, ...) = 0;
	
	///
	virtual void FatalError(const char *asMsg, ...) = 0;
};

}; // namespace sbe