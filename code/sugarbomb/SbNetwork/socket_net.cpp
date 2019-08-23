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


/*
================================================================================================
Contains the NetworkSystem implementation for SOCKET APIs, currently POSIX Systems (Linux, FreeBSD,
OSX, ...) and Winsock (Windows...) are supported.
Note that other POSIX systems may need some small changes, e.g. in Sys_InitNetworking()
================================================================================================
*/

#pragma hdrstop
#include "precompiled.h"

#ifdef _WIN32

#include <iptypes.h>
#include <iphlpapi.h>
// force these libs to be included, so users of idLib don't need to add them to every project
#pragma comment(lib, "iphlpapi.lib" )
#pragma comment(lib, "wsock32.lib" )

#else // ! _WIN32

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/select.h>
#include <net/if.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include <ifaddrs.h>
#endif

#endif // _WIN32

//namespace BFG
//{

/*
================================================================================================

Some #defines and platform specific things etc to abstract differences between Winsocks and
real POSIX sockets away..

================================================================================================
*/

#ifdef _WIN32

#define Net_GetLastError WSAGetLastError

#define D3_NET_EWOULDBLOCK   WSAEWOULDBLOCK
#define D3_NET_ECONNRESET    WSAECONNRESET
#define D3_NET_EADDRNOTAVAIL WSAEADDRNOTAVAIL

typedef ULONG in_addr_t;
typedef int socklen_t;

static WSADATA	winsockdata;
static bool	winsockInitialized = false;

#else // ! _WIN32

static int Net_GetLastError()
{
	return errno;
}

#define D3_NET_EWOULDBLOCK   EWOULDBLOCK
#define D3_NET_ECONNRESET    ECONNRESET
#define D3_NET_EADDRNOTAVAIL EADDRNOTAVAIL

typedef int SOCKET;
#define closesocket(x) close(x)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#endif // _WIN32

/*
================================================================================================

	Network CVars

================================================================================================
*/

idCVar net_socksServer( "net_socksServer", "", CVAR_ARCHIVE, "" );
idCVar net_socksPort( "net_socksPort", "1080", CVAR_ARCHIVE | CVAR_INTEGER, "" );
idCVar net_socksUsername( "net_socksUsername", "", CVAR_ARCHIVE, "" );
idCVar net_socksPassword( "net_socksPassword", "", CVAR_ARCHIVE, "" );

idCVar net_ip( "net_ip", "localhost", CVAR_NOCHEAT, "local IP address" );

static struct sockaddr_in	socksRelayAddr;

// static SOCKET	ip_socket; FIXME: what was this about?

static bool usingSocks = false;
static SOCKET	socks_socket = 0;
static char		socksBuf[4096];

typedef struct
{
	// RB: 64 bit fixes, changed long to int
	// FIXME: IPv6?
	unsigned int ip;
	unsigned int mask;
	// RB end
	char addr[16];
} net_interface;

#define 		MAX_INTERFACES	32
int				num_interfaces = 0;
net_interface	netint[MAX_INTERFACES];

/*
================================================================================================

	Free Functions

================================================================================================
*/

/*
========================
NET_ErrorString
========================
*/
const char* NET_ErrorString()
{
#ifndef _WIN32
	return strerror( errno );
#else // _WIN32
	int code = WSAGetLastError();
	switch( code )
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
	}
#endif // _WIN32
}

/*
========================
Net_NetadrToSockadr
========================
*/
void Net_NetadrToSockadr( const netadr_t* a, sockaddr_in* s )
{
	memset( s, 0, sizeof( *s ) );
	
	if( a->type == NA_BROADCAST )
	{
		s->sin_family = AF_INET;
		s->sin_addr.s_addr = INADDR_BROADCAST;
	}
	else if( a->type == NA_IP || a->type == NA_LOOPBACK )
	{
		s->sin_family = AF_INET;
		s->sin_addr.s_addr = *( ( in_addr_t* ) &a->ip );
	}
	
	s->sin_port = htons( ( short )a->port );
}

/*
========================
Net_SockadrToNetadr
========================
*/
#define LOOPBACK_NET    0x7F000000 // 127.0.0.0
#define LOOPBACK_PREFIX 0xFF000000 // /8 or 255.0.0.0
void Net_SockadrToNetadr( sockaddr_in* s, netadr_t* a )
{
	in_addr_t ip;
	if( s->sin_family == AF_INET )
	{
		ip = s->sin_addr.s_addr;
		*( in_addr_t* )&a->ip = ip;
		a->port = ntohs( s->sin_port );
		// we store in network order, that loopback test is host order..
		ip = ntohl( ip );
		// DG: just comparing ip with INADDR_LOOPBACK is lame,
		//     because all of 127.0.0.0/8 is loopback.
		// if( ( ip & LOOPBACK_PREFIX ) == LOOPBACK_NET )
		if( ip == INADDR_LOOPBACK )
		{
			a->type = NA_LOOPBACK;
		}
		else
		{
			a->type = NA_IP;
		}
	}
}

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
	p += Min( bufsize - 1, idStr::Length( src ) );
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
static bool Net_StringToSockaddr( const char* s, sockaddr_in* sadr, bool doDNSResolve )
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

/*
========================
NET_IPSocket
========================
*/
int NET_IPSocket( const char* bind_ip, int port, netadr_t* bound_to )
{
	SOCKET				newsocket;
	sockaddr_in			address;
	
	if( port != PORT_ANY )
	{
		if( bind_ip )
		{
			idLib::Printf( "Opening IP socket: %s:%i\n", bind_ip, port );
		}
		else
		{
			idLib::Printf( "Opening IP socket: localhost:%i\n", port );
		}
	}
	
	if( ( newsocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == INVALID_SOCKET )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: socket: %s\n", NET_ErrorString() );
		return 0;
	}
	
	// make it non-blocking
#ifdef _WIN32 // which has no fcntl()
	unsigned long	_true = 1;
	if( ioctlsocket( newsocket, FIONBIO, &_true ) == SOCKET_ERROR )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: ioctl FIONBIO: %s\n", NET_ErrorString() );
		closesocket( newsocket );
		return 0;
	}
#else
	int flags = fcntl( newsocket, F_GETFL, 0 );
	if( flags < 0 )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: fcntl F_GETFL: %s\n", NET_ErrorString() );
		closesocket( newsocket );
		return 0;
	}
	flags |= O_NONBLOCK;
	if( fcntl( newsocket, F_SETFL, flags ) < 0 )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: fcntl F_SETFL with O_NONBLOCK: %s\n", NET_ErrorString() );
		closesocket( newsocket );
		return 0;
	}
#endif
	
	// make it broadcast capable
	int i = 1;
	if( setsockopt( newsocket, SOL_SOCKET, SO_BROADCAST, ( char* )&i, sizeof( i ) ) == SOCKET_ERROR )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString() );
		closesocket( newsocket );
		return 0;
	}
	
	if( !bind_ip || !bind_ip[0] || !idStr::Icmp( bind_ip, "localhost" ) )
	{
		address.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		Net_StringToSockaddr( bind_ip, &address, true );
	}
	
	if( port == PORT_ANY )
	{
		address.sin_port = 0;
	}
	else
	{
		address.sin_port = htons( ( short )port );
	}
	
	address.sin_family = AF_INET;
	
	if( bind( newsocket, ( const sockaddr* )&address, sizeof( address ) ) == SOCKET_ERROR )
	{
		idLib::Printf( "WARNING: UDP_OpenSocket: bind: %s\n", NET_ErrorString() );
		closesocket( newsocket );
		return 0;
	}
	
	// if the port was PORT_ANY, we need to query again to know the real port we got bound to
	// ( this used to be in idUDP::InitForPort )
	if( bound_to )
	{
		socklen_t len = sizeof( address );
		if( getsockname( newsocket, ( struct sockaddr* )&address, &len ) == SOCKET_ERROR )
		{
			common->Printf( "ERROR: IPSocket: getsockname: %s\n", NET_ErrorString() );
			closesocket( newsocket );
			return 0;
		}
		Net_SockadrToNetadr( &address, bound_to );
	}
	
	return newsocket;
}

/*
========================
NET_OpenSocks
========================
*/
void NET_OpenSocks( int port )
{
	sockaddr_in			address;
	struct hostent*		h;
	int					len;
	bool				rfc1929;
	unsigned char		buf[64];
	
	usingSocks = false;
	
	idLib::Printf( "Opening connection to SOCKS server.\n" );
	
	if( ( socks_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET )
	{
		idLib::Printf( "WARNING: NET_OpenSocks: socket: %s\n", NET_ErrorString() );
		return;
	}
	
	h = gethostbyname( net_socksServer.GetString() );
	if( h == nullptr )
	{
		idLib::Printf( "WARNING: NET_OpenSocks: gethostbyname: %s\n", NET_ErrorString() );
		return;
	}
	if( h->h_addrtype != AF_INET )
	{
		idLib::Printf( "WARNING: NET_OpenSocks: gethostbyname: address type was not AF_INET\n" );
		return;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = *( in_addr_t* )h->h_addr_list[0];
	address.sin_port = htons( ( short )net_socksPort.GetInteger() );
	
	if( connect( socks_socket, ( sockaddr* )&address, sizeof( address ) ) == SOCKET_ERROR )
	{
		idLib::Printf( "NET_OpenSocks: connect: %s\n", NET_ErrorString() );
		return;
	}
	
	// send socks authentication handshake
	if( *net_socksUsername.GetString() || *net_socksPassword.GetString() )
	{
		rfc1929 = true;
	}
	else
	{
		rfc1929 = false;
	}
	
	buf[0] = 5;		// SOCKS version
	// method count
	if( rfc1929 )
	{
		buf[1] = 2;
		len = 4;
	}
	else
	{
		buf[1] = 1;
		len = 3;
	}
	buf[2] = 0;		// method #1 - method id #00: no authentication
	if( rfc1929 )
	{
		buf[2] = 2;		// method #2 - method id #02: username/password
	}
	if( send( socks_socket, ( const char* )buf, len, 0 ) == SOCKET_ERROR )
	{
		idLib::Printf( "NET_OpenSocks: send: %s\n", NET_ErrorString() );
		return;
	}
	
	// get the response
	len = recv( socks_socket, ( char* )buf, 64, 0 );
	if( len == SOCKET_ERROR )
	{
		idLib::Printf( "NET_OpenSocks: recv: %s\n", NET_ErrorString() );
		return;
	}
	if( len != 2 || buf[0] != 5 )
	{
		idLib::Printf( "NET_OpenSocks: bad response\n" );
		return;
	}
	switch( buf[1] )
	{
		case 0:	// no authentication
			break;
		case 2: // username/password authentication
			break;
		default:
			idLib::Printf( "NET_OpenSocks: request denied\n" );
			return;
	}
	
	// do username/password authentication if needed
	if( buf[1] == 2 )
	{
		int		ulen;
		int		plen;
		
		// build the request
		ulen = idStr::Length( net_socksUsername.GetString() );
		plen = idStr::Length( net_socksPassword.GetString() );
		
		buf[0] = 1;		// username/password authentication version
		buf[1] = ulen;
		if( ulen )
		{
			memcpy( &buf[2], net_socksUsername.GetString(), ulen );
		}
		buf[2 + ulen] = plen;
		if( plen )
		{
			memcpy( &buf[3 + ulen], net_socksPassword.GetString(), plen );
		}
		
		// send it
		if( send( socks_socket, ( const char* )buf, 3 + ulen + plen, 0 ) == SOCKET_ERROR )
		{
			idLib::Printf( "NET_OpenSocks: send: %s\n", NET_ErrorString() );
			return;
		}
		
		// get the response
		len = recv( socks_socket, ( char* )buf, 64, 0 );
		if( len == SOCKET_ERROR )
		{
			idLib::Printf( "NET_OpenSocks: recv: %s\n", NET_ErrorString() );
			return;
		}
		if( len != 2 || buf[0] != 1 )
		{
			idLib::Printf( "NET_OpenSocks: bad response\n" );
			return;
		}
		if( buf[1] != 0 )
		{
			idLib::Printf( "NET_OpenSocks: authentication failed\n" );
			return;
		}
	}
	
	// send the UDP associate request
	buf[0] = 5;		// SOCKS version
	buf[1] = 3;		// command: UDP associate
	buf[2] = 0;		// reserved
	buf[3] = 1;		// address type: IPV4
	*( int* )&buf[4] = INADDR_ANY;
	*( short* )&buf[8] = htons( ( short )port );		// port
	if( send( socks_socket, ( const char* )buf, 10, 0 ) == SOCKET_ERROR )
	{
		idLib::Printf( "NET_OpenSocks: send: %s\n", NET_ErrorString() );
		return;
	}
	
	// get the response
	len = recv( socks_socket, ( char* )buf, 64, 0 );
	if( len == SOCKET_ERROR )
	{
		idLib::Printf( "NET_OpenSocks: recv: %s\n", NET_ErrorString() );
		return;
	}
	if( len < 2 || buf[0] != 5 )
	{
		idLib::Printf( "NET_OpenSocks: bad response\n" );
		return;
	}
	// check completion code
	if( buf[1] != 0 )
	{
		idLib::Printf( "NET_OpenSocks: request denied: %i\n", buf[1] );
		return;
	}
	if( buf[3] != 1 )
	{
		idLib::Printf( "NET_OpenSocks: relay address is not IPV4: %i\n", buf[3] );
		return;
	}
	socksRelayAddr.sin_family = AF_INET;
	socksRelayAddr.sin_addr.s_addr = *( int* )&buf[4];
	socksRelayAddr.sin_port = *( short* )&buf[8];
	memset( socksRelayAddr.sin_zero, 0, sizeof( socksRelayAddr.sin_zero ) );
	
	usingSocks = true;
}

/*
========================
Net_WaitForData
========================
*/
bool Net_WaitForData( int netSocket, int timeout )
{
	int					ret;
	fd_set				set;
	struct timeval		tv;
	
	if( !netSocket )
	{
		return false;
	}
	
	if( timeout < 0 )
	{
		return true;
	}
	
	FD_ZERO( &set );
	FD_SET( netSocket, &set ); // TODO: winsocks may want an unsigned int for netSocket?
	
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = ( timeout % 1000 ) * 1000;
	
	ret = select( netSocket + 1, &set, nullptr, nullptr, &tv );
	
	if( ret == -1 )
	{
		idLib::Printf( "Net_WaitForData select(): %s\n", NET_ErrorString() );
		return false;
	}
	
	// timeout with no data
	if( ret == 0 )
	{
		return false;
	}
	
	return true;
}

/*
========================
Net_GetUDPPacket
========================
*/
bool Net_GetUDPPacket( int netSocket, netadr_t& net_from, char* data, int& size, int maxSize )
{
	int 			ret;
	sockaddr_in		from;
	socklen_t		fromlen;
	int				err;
	
	if( !netSocket )
	{
		return false;
	}
	
	fromlen = sizeof( from );
	ret = recvfrom( netSocket, data, maxSize, 0, ( sockaddr* )&from, &fromlen );
	if( ret == SOCKET_ERROR )
	{
		err = Net_GetLastError();
		
		if( err == D3_NET_EWOULDBLOCK || err == D3_NET_ECONNRESET )
		{
			return false;
		}
		
		idLib::Printf( "Net_GetUDPPacket: %s\n", NET_ErrorString() );
		return false;
	}
#if 0
	// TODO: WTF was this about?
	// DG: ip_socket is never initialized, so this is dead code
	// - and if netSocket is 0 (so this would be true) recvfrom above will already fail
	if( static_cast<unsigned int>( netSocket ) == ip_socket )
	{
		memset( from.sin_zero, 0, sizeof( from.sin_zero ) );
	}
	
	if( usingSocks && static_cast<unsigned int>( netSocket ) == ip_socket && memcmp( &from, &socksRelayAddr, fromlen ) == 0 )
	{
		if( ret < 10 || data[0] != 0 || data[1] != 0 || data[2] != 0 || data[3] != 1 )
		{
			return false;
		}
		net_from.type = NA_IP;
		net_from.ip[0] = data[4];
		net_from.ip[1] = data[5];
		net_from.ip[2] = data[6];
		net_from.ip[3] = data[7];
		net_from.port = *( short* )&data[8];
		memmove( data, &data[10], ret - 10 );
	}
	else
	{
#endif // 0
	
		Net_SockadrToNetadr( &from, &net_from );
#if 0 // this is ugly, but else astyle is confused
	}
#endif
	
	if( ret > maxSize )
	{
		idLib::Printf( "Net_GetUDPPacket: oversize packet from %s\n", Sys_NetAdrToString( net_from ) );
		return false;
	}
	
	size = ret;
	
	return true;
}

/*
========================
Net_SendUDPPacket
========================
*/
void Net_SendUDPPacket( int netSocket, int length, const void* data, const netadr_t to )
{
	int				ret;
	sockaddr_in		addr;
	
	if( !netSocket )
	{
		return;
	}
	
	Net_NetadrToSockadr( &to, &addr );
	
	if( usingSocks && to.type == NA_IP )
	{
		socksBuf[0] = 0;	// reserved
		socksBuf[1] = 0;
		socksBuf[2] = 0;	// fragment (not fragmented)
		socksBuf[3] = 1;	// address type: IPV4
		*( int* )&socksBuf[4] = addr.sin_addr.s_addr;
		*( short* )&socksBuf[8] = addr.sin_port;
		memcpy( &socksBuf[10], data, length );
		ret = sendto( netSocket, socksBuf, length + 10, 0, ( sockaddr* )&socksRelayAddr, sizeof( socksRelayAddr ) );
	}
	else
	{
		ret = sendto( netSocket, ( const char* )data, length, 0, ( sockaddr* )&addr, sizeof( addr ) );
	}
	if( ret == SOCKET_ERROR )
	{
		int err = Net_GetLastError();
		// some PPP links do not allow broadcasts and return an error
		if( ( err == D3_NET_EADDRNOTAVAIL ) && ( to.type == NA_BROADCAST ) )
		{
			return;
		}
		
		// NOTE: EWOULDBLOCK used to be silently ignored,
		// but that means the packet will be dropped so I don't feel it's a good thing to ignore
		idLib::Printf( "UDP sendto error - packet dropped: %s\n", NET_ErrorString() );
	}
}

static void ip_to_addr( const char ip[4], char* addr )
{
	idStr::snPrintf( addr, 16, "%d.%d.%d.%d", ( unsigned char )ip[0], ( unsigned char )ip[1],
					 ( unsigned char )ip[2], ( unsigned char )ip[3] );
}

/*
========================
Sys_StringToNetAdr
========================
*/
bool Sys_StringToNetAdr( const char* s, netadr_t* a, bool doDNSResolve )
{
	sockaddr_in sadr;
	
	if( !Net_StringToSockaddr( s, &sadr, doDNSResolve ) )
	{
		return false;
	}
	
	Net_SockadrToNetadr( &sadr, a );
	return true;
}



//} // namespace BFG