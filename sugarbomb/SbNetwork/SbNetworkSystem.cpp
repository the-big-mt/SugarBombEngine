/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include "SbNetworkSystem.hpp"
#include "SbNetworkImpl.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbMain/SbMath.hpp"
#include "CoreLibs/SbMain/SbString.hpp"

#include <cstring> // TODO: temp
#include <climits> // TODO: temp
#include <cstdlib> // TODO: temp

#ifdef _WIN32
#	include <Winsock2.h>
#endif

//*****************************************************************************

namespace sbe::SbNetwork
{

#ifdef _WIN32
using in_addr_t = ULONG;
#endif

SbNetworkSystem::SbNetworkSystem(SbSystem &aSystem, SbNetworkImpl &aImpl) : mSystem(aSystem), mImpl(aImpl){}

void SbNetworkSystem::Init()
{
	mSystem.Printf("%s\n", __PRETTY_FUNCTION__);
	
	mImpl.Init();
};

void SbNetworkSystem::Shutdown()
{
	mImpl.Shutdown();
};

/*
========================
Sys_StringToNetAdr
========================
*/
bool SbNetworkSystem::StringToNetAdr(const char *asNetAdr, SbNetAdr &aOutNetAdr, bool abDoDNSResolve)
{
	sockaddr_in sadr{};
	
	if(!Net_StringToSockaddr(asNetAdr, &sadr, abDoDNSResolve))
		return false;
	
	Net_SockadrToNetadr(&sadr, aOutNetAdr);
	return true;
};

/*
========================
Net_ExtractPort
========================
*/
static bool Net_ExtractPort( const char* src, char* buf, int bufsize, int* port )
{
	char* p;
	strncpy( buf, src, bufsize );
	p = buf;
	p += SbMath::Min( bufsize - 1, SbString::Length( src ) );
	*p = '\0';
	p = strchr( buf, ':' );
	if( !p )
	{
		return false;
	}
	*p = '\0';
	
	long lport = strtol( p + 1, nullptr, 10 );
	if( lport == 0 || lport == LONG_MIN || lport == LONG_MAX )
	{
		*port = 0;
		return false;
	}
	*port = lport;
	return true;
}

/*
========================
Net_StringToSockaddr
========================
*/
bool SbNetworkSystem::Net_StringToSockaddr( const char* s, sockaddr_in* sadr, bool doDNSResolve )
{
	/* NOTE: the doDNSResolve argument is ignored for two reasons:
	 * 1. domains can start with numbers nowadays so the old heuristic to find out if it's
	 *    an IP (check if the first char is a digit) isn't reliable
	 * 2. gethostbyname() works fine for IPs and doesn't do a lookup if the passed string
	 *    is an IP
	 */
	struct hostent*	h;
	char buf[256];
	int port;
	
	memset( sadr, 0, sizeof( *sadr ) );
	
	sadr->sin_family = AF_INET;
	sadr->sin_port = 0;
	
	// try to remove the port first, otherwise the DNS gets confused into multiple timeouts
	// failed or not failed, buf is expected to contain the appropriate host to resolve
	if( Net_ExtractPort( s, buf, sizeof( buf ), &port ) )
	{
		sadr->sin_port = htons( port );
	}
	// buf contains the host, even if Net_ExtractPort returned false
	h = gethostbyname( buf );
	if( h == nullptr )
	{
		return false;
	}
	sadr->sin_addr.s_addr = *( in_addr_t* ) h->h_addr_list[0];
	
	return true;
}

}; // namespace sbe::SbNetwork