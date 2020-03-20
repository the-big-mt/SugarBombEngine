/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
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
/// @brief network address data

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct SbNetAdr
{
	enum class Type : int
	{
		NA_BAD, ///< an address lookup failed
		NA_LOOPBACK,
		NA_BROADCAST,
		NA_IP
	};

	Type type{Type::NA_BAD};
	byte ip[4]{};
	unsigned short port{0};
	
	const char *ToString() const;
	
	bool IsLocal() const;
	
	bool CompareBase(const SbNetAdr &other) const;
};

/*
========================
Sys_NetAdrToString
========================
*/
const char *SbNetAdr::ToString() const
{
	// DG: FIXME: those static buffers look fishy - I would feel better if they were
	//            at least thread-local - so /maybe/ use ID_TLS here?
	//            or maybe return an idStr and change calling code accordingly
	static int index{0};
	static char buf[ 4 ][ 64 ]{};	// flip/flop
	
	char *s = buf[index];
	
	index = ( index + 1 ) & 3;
	if( type == NA_IP || type == NA_LOOPBACK )
		; //idStr::snPrintf( s, 64, "%i.%i.%i.%i:%i", ip[0], ip[1], ip[2], ip[3], port ); // TODO
	else if( type == NA_BROADCAST )
		; //idStr::snPrintf( s, 64, "BROADCAST" );  // TODO
	else if( type == NA_BAD )
		; //idStr::snPrintf( s, 64, "BAD_IP" );  // TODO
	else
		; //idStr::snPrintf( s, 64, "WTF_UNKNOWN_IP_TYPE_%i", type ); // TODO

	return s;
};

/*
========================
Sys_IsLANAddress
========================
*/
bool SbNetAdr::IsLocal() const
{
	if( type == NA_LOOPBACK )
		return true;
	
	if( type != NA_IP )
		return false;
	
	// NOTE: this function won't work reliably for addresses on the local net
	// that are connected through a router (i.e. no IP from that net is on any interface)
	// However, I don't expect most people to have such setups at home and the code
	// would get a lot more complex and less portable.
	// Furthermore, this function isn't even used currently
	// TODO: should this even be here?
	/*
	if( num_interfaces )
	{
		int i;
		// DG: for 64bit compatibility, make these longs ints.
		unsigned int* p_ip;
		unsigned int ip;
		p_ip = ( unsigned int* )&ip[0];
		// DG end
		ip = ntohl( *p_ip );
		
		for( i = 0; i < num_interfaces; i++ )
			if( ( netint[i].ip & netint[i].mask ) == ( ip & netint[i].mask ) )
				return true;
	};
	*/
	
	return false;
};

/*
========================
Sys_CompareNetAdrBase

Compares without the port.
========================
*/
bool SbNetAdr::CompareBase( const SbNetAdr &b ) const
{
	if( type != b.type )
		return false;

	if( type == NA_LOOPBACK )
	{
		// DG: wtf is this comparison about, the comment above says "without the port"
		if( port == b.port )
			return true;
		
		return false;
	};
	
	if( type == NA_IP )
	{
		if( ip[0] == b.ip[0] && ip[1] == b.ip[1] && ip[2] == b.ip[2] && ip[3] == b.ip[3] )
			return true;

		return false;
	};
	
	//idLib::Printf( "Sys_CompareNetAdrBase: bad address type\n" ); // TODO
	return false;
};

}; // namespace sbe