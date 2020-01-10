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

#include <vector>

#include "SbNetwork/INetServer.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

using tNetPeerVec = std::vector<INetPeer*>;

class SbNetServer : public INetServer
{
public:
	SbNetServer(uint32_t anPort);
	
	INetPeer *GetClient(uint32_t anIndex) const override {return mvClients.at(anIndex);}
	
	uint32_t GetPort() const {return mnPort;}
private:
	tNetPeerVec mvClients;
	
	uint32_t mnPort{0};
};

}; // namespace sbe::SbNetwork