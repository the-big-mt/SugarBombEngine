/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Daniel Gibson (changes for POSIX)
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

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

#include "precompiled.h"
#include "SbUDP.hpp"

//namespace sbe
//{

/*
================================================================================================

	idUDP

================================================================================================
*/

/*
========================
idUDP::idUDP
========================
*/
idUDP::idUDP()
{
	netSocket = 0;
	memset( &bound_to, 0, sizeof( bound_to ) );
	silent = false;
	packetsRead = 0;
	bytesRead = 0;
	packetsWritten = 0;
	bytesWritten = 0;
}

/*
========================
idUDP::~idUDP
========================
*/
idUDP::~idUDP()
{
	Close();
}

/*
========================
idUDP::InitForPort
========================
*/
bool idUDP::InitForPort( int portNumber )
{
	// DG: don't specify an IP to bind for (and certainly not net_ip)
	// => it'll listen on all addresses (0.0.0.0 / INADDR_ANY)
	netSocket = NET_IPSocket( nullptr, portNumber, &bound_to );
	// DG end
	if( netSocket <= 0 )
	{
		netSocket = 0;
		memset( &bound_to, 0, sizeof( bound_to ) );
		return false;
	}
	
	return true;
}

/*
========================
idUDP::Close
========================
*/
void idUDP::Close()
{
	if( netSocket )
	{
		closesocket( netSocket );
		netSocket = 0;
		memset( &bound_to, 0, sizeof( bound_to ) );
	}
}

/*
========================
idUDP::GetPacket
========================
*/
bool idUDP::GetPacket( netadr_t& from, void* data, int& size, int maxSize )
{
	// DG: this fake while(1) loop pissed me off so I replaced it.. no functional change.
	if( ! Net_GetUDPPacket( netSocket, from, ( char* )data, size, maxSize ) )
	{
		return false;
	}
	
	packetsRead++;
	bytesRead += size;
	
	return true;
	// DG end
}

/*
========================
idUDP::GetPacketBlocking
========================
*/
bool idUDP::GetPacketBlocking( netadr_t& from, void* data, int& size, int maxSize, int timeout )
{

	if( !Net_WaitForData( netSocket, timeout ) )
	{
		return false;
	}
	
	if( GetPacket( from, data, size, maxSize ) )
	{
		return true;
	}
	
	return false;
}

/*
========================
idUDP::SendPacket
========================
*/
void idUDP::SendPacket( const netadr_t to, const void* data, int size )
{
	if( to.type == NA_BAD )
	{
		idLib::Warning( "idUDP::SendPacket: bad address type NA_BAD - ignored" );
		return;
	}
	
	packetsWritten++;
	bytesWritten += size;
	
	if( silent )
	{
		return;
	}
	
	Net_SendUDPPacket( netSocket, size, data, to );
}

//} // namespace sbe