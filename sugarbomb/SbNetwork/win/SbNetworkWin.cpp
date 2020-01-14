/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

//*****************************************************************************

#include "SbNetworkWin.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbNetwork
{

SbNetworkWin::SbNetworkWin(ISystem &aSystem) : mSystem(aSystem){}

void SbNetworkWin::Init()
{
	if( winsockInitialized )
		return;

	int r = WSAStartup( MAKEWORD( 1, 1 ), &winsockdata );
	if( r )
	{
		idLib::Printf( "WARNING: Winsock initialization failed, returned %d\n", r ); // TODO
		return;
	};
	
	winsockInitialized = true;
	idLib::Printf( "Winsock Initialized\n" ); // TODO
	
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = nullptr;
	DWORD dwRetVal = 0;
	PIP_ADDR_STRING pIPAddrString;
	ULONG ulOutBufLen;
	
	num_interfaces = 0;
	foundloopback = false;
	
	pAdapterInfo = ( IP_ADAPTER_INFO* )malloc( sizeof( IP_ADAPTER_INFO ) );
	if( !pAdapterInfo )
	{
		idLib::FatalError( "Sys_InitNetworking: Couldn't malloc( %d )", sizeof( IP_ADAPTER_INFO ) ); // TODO
	}
	ulOutBufLen = sizeof( IP_ADAPTER_INFO );
	
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW )
	{
		free( pAdapterInfo );
		pAdapterInfo = ( IP_ADAPTER_INFO* )malloc( ulOutBufLen );
		if( !pAdapterInfo )
		{
			idLib::FatalError( "Sys_InitNetworking: Couldn't malloc( %ld )", ulOutBufLen ); // TODO
		}
	}
	
	if( ( dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) ) != NO_ERROR )
	{
		// happens if you have no network connection
		idLib::Printf( "Sys_InitNetworking: GetAdaptersInfo failed (%ld).\n", dwRetVal ); // TODO
	}
	else
	{
		pAdapter = pAdapterInfo;
		while( pAdapter )
		{
			idLib::Printf( "Found interface: %s %s - ", pAdapter->AdapterName, pAdapter->Description ); // TODO
			pIPAddrString = &pAdapter->IpAddressList;
			while( pIPAddrString )
			{
				unsigned long ip_a, ip_m;
				if( !idStr::Icmp( "127.0.0.1", pIPAddrString->IpAddress.String ) )
				{
					foundloopback = true;
				}
				ip_a = ntohl( inet_addr( pIPAddrString->IpAddress.String ) );
				ip_m = ntohl( inet_addr( pIPAddrString->IpMask.String ) );
				//skip null netmasks
				if( !ip_m )
				{
					idLib::Printf( "%s nullptr netmask - skipped\n", pIPAddrString->IpAddress.String ); // TODO
					pIPAddrString = pIPAddrString->Next;
					continue;
				}
				idLib::Printf( "%s/%s\n", pIPAddrString->IpAddress.String, pIPAddrString->IpMask.String ); // TODO
				netint[num_interfaces].ip = ip_a;
				netint[num_interfaces].mask = ip_m;
				idStr::Copynz( netint[num_interfaces].addr, pIPAddrString->IpAddress.String, sizeof( netint[num_interfaces].addr ) );
				num_interfaces++;
				if( num_interfaces >= MAX_INTERFACES )
				{
					idLib::Printf( "Sys_InitNetworking: MAX_INTERFACES(%d) hit.\n", MAX_INTERFACES ); // TODO
					free( pAdapterInfo );
					return;
				}
				pIPAddrString = pIPAddrString->Next;
			}
			pAdapter = pAdapter->Next;
		};
	};
	
	free( pAdapterInfo );
};

void SbNetworkWin::Shutdown()
{
	if(!winsockInitialized)
		return;

	WSACleanup();
	winsockInitialized = false;
};

const char *SbNetworkWin::GetErrorString() const
{
	int nCode{WSAGetLastError()};
	switch(nCode)
	{
		case WSAEINTR:
			return "WSAEINTR";
		case WSAEBADF:
			return "WSAEBADF";
		case WSAEACCES:
			return "WSAEACCES";
		case WSAEDISCON:
			return "WSAEDISCON";
		case WSAEFAULT:
			return "WSAEFAULT";
		case WSAEINVAL:
			return "WSAEINVAL";
		case WSAEMFILE:
			return "WSAEMFILE";
		case WSAEWOULDBLOCK:
			return "WSAEWOULDBLOCK";
		case WSAEINPROGRESS:
			return "WSAEINPROGRESS";
		case WSAEALREADY:
			return "WSAEALREADY";
		case WSAENOTSOCK:
			return "WSAENOTSOCK";
		case WSAEDESTADDRREQ:
			return "WSAEDESTADDRREQ";
		case WSAEMSGSIZE:
			return "WSAEMSGSIZE";
		case WSAEPROTOTYPE:
			return "WSAEPROTOTYPE";
		case WSAENOPROTOOPT:
			return "WSAENOPROTOOPT";
		case WSAEPROTONOSUPPORT:
			return "WSAEPROTONOSUPPORT";
		case WSAESOCKTNOSUPPORT:
			return "WSAESOCKTNOSUPPORT";
		case WSAEOPNOTSUPP:
			return "WSAEOPNOTSUPP";
		case WSAEPFNOSUPPORT:
			return "WSAEPFNOSUPPORT";
		case WSAEAFNOSUPPORT:
			return "WSAEAFNOSUPPORT";
		case WSAEADDRINUSE:
			return "WSAEADDRINUSE";
		case WSAEADDRNOTAVAIL:
			return "WSAEADDRNOTAVAIL";
		case WSAENETDOWN:
			return "WSAENETDOWN";
		case WSAENETUNREACH:
			return "WSAENETUNREACH";
		case WSAENETRESET:
			return "WSAENETRESET";
		case WSAECONNABORTED:
			return "WSAECONNABORTED";
		case WSAECONNRESET:
			return "WSAECONNRESET";
		case WSAENOBUFS:
			return "WSAENOBUFS";
		case WSAEISCONN:
			return "WSAEISCONN";
		case WSAENOTCONN:
			return "WSAENOTCONN";
		case WSAESHUTDOWN:
			return "WSAESHUTDOWN";
		case WSAETOOMANYREFS:
			return "WSAETOOMANYREFS";
		case WSAETIMEDOUT:
			return "WSAETIMEDOUT";
		case WSAECONNREFUSED:
			return "WSAECONNREFUSED";
		case WSAELOOP:
			return "WSAELOOP";
		case WSAENAMETOOLONG:
			return "WSAENAMETOOLONG";
		case WSAEHOSTDOWN:
			return "WSAEHOSTDOWN";
		case WSASYSNOTREADY:
			return "WSASYSNOTREADY";
		case WSAVERNOTSUPPORTED:
			return "WSAVERNOTSUPPORTED";
		case WSANOTINITIALISED:
			return "WSANOTINITIALISED";
		case WSAHOST_NOT_FOUND:
			return "WSAHOST_NOT_FOUND";
		case WSATRY_AGAIN:
			return "WSATRY_AGAIN";
		case WSANO_RECOVERY:
			return "WSANO_RECOVERY";
		case WSANO_DATA:
			return "WSANO_DATA";
		default:
			return "NO ERROR";
	};
};

}; // sbe::SbNetwork