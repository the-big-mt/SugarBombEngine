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

#include <windows.h>

#include "SbSystem.hpp"

//*****************************************************************************

namespace sbe::SbSystem
{

typedef BOOL (WINAPI *LPFN_GLPI)( PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD );

enum LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL
{
    localRelationProcessorCore,
    localRelationNumaNode,
    localRelationCache,
	localRelationProcessorPackage
};

struct cpuInfo_t {
	int processorPackageCount;
	int processorCoreCount;
	int logicalProcessorCount;
	int numaNodeCount;
	struct cacheInfo_t {
		int count;
		int associativity;
		int lineSize;
		int size;
	} cacheLevel[3];
};

/*
========================
GetCPUInfo
========================
*/
bool GetCPUInfo( cpuInfo_t & cpuInfo )
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = nullptr;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = nullptr;
	PCACHE_DESCRIPTOR Cache;
	LPFN_GLPI	glpi;
	BOOL		done = FALSE;
	DWORD		returnLength = 0;
	DWORD		byteOffset = 0;

	memset( & cpuInfo, 0, sizeof( cpuInfo ) );

	glpi = (LPFN_GLPI)GetProcAddress( GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation" );
	if ( nullptr == glpi )
	{
		idLib::Printf( "\nGetLogicalProcessorInformation is not supported.\n" );
		return 0;
	};

	while ( !done )
	{
		DWORD rc = glpi( buffer, &returnLength );

		if ( FALSE == rc )
		{
			if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
			{
				if ( buffer )
					free( buffer );

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc( returnLength );
			}
			else
			{
				idLib::Printf( "Sys_CPUCount error: %d\n", GetLastError() );
				return false;
			};
		};
		else
			done = TRUE;
	};

	ptr = buffer;

	while ( byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength )
	{
		switch ( (LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL) ptr->Relationship )
		{
			case localRelationProcessorCore:
				cpuInfo.processorCoreCount++;

				// A hyperthreaded core supplies more than one logical processor.
				cpuInfo.logicalProcessorCount += CountSetBits( ptr->ProcessorMask );
				break;

			case localRelationNumaNode:
				// Non-NUMA systems report a single record of this type.
				cpuInfo.numaNodeCount++;
				break;

			case localRelationCache:
				// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
				Cache = &ptr->Cache;
				if ( Cache->Level >= 1 && Cache->Level <= 3 )
				{
					int level = Cache->Level - 1;
					if ( cpuInfo.cacheLevel[level].count > 0 )
						cpuInfo.cacheLevel[level].count++;
					else
					{
						cpuInfo.cacheLevel[level].associativity = Cache->Associativity;
						cpuInfo.cacheLevel[level].lineSize = Cache->LineSize;
						cpuInfo.cacheLevel[level].size = Cache->Size;
					};
				};
				break;

			case localRelationProcessorPackage:
				// Logical processors share a physical package.
				cpuInfo.processorPackageCount++;
				break;

			default:
				idLib::Printf( "Error: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n" );
				break;
		};
		byteOffset += sizeof( SYSTEM_LOGICAL_PROCESSOR_INFORMATION );
		ptr++;
	};

	free( buffer );

	return true;
};

/*
========================
Sys_GetCmdLine
========================
*/
const char *SbSystem::GetCmdLine() const
{
	return sys_cmdline;
};

/*
========================
Sys_CPUCount

numLogicalCPUCores	- the number of logical CPU per core
numPhysicalCPUCores	- the total number of cores per package
numCPUPackages		- the total number of packages (physical processors)
========================
*/
void SbSystem::GetCPUCount(int &anLogicalCPUCores, int &anPhysicalCPUCores, int &anCPUPackages) const
{
	cpuInfo_t cpuInfo{};
	GetCPUInfo(cpuInfo);

	anPhysicalCPUCores = cpuInfo.processorCoreCount;
	anLogicalCPUCores = cpuInfo.logicalProcessorCount;
	anCPUPackages = cpuInfo.processorPackageCount;
};

/*
=============
Sys_Error

Show the early console as an error dialog
=============
*/
void SbSystem::Error(const char *asMsg, ...)
{
	va_list argptr;
	char sFullErrorMsg[4096]{};

	va_start(argptr, asMsg);
	std::vsprintf(sFullErrorMsg, asMsg, argptr);
	va_end(argptr);

	Conbuf_AppendText(sFullErrorMsg);
	Conbuf_AppendText("\n");

	Win_SetErrorText(sFullErrorMsg);
	Sys_ShowConsole(1, true);

	timeEndPeriod(1);

	for(auto It : mlstEventListeners)
		It->OnError(sFullErrorMsg);
	
	extern idCVar com_productionMode;
	if(com_productionMode.GetInteger() == 0)
	{
		MSG msg;
		
		// wait for the user to quit
		while(1)
		{
			if(!GetMessage(&msg, nullptr, 0, 0))
				common->Quit();

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};
	};
	
	Sys_DestroyConsole();

	exit(EXIT_FAILURE);
};

}; // namespace sbe::SbSystem