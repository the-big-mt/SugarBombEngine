/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#ifndef __CMDSYSTEM_H__
#define __CMDSYSTEM_H__

#include "idlib/sys/sys_types.h"
#include "idlib/sys/sys_defines.h"
#include "idlib/Str.h"
#include "idlib/containers/List.h"
#include "idlib/containers/StrList.h"

#include "framework/ICmdSystem.hpp"

namespace sbe
{

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

// soundname

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

}; // namespace sbe

#endif /* !__CMDSYSTEM_H__ */
