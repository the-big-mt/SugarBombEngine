/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Daniel Gibson (changes for POSIX)
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "SbNetworkSystem.hpp"

static netExport_t gNetExport;

SbNetworkSystem gNetworkSystem;
SbNetworkSystem *gpNetworkSystem{&gNetworkSystem};

#ifndef SBE_SINGLE_BINARY
idSys *gpSys{nullptr};
idCommon *gpCommon{nullptr};
idCmdSystem *gpCmdSystem{nullptr};
idCVarSystem *gpCvarSystem{nullptr};
idFileSystem *gpFileSystem{nullptr};

idCVar *idCVar::staticVars{nullptr};
#endif

/*
===========
GetNetAPI
============
*/
#ifndef SBE_SINGLE_BINARY
C_EXPORT
#endif
netExport_t *GetNetAPI(inputImport_t *apImport)
{
	if(apImport->version == NET_API_VERSION)
	{
		// set interface pointers used by the module
		gpSys = apImport->sys;
		gpCommon = apImport->common;
		gpCmdSystem = apImport->cmdSystem;
		gpCvarSystem = apImport->cvarSystem;
		gpFileSystem = apImport->fileSystem;
	};
	
	// set interface pointers used by idLib
	idLib::sys = gpSys;
	idLib::common = gpCommon;
	idLib::cvarSystem = gpCvarSystem;
	idLib::fileSystem = gpFileSystem;
	
	// setup export interface
	gNetExport.version = NET_API_VERSION;
	gNetExport.networkSystem = gpNetworkSystem;
	
	return &gNetExport;
};

/*
================================================================================================

SbNetworkSystem

================================================================================================
*/

/*
========================
Sys_InitNetworking
========================
*/
void SbNetworkSystem::Init()
{

	bool foundloopback = false;
	
#if defined(_WIN32) // DG: add win32 stuff here for socket networking code unification..
	if( winsockInitialized )
	{
		return;
	}
	int r = WSAStartup( MAKEWORD( 1, 1 ), &winsockdata );
	if( r )
	{
		idLib::Printf( "WARNING: Winsock initialization failed, returned %d\n", r );
		return;
	}
	
	winsockInitialized = true;
	idLib::Printf( "Winsock Initialized\n" );
	
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
		idLib::FatalError( "Sys_InitNetworking: Couldn't malloc( %d )", sizeof( IP_ADAPTER_INFO ) );
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
			idLib::FatalError( "Sys_InitNetworking: Couldn't malloc( %ld )", ulOutBufLen );
		}
	}
	
	if( ( dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) ) != NO_ERROR )
	{
		// happens if you have no network connection
		idLib::Printf( "Sys_InitNetworking: GetAdaptersInfo failed (%ld).\n", dwRetVal );
	}
	else
	{
		pAdapter = pAdapterInfo;
		while( pAdapter )
		{
			idLib::Printf( "Found interface: %s %s - ", pAdapter->AdapterName, pAdapter->Description );
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
					idLib::Printf( "%s nullptr netmask - skipped\n", pIPAddrString->IpAddress.String );
					pIPAddrString = pIPAddrString->Next;
					continue;
				}
				idLib::Printf( "%s/%s\n", pIPAddrString->IpAddress.String, pIPAddrString->IpMask.String );
				netint[num_interfaces].ip = ip_a;
				netint[num_interfaces].mask = ip_m;
				idStr::Copynz( netint[num_interfaces].addr, pIPAddrString->IpAddress.String, sizeof( netint[num_interfaces].addr ) );
				num_interfaces++;
				if( num_interfaces >= MAX_INTERFACES )
				{
					idLib::Printf( "Sys_InitNetworking: MAX_INTERFACES(%d) hit.\n", MAX_INTERFACES );
					free( pAdapterInfo );
					return;
				}
				pIPAddrString = pIPAddrString->Next;
			}
			pAdapter = pAdapter->Next;
		}
	}
	free( pAdapterInfo );
	
#elif defined(__APPLE__) || defined(__FreeBSD__)
	// haven't been able to clearly pinpoint which standards or RFCs define SIOCGIFCONF, SIOCGIFADDR, SIOCGIFNETMASK ioctls
	// it seems fairly widespread, in Linux kernel ioctl, and in BSD .. so let's assume it's always available on our targets
	
	unsigned int ip, mask;
	struct ifaddrs* ifap, *ifp;
	
	num_interfaces = 0;
	
	if( getifaddrs( &ifap ) < 0 )
	{
		common->FatalError( "InitNetworking: SIOCGIFCONF error - %s\n", strerror( errno ) );
		return;
	}
	
	for( ifp = ifap; ifp; ifp = ifp->ifa_next )
	{
		if( ifp->ifa_addr->sa_family != AF_INET )
			continue;
	
		if( !( ifp->ifa_flags & IFF_UP ) )
			continue;
	
		if( !ifp->ifa_addr )
			continue;
	
		if( !ifp->ifa_netmask )
			continue;
	
		// RB: 64 bit fixes, changed long to int
		ip = ntohl( *( unsigned int* )&ifp->ifa_addr->sa_data[2] );
		mask = ntohl( *( unsigned int* )&ifp->ifa_netmask->sa_data[2] );
		// RB end
	
		if( ip == INADDR_LOOPBACK )
		{
			foundloopback = true;
			common->Printf( "loopback\n" );
		}
		else
		{
			common->Printf( "%d.%d.%d.%d",
							( unsigned char )ifp->ifa_addr->sa_data[2],
							( unsigned char )ifp->ifa_addr->sa_data[3],
							( unsigned char )ifp->ifa_addr->sa_data[4],
							( unsigned char )ifp->ifa_addr->sa_data[5] );
			common->Printf( "/%d.%d.%d.%d\n",
							( unsigned char )ifp->ifa_netmask->sa_data[2],
							( unsigned char )ifp->ifa_netmask->sa_data[3],
							( unsigned char )ifp->ifa_netmask->sa_data[4],
							( unsigned char )ifp->ifa_netmask->sa_data[5] );
		}
		netint[ num_interfaces ].ip = ip;
		netint[ num_interfaces ].mask = mask;
		// DG: set netint addr
		ip_to_addr( &ifp->ifa_addr->sa_data[2], netint[ num_interfaces ].addr );
		// DG end
		num_interfaces++;
	}
#else // not _WIN32, OSX or FreeBSD
	int		s;
	char	buf[ MAX_INTERFACES * sizeof( ifreq ) ];
	ifconf	ifc;
	ifreq*	ifr;
	int		ifindex;
	unsigned int ip, mask;
	
	num_interfaces = 0;
	
	s = socket( AF_INET, SOCK_DGRAM, 0 );
	ifc.ifc_len = MAX_INTERFACES * sizeof( ifreq );
	ifc.ifc_buf = buf;
	if( ioctl( s, SIOCGIFCONF, &ifc ) < 0 )
	{
		common->FatalError( "InitNetworking: SIOCGIFCONF error - %s\n", strerror( errno ) );
		return;
	}
	ifindex = 0;
	while( ifindex < ifc.ifc_len )
	{
		common->Printf( "found interface %s - ", ifc.ifc_buf + ifindex );
		// find the type - ignore interfaces for which we can find we can't get IP and mask ( not configured )
		ifr = ( ifreq* )( ifc.ifc_buf + ifindex );
		if( ioctl( s, SIOCGIFADDR, ifr ) < 0 )
		{
			common->Printf( "SIOCGIFADDR failed: %s\n", strerror( errno ) );
		}
		else
		{
			if( ifr->ifr_addr.sa_family != AF_INET )
			{
				common->Printf( "not AF_INET\n" );
			}
			else
			{
				// RB: 64 bit fixes, changed long to int
				ip = ntohl( *( unsigned int* )&ifr->ifr_addr.sa_data[2] );
				// RB end
				if( ip == INADDR_LOOPBACK )
				{
					foundloopback = true;
					common->Printf( "loopback\n" );
				}
				else
				{
					common->Printf( "%d.%d.%d.%d",
									( unsigned char )ifr->ifr_addr.sa_data[2],
									( unsigned char )ifr->ifr_addr.sa_data[3],
									( unsigned char )ifr->ifr_addr.sa_data[4],
									( unsigned char )ifr->ifr_addr.sa_data[5] );
				}
	
				// DG: set netint address before getting the mask
				ip_to_addr( &ifr->ifr_addr.sa_data[2], netint[ num_interfaces ].addr );
				// DG end
	
				if( ioctl( s, SIOCGIFNETMASK, ifr ) < 0 )
				{
					common->Printf( " SIOCGIFNETMASK failed: %s\n", strerror( errno ) );
				}
				else
				{
					// RB: 64 bit fixes, changed long to int
					mask = ntohl( *( unsigned int* )&ifr->ifr_addr.sa_data[2] );
					// RB end
					if( ip != INADDR_LOOPBACK )
					{
						common->Printf( "/%d.%d.%d.%d\n",
										( unsigned char )ifr->ifr_addr.sa_data[2],
										( unsigned char )ifr->ifr_addr.sa_data[3],
										( unsigned char )ifr->ifr_addr.sa_data[4],
										( unsigned char )ifr->ifr_addr.sa_data[5] );
					}
					netint[ num_interfaces ].ip = ip;
					netint[ num_interfaces ].mask = mask;
					num_interfaces++;
				}
			}
		}
		ifindex += sizeof( ifreq );
	}
#endif // all those operating systems..
	
	// for some retarded reason, win32 doesn't count loopback as an adapter...
	// and because I'm extra-cautious I add this check on real operating systems as well :)
	if( !foundloopback && num_interfaces < MAX_INTERFACES )
	{
		idLib::Printf( "Sys_InitNetworking: adding loopback interface\n" );
		netint[num_interfaces].ip = ntohl( inet_addr( "127.0.0.1" ) );
		netint[num_interfaces].mask = ntohl( inet_addr( "255.0.0.0" ) );
		num_interfaces++;
	}
};

/*
========================
Sys_ShutdownNetworking
========================
*/
void SbNetworkSystem::Shutdown()
{
#ifdef _WIN32
	if( !winsockInitialized )
	{
		return;
	}
	WSACleanup();
	winsockInitialized = false;
#endif
	if( usingSocks )
		closesocket( socks_socket );
};

/*
========================
Sys_GetLocalIPCount
========================
*/
int SbNetworkSystem::GetLocalIPCount() const
{
	return num_interfaces;
};

/*
========================
Sys_GetLocalIP
========================
*/
const char *SbNetworkSystem::GetLocalIP(int anIndex) const
{
	if( ( anIndex < 0 ) || ( anIndex >= num_interfaces ) )
		return nullptr;

	return netint[anIndex].addr;
};