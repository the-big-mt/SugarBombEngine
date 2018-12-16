/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __CMDSYSTEM_H__
#define __CMDSYSTEM_H__

#include "framework/ICmdSystem.hpp"

/*
================================================
idCommandLink is a convenient way to get a function registered as a
ConsoleCommand without having to add an explicit call to idCmdSystem->AddCommand() in a startup
function somewhere. Simply declare a static variable with the parameters and it will get
executed before main(). For example:

static idCommandLink sys_dumpMemory( "sys_dumpMemory", Sys_DumpMemory_f, "Walks the heap and reports stats" );
================================================
*/

class idCommandLink
{
public:
	idCommandLink( const char* cmdName, cmdFunction_t function,
				   const char* description, argCompletion_t argCompletion = NULL );
	idCommandLink* 	next;
	const char* 	cmdName_;
	cmdFunction_t	function_;
	const char* 	description_;
	argCompletion_t argCompletion_;
};

// The command system will create commands for all the static definitions
// when it initializes.
idCommandLink* CommandLinks( idCommandLink* cl = NULL );

/*
================================================
The CONSOLE_COMMAND macro is an even easier way to create a console command by
automatically generating the idCommandLink variable, and it also allows all the
command code to be stripped from a build with a single define.  For example:

CONSOLE_COMMAND( Sys_DumpMemory, "Walks the heap and reports stats" ) {
	// do stuff
}

NOTE: All CONSOLE_COMMANDs will be stripped with the shipping build unless it's
created using the CONSOLE_COMMAND_SHIP macro.
================================================
*/

#if defined ( ID_RETAIL ) && !defined( ID_RETAIL_INTERNAL )
#define CONSOLE_COMMAND_SHIP			CONSOLE_COMMAND_COMPILE
#define CONSOLE_COMMAND					CONSOLE_COMMAND_NO_COMPILE
// We need to disable this warning to get commands that were made friends
// of classes to compile as inline.
// warning C4211: nonstandard extension used : redefined extern to static
#pragma warning( disable : 4211 )
// warning C4505: 'xxx' : unreferenced local function has been removed
#pragma warning( disable : 4505 )
#else
#define CONSOLE_COMMAND_SHIP			CONSOLE_COMMAND_COMPILE
#define CONSOLE_COMMAND					CONSOLE_COMMAND_COMPILE
#endif

// Turn console commands into static inline code, which will cause them to be
// removed from the build.
#define CONSOLE_COMMAND_NO_COMPILE( name, comment, completion ) \
	static inline void name ## _f( const idCmdArgs &args )

// lint incorrectly gives this for all console commands: Issue 1568: (Warning -- Variable 'TestAtomicString_v' accesses variable 'atomicStringManager' before the latter is initialized through calls: 'TestAtomicString_f() => idAtomicString::FreeDynamic()')
// I can't figure out how to disable this just around CONSOLE_COMMAND, so it must stay disabled everywhere,
// which is a shame.
//lint -e1568
#define CONSOLE_COMMAND_COMPILE( name, comment, completion ) \
	void name ## _f( const idCmdArgs &args ); \
	idCommandLink name ## _v( #name, name ## _f, comment, completion  ); \
	void name ## _f( const idCmdArgs &args )

extern idCmdSystem* 	cmdSystem;


ID_INLINE void idCmdSystem::ArgCompletion_Boolean( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	callback( va( "%s 0", args.Argv( 0 ) ) );
	callback( va( "%s 1", args.Argv( 0 ) ) );
}

template<int min, int max> ID_INLINE void idCmdSystem::ArgCompletion_Integer( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	for( int i = min; i <= max; i++ )
	{
		callback( va( "%s %d", args.Argv( 0 ), i ) );
	}
}

template<const char** strings> ID_INLINE void idCmdSystem::ArgCompletion_String( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	for( int i = 0; strings[i]; i++ )
	{
		callback( va( "%s %s", args.Argv( 0 ), strings[i] ) );
	}
}

template<int type> ID_INLINE void idCmdSystem::ArgCompletion_Decl( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_DeclName( args, callback, type );
}

ID_INLINE void idCmdSystem::ArgCompletion_FileName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "/", true, "", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_MapName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "maps/", true, ".map", ".json", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_MapNameNoJson( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "maps/", true, ".map", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_ModelName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "models/", false, ".lwo", ".ase", ".md5mesh", ".ma", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_SoundName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "sound/", false, ".wav", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_ImageName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "/", false, ".tga", ".dds", ".jpg", ".pcx", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_VideoName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "/", false, ".bik", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_ConfigName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "/", true, ".cfg", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_SaveGame( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "SaveGames/", true, ".save", NULL );
}

ID_INLINE void idCmdSystem::ArgCompletion_DemoName( const idCmdArgs& args, void( *callback )( const char* s ) )
{
	cmdSystem->ArgCompletion_FolderExtension( args, callback, "demos/", true, ".demo", NULL );
}

#endif /* !__CMDSYSTEM_H__ */
