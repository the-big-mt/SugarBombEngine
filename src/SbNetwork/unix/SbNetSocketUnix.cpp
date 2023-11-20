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
/// @brief Berkeley sockets wrapper

//*****************************************************************************

#include "SbNetSocket.hpp"

#include <sys/socket.h>

//*****************************************************************************

namespace sbe::SbNetwork
{

ssize_t SbNetSocket::SendDataTo(const void *apDataBuffer, size_t anBufferLen, const SbNetAdr &aDestAdr /*, int anFlags*/)
{
	return sendto(mnHandle, apDataBuffer, anBufferLen, 0);
};

ssize_t SbNetSocket::ReceiveDataFrom(void *apDataBuffer, size_t anBufferLen, SbNetAdr &aSrcAdr /*, int anFlags*/)
{
	return recvfrom(mnHandle, apDataBuffer, anBufferLen, 0);
};

ssize_t SbNetSocket::SendData(const void *apDataBuffer, size_t anBufferLen /*, int anFlags*/)
{
	return send(mnHandle, apDataBuffer, anBufferLen, 0);
};

ssize_t SbNetSocket::ReceiveData(void *apDataBuffer, size_t anBufferLen /*, int anFlags*/)
{
	return recv(mnHandle, apDataBuffer, anBufferLen, 0);
};

void SbNetSocket::Close()
{
	close(mnHandle);
};

}; // namespace sbe::SbNetwork