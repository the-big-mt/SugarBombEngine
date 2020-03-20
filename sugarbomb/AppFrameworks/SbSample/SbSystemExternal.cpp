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

#include <stdexcept>

#include "SbSystemExternal.hpp"
#include "AppFrameworks/SbLibraryLoader/SbLibraryLoader.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"
#include "CoreLibs/SbSystem/IFileSystem.hpp"
#include "CoreLibs/SbSystem/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe
{

SbSystemExternal::SbSystemExternal()
{
	LoadModule();
};

SbSystemExternal::~SbSystemExternal()
{
	SbLibraryLoader::Unload(mnSystemLib);
};

void SbSystemExternal::LoadModule()
{
	mnSystemLib = SbLibraryLoader::Load("SbSystem");
	
	if(!mnSystemLib)
		throw std::runtime_error("Failed to load the system module!");
	
	GetSystemAPI_t pfnGetSystemAPI{SbLibraryLoader::GetSymbol<GetSystemAPI_t>(mnSystemLib, "GetSystemAPI")};
	
	if(!pfnGetSystemAPI)
		throw std::runtime_error("");
	
	sysImport_t ModuleImports{};
	ModuleImports.version = SYS_API_VERSION;
	auto ModuleExports{pfnGetSystemAPI(&ModuleImports)};
	
	if(!ModuleExports)
		throw std::runtime_error("");
	
	mpSystem = ModuleExports->sys;
	
	if(!mpSystem)
		throw std::runtime_error("");
	
	mpFileSystem = ModuleExports->fileSystem;
	
	if(!mpFileSystem)
		throw std::runtime_error("");
};

}; // namespace sbe