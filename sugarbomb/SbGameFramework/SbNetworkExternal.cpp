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

#include <functional>

#include "SbNetworkExternal.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbNetwork/INetworkSystem.hpp"
#include "SbNetwork/SbModuleAPI.hpp"

//*****************************************************************************

namespace sbe::SbGameFramework
{

SbNetworkExternal::SbNetworkExternal(ISystem &aSystem) : mSystem(aSystem)
{
	LoadModule();
};

SbNetworkExternal::~SbNetworkExternal()
{
	UnloadModule();
};

void SbNetworkExternal::LoadModule()
{
	mnNetworkLib = mSystem.LoadLib("SbNetwork");
	
	if(!mnNetworkLib)
		return;
	
	GetNetworkAPI_t pfnGetNetworkAPI{mSystem.GetLibSymbol<GetNetworkAPI_t>(mnNetworkLib, "GetNetworkAPI")};
	
	if(!pfnGetNetworkAPI)
		return;
	
	netImport_t ModuleImports{};
	ModuleImports.version = NET_API_VERSION;
	ModuleImports.sys = std::addressof(mSystem);
	auto ModuleExports{pfnGetNetworkAPI(&ModuleImports)};
	
	if(!ModuleExports)
		return;
	
	mpNetworkSystem = ModuleExports->networkSystem;
	
	if(!mpNetworkSystem)
		return;
};

void SbNetworkExternal::UnloadModule()
{
	mSystem.FreeLib(mnNetworkLib);
};

}; // namespace sbe::SbGameFramework