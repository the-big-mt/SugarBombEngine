/*
*******************************************************************************

Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2022 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @brief library wrapper class

//*****************************************************************************

#pragma once

#include <cstdint>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

//*****************************************************************************

namespace sbe
{

class SbLibrary
{
public:
	SbLibrary(const char *asPath);
	~SbLibrary();
	
	void *GetSymbol(const char *asSymbol) const;

	///
	template<typename T>
	T GetSymbol(const char *asSymbol) const {return reinterpret_cast<T>(GetSymbol(asSymbol));}
private:
	///
	void Load(const char *asPath);
	
	///
	void Free(); // TODO: Unload?
private:
	intptr_t mnHandle{nullptr};
};

#include "SbLibrary.inl"

}; // namespace sbe