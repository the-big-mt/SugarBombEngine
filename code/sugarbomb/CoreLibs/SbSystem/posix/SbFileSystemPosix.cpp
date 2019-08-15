/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "precompiled.h"

/*
================
Sys_Mkdir
================
*/
void Sys_Mkdir( const char* path )
{
	mkdir( path, 0777 );
};

/*
================
Sys_Rmdir
================
*/
bool Sys_Rmdir( const char* path )
{
	return ( rmdir( path ) == 0 );
};

/*
========================
Sys_IsFolder
========================
*/
sysFolder_t Sys_IsFolder( const char* path )
{
	struct stat buffer;
	
	if( stat( path, &buffer ) < 0 )
		return FOLDER_ERROR;
	
	return ( buffer.st_mode & S_IFDIR ) != 0 ? FOLDER_YES : FOLDER_NO;
};

/*
================
Sys_ListFiles
================
*/
int Sys_ListFiles( const char* directory, const char* extension, idStrList& list )
{
	struct dirent* d;
	DIR* fdir;
	bool dironly = false;
	char search[MAX_OSPATH];
	struct stat st;
	bool debug;
	
	list.Clear();
	
	debug = cvarSystem->GetCVarBool( "fs_debug" );
	// DG: we use fnmatch for shell-style pattern matching
	// so the pattern should at least contain "*" to match everything,
	// the extension will be added behind that (if !dironly)
	idStr pattern( "*" );
	
	// passing a slash as extension will find directories
	if( extension[0] == '/' && extension[1] == 0 )
		dironly = true;
	else
	{
		// so we have *<extension>, the same as in the windows code basically
		pattern += extension;
	};
	// DG end
	
	// NOTE: case sensitivity of directory path can screw us up here
	if( ( fdir = opendir( directory ) ) == nullptr )
	{
		if( debug )
			common->Printf( "Sys_ListFiles: opendir %s failed\n", directory );

		return -1;
	};
	
	// DG: use readdir_r instead of readdir for thread safety
	// the following lines are from the readdir_r manpage.. fscking ugly.
	int nameMax = pathconf( directory, _PC_NAME_MAX );
	if( nameMax == -1 )
		nameMax = 255;
	int direntLen = offsetof( struct dirent, d_name ) + nameMax + 1;
	
	struct dirent* entry = ( struct dirent* )Mem_Alloc( direntLen, TAG_CRAP );
	
	if( entry == nullptr )
	{
		common->Warning( "Sys_ListFiles: Mem_Alloc for entry failed!" );
		closedir( fdir );
		return 0;
	};
	
	while( readdir_r( fdir, entry, &d ) == 0 && d != nullptr )
	{
		// DG end
		idStr::snPrintf( search, sizeof( search ), "%s/%s", directory, d->d_name );
		if( stat( search, &st ) == -1 )
			continue;
		if( !dironly )
		{
			// DG: the original code didn't work because d3 bfg abuses the extension
			// to match whole filenames and patterns in the savegame-code, not just file extensions...
			// so just use fnmatch() which supports matching shell wildcard patterns ("*.foo" etc)
			// if we should ever need case insensitivity, use FNM_CASEFOLD as third flag
			if( fnmatch( pattern.c_str(), d->d_name, 0 ) != 0 )
				continue;
			// DG end
		};

		if( ( dironly && !( st.st_mode & S_IFDIR ) ) ||
				( !dironly && ( st.st_mode & S_IFDIR ) ) )
			continue;
			
		list.Append( d->d_name );
	};
	
	closedir( fdir );
	Mem_Free( entry );
	
	if( debug )
		common->Printf( "Sys_ListFiles: %d entries in %s\n", list.Num(), directory );
	
	return list.Num();
};

/*
================
Posix_Cwd
================
*/
const char* Posix_Cwd()
{
	static char cwd[MAX_OSPATH];
	
	getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH - 1] = 0;
	
	return cwd;
};

/*
================
Sys_DefaultBasePath

Get the default base path
- binary image path
- current directory
- hardcoded
Try to be intelligent: if there is no BASE_GAMEDIR, try the next path
================
*/
const char* Sys_DefaultBasePath()
{
	struct stat st;
	idStr testbase;
	basepath = Sys_EXEPath();
	if( basepath.Length() )
	{
		basepath.StripFilename();
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
			return basepath.c_str();
		else
			common->Printf( "no '%s' directory in exe path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
	};

	if( basepath != Posix_Cwd() )
	{
		basepath = Posix_Cwd();
		testbase = basepath;
		testbase += "/";
		testbase += BASE_GAMEDIR;
		if( stat( testbase.c_str(), &st ) != -1 && S_ISDIR( st.st_mode ) )
			return basepath.c_str();
		else
			common->Printf( "no '%s' directory in cwd path %s, skipping\n", BASE_GAMEDIR, basepath.c_str() );
	};
	common->Printf( "WARNING: using hardcoded default base path %s\n", DEFAULT_BASEPATH );
	return DEFAULT_BASEPATH;
};

/*
 ==============
 Sys_DefaultSavePath
 ==============
 */
const char* Sys_DefaultSavePath()
{
#if defined(__APPLE__)
	char* base_path = SDL_GetPrefPath( "", "RBDOOM-3-BFG" );
	if( base_path )
	{
		savepath = SDL_strdup( base_path );
		SDL_free( base_path );
	};
#else
	sprintf( savepath, "%s/.rbdoom3bfg", getenv( "HOME" ) );
#endif
	
	return savepath.c_str();
};