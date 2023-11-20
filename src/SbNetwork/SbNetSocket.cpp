/*
*******************************************************************************

Copyright (C) 2020 SugarBombEngine Developers

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

#include "SbNetSocket.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

SbNetSocket::~SbNetSocket()
{
	Close();
};

void SbNetSocket::Bind(uint16_t anPort)
{
	sockaddr_in BindAddr{};
	
	BindAddr.sin_family = AF_INET;
	BindAddr.sin_port = htons(anPort);
	BindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(mnHandle, reinterpret_cast<const struct sockaddr*>(&BindAddr), sizeof(BindAddr));
};

}; // namespace sbe::SbNetwork