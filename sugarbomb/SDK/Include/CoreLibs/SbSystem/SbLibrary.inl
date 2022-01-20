/*
*******************************************************************************

Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2020, 2022 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

SbLibrary::SbLibrary(const char *asName)
{
	Load(asName);
};

SbLibrary::~SbLibrary()
{
	Free();
};

// RB: 64 bit fixes, changed int to intptr_t

#ifdef _WIN32
void SbLibrary::Load(const char *asName)
{
	if(mnHandle)
		Free();

	mnHandle = reinterpret_cast<intptr_t>(LoadLibrary(asName));
};

void SbLibrary::Free()
{
	if(!mnHandle)
		return;

	FreeLibrary(reinterpret_cast<HMODULE>(mnHandle));
};

void *SbLibrary::GetSymbol(const char *asSymbol)
{
	return reinterpret_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(mnHandle), asSymbol));
};
#else // if not _WIN32
void SbLibrary::Load(const char *asName)
{
	if(mnHandle)
		Free();

	mnHandle = reinterpret_cast<intptr_t>(dlopen(asName, RTLD_NOW));
};

void SbLibrary::Free()
{
	dlclose(reinterpret_cast<void*>(mnHandle));
};

void *SbLibrary::GetSymbol(const char *asSymbol)
{
	return dlsym(reinterpret_cast<void *>(mnHandle), asSymbol);
};
#endif // _WIN32