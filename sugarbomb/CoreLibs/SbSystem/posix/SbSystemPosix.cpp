/*
*******************************************************************************

Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
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

*******************************************************************************
*/

/// @file

#include "SbSystem.hpp"

namespace sbe::SbSystem
{

const int siglist[] =
{
	SIGHUP,
	SIGQUIT,
	SIGILL,
	SIGTRAP,
	SIGIOT,
	SIGBUS,
	SIGFPE,
	SIGSEGV,
	SIGPIPE,
	SIGABRT,
	//SIGTTIN,
	//SIGTTOU,
	-1
};

const char* signames[] =
{
	"SIGHUP",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGIOT",
	"SIGBUS",
	"SIGFPE",
	"SIGSEGV",
	"SIGPIPE",
	"SIGABRT",
	//"SIGTTIN",
	//"SIGTTOUT"
};

static char fatalError[1024]{};

/*
===============
Sys_FPE_handler
===============
*/
void Sys_FPE_handler( int signum, siginfo_t* info, void* context )
{
	assert( signum == SIGFPE );
	Sys_Printf( "FPE\n" );
};

/*
================
Posix_InitSigs
================
*/
void Posix_InitSigs()
{
	struct sigaction action;
	
	fatalError[0] = '\0';
	
	/* Set up the structure */
	action.sa_sigaction = sig_handler;
	sigemptyset( &action.sa_mask );
	action.sa_flags = SA_SIGINFO | SA_NODEFER;
	
	int i = 0;
	
	while( siglist[ i ] != -1 )
	{
		if( siglist[ i ] == SIGFPE )
		{
			action.sa_sigaction = Sys_FPE_handler;
			if( sigaction( siglist[ i ], &action, nullptr ) != 0 )
				Sys_Printf( "Failed to set SIGFPE handler: %s\n", strerror( errno ) );

			action.sa_sigaction = sig_handler;
		}
		else if( sigaction( siglist[ i ], &action, nullptr ) != 0 )
			Sys_Printf( "Failed to set %s handler: %s\n", signames[ i ], strerror( errno ) );

		i++;
	};
	
	// if the process is backgrounded (running non interactively)
	// then SIGTTIN or SIGTOU could be emitted, if not caught, turns into a SIGSTP
	signal( SIGTTIN, SIG_IGN );
	signal( SIGTTOU, SIG_IGN );
};

/*
===============
Posix_EarlyInit
===============
*/
void Posix_EarlyInit()
{
	//memset(&asyncThread, 0, sizeof(asyncThread));
	
	exit_spawn[0] = '\0';
	Posix_InitSigs();
	
	// set the base time
	Sys_Milliseconds();
	
	//Posix_InitPThreads();
};

void SbSystem::PlatformInit()
{
	Posix_EarlyInit();
};

}; // namespace sbe::SbSystem