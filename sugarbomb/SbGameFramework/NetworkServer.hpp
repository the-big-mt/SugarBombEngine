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

This file incorporates work originally covered by the MIT License. See
LICENSE-vaultmp file for details.

*******************************************************************************
*/
#pragma once

#include "vaultserver.hpp"
#include "Network.hpp"

/**
 * \brief Server network interface
 */

class NetworkServer : public Network
{
		friend class Dedicated;

	private:
		NetworkServer() = delete;

#ifdef VAULTMP_DEBUG
		static DebugInput<NetworkServer> debug;
#endif

	public:
		/**
		 * \brief Processes an event of a given type
		 *
		 * Returns a NetworkResponse to send to the client(s)
		 */
		static NetworkResponse ProcessEvent(unsigned char id);
		/**
		 * \brief Processes a packet from a client
		 *
		 * Returns a NetworkResponse to send to the client(s)
		 */
		static NetworkResponse ProcessPacket(RakNet::Packet* data);

};