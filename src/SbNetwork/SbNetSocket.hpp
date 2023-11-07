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

#pragma once

#include <cstdint> // TODO: temp

#include "SbNetwork/SbNetAdr.hpp"

#ifdef _WIN32
#include <Winsock2.h>
#endif

//*****************************************************************************

namespace sbe::SbNetwork
{

class SbNetSocket
{
public:
	virtual ~SbNetSocket();
	
	///
	void Bind(uint16_t anPort);
	
	///
	ssize_t SendDataTo(const void *apDataBuffer, size_t anBufferLen, const SbNetAdr &aDestAdr /*, int anFlags*/);
	
	///
	ssize_t ReceiveDataFrom(void *apDataBuffer, size_t anBufferLen, SbNetAdr &aSrcAdr /*, int anFlags*/);
	
	///
	ssize_t SendData(const void *apDataBuffer, size_t anBufferLen /*, int anFlags*/);
	
	///
	ssize_t ReceiveData(void *apDataBuffer, size_t anBufferLen /*, int anFlags*/);
	
	///
	const SbNetAdr &GetAdr() const {return mAdr;}
protected:
	//using socket_t = decltype(socket(0, 0, 0));
	//socket_t mnHandle{};
	
#ifdef _WIN32
	SOCKET mnHandle{INVALID_SOCKET};
#else
	int mnHandle{-1};
#endif
private:
	void Close();
private:
	SbNetAdr mAdr{};
};

}; // namespace sbe::SbNetwork