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

void SbNetworkSystem::Init()
{
};

void SbNetworkSystem::Shutdown()
{
};

int SbNetworkSystem::GetLocalIPCount() const
{
	return 0;
};

const char *SbNetworkSystem::GetLocalIP(int anIndex) const
{
	return "";
};