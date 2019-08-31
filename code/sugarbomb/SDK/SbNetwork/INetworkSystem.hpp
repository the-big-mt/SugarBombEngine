/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
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

#pragma once

/*
==============================================================

	Networking

==============================================================
*/

#include "NetworkTypes.hpp"
namespace sbe
{

/*
================================================
idUDP
================================================
*/
class idUDP
{
public:
	// this just zeros netSocket and port
	idUDP();
	virtual ~idUDP();
	
	/// if the InitForPort fails, the idUDP.port field will remain 0
	bool InitForPort(int portNumber);
	
	int GetPort() const
	{
		return bound_to.port;
	};
	netadr_t GetAdr() const
	{
		return bound_to;
	};
	uint32 GetUIntAdr() const
	{
		return (bound_to.ip[0] | bound_to.ip[1] << 8 | bound_to.ip[2] << 16 | bound_to.ip[3] << 24);
	};
	void Close();
	
	void SendPacket(const netadr_t &to, const void *data, int size);

	bool GetPacket(netadr_t &from, void *data, int &size, int maxSize); // TODO: ReceivePacket?
	bool GetPacketBlocking(netadr_t &from, void *data, int &size, int maxSize, int timeout); // TODO: ReceivePacketBlocking?
	
	void SetSilent(bool silent)
	{
		this->silent = silent;
	};
	bool GetSilent() const // TODO: IsSilent?
	{
		return silent;
	};
	
	int packetsRead{0};
	int bytesRead{0};
	
	int packetsWritten{0};
	int bytesWritten{0};
	
	bool IsOpen() const
	{
		return netSocket > 0;
	};
private:
	netadr_t bound_to{}; ///< interface and port
	int netSocket{0}; ///< OS specific socket
	bool silent{false}; ///< don't emit anything ( black hole )
};

// parses the port number
// can also do DNS resolve if you ask for it.
// NOTE: DNS resolve is a slow/blocking call, think before you use
// ( could be exploited for server DoS )
bool			Sys_StringToNetAdr( const char* s, netadr_t* a, bool doDNSResolve );

int				Sys_GetLocalIPCount();
const char* 	Sys_GetLocalIP( int i );

void			Sys_InitNetworking();
void			Sys_ShutdownNetworking();

struct INetworkSystem
{
	///
	virtual void Init() = 0;

	///
	virtual void Shutdown() = 0;

	///
	virtual int GetLocalIPCount() const = 0;

	///
	virtual const char *GetLocalIP(int i) const = 0;
};

/*
===============================================================================

	Networking API

===============================================================================
*/

const auto NET_API_VERSION{1};

struct idCommon;
struct idCmdSystem;
struct idCVarSystem;
struct idFileSystem;

struct netImport_t
{
	int version{-1}; ///< API version
	idCommon *common{nullptr}; ///< common
	idCmdSystem *cmdSystem{nullptr}; ///< console command system
	idCVarSystem *cvarSystem{nullptr}; ///< console variable system
	idFileSystem *fileSystem{nullptr}; ///< file system
};

struct netExport_t
{
	int version{-1}; ///< API version
	INetwork *netSystem{nullptr};
};

extern "C" using GetNetAPI_t = netExport_t *(*)(netImport_t *import);
}; // namespace sbe