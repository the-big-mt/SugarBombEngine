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

#include "SbNetPeer.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

SbNetPeer::~SbNetPeer()
{
	closesocket(mnSocket);
};

ssize_t SbNetPeer::SendDataTo(const void *apDataBuffer, size_t anBufferLen, const netadr_t &aDestAdr /*, int anFlags*/)
{
	return sendto(mnSocket, reinterpret_cast<const char*>(apDataBuffer), anBufferLen, 0);
};

ssize_t SbNetPeer::ReceiveDataFrom(void *apDataBuffer, size_t anBufferLen, netadr_t &aSrcAdr /*, int anFlags*/)
{
	return recvfrom(mnSocket, reinterpret_cast<char*>(apDataBuffer), anBufferLen, 0);
};

ssize_t SbNetPeer::SendData(const void *apDataBuffer, size_t anBufferLen /*, int anFlags*/)
{
	return send(mnSocket, reinterpret_cast<const char*>(apDataBuffer), anBufferLen, 0);
};

ssize_t SbNetPeer::ReceiveData(void *apDataBuffer, size_t anBufferLen /*, int anFlags*/)
{
	return recv(mnSocket, reinterpret_cast<char*>(apDataBuffer), anBufferLen, 0);
};

}; // namespace sbe::SbNetwork