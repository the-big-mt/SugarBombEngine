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

#include "SbNetClient.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

void SbNetClient::Connect(const char *asIP, uint16_t anPort)
{
	if(IsConnected())
		Disconnect();
	
	SendConnectionless(asIP, anPort, "connect");
};

void SbNetClient::Disconnect()
{
	if(!IsConnected())
		return;
	
	// TODO
	//SbNetMsg_Disconnect DisconnectMsg;
	//SendMsg(DisconnectMsg);
};

void SbNetClient::SendConnectionless(const char *asIP, uint16_t anPort, const char *asString)
{
	//netadr_t Adr{};
	//Adr.sIP;
	//Adr.nPort = 
	//mpPeer->SendDataTo();
};

bool SbNetClient::IsConnected() const
{
	// TODO
	return false;
};

}; // namespace sbe::SbNetwork