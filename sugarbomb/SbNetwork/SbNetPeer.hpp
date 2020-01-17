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

#include "SbNetwork/INetPeer.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

class SbNetPeer : public INetPeer
{
public:
	size_t SendData(const void *apData) override;
	size_t ReceiveData(void *apBuffer, int anSize) override;
	
	bool IsLocal() const override;
};

}; // namespace sbe::SbNetwork