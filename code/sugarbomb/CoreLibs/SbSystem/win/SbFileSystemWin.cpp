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

#include "precompiled.h"

/*
==============
Sys_Mkdir
==============
*/
void Sys_Mkdir( const char *path )
{
	_mkdir (path);
};

/*
========================
Sys_Rmdir
========================
*/
bool Sys_Rmdir( const char *path )
{
	return _rmdir( path ) == 0;
};

/*
========================
Sys_IsFolder
========================
*/
sysFolder_t Sys_IsFolder( const char *path )
{
	struct _stat buffer;
	if ( _stat( path, &buffer ) < 0 ) 
		return FOLDER_ERROR;

	return ( buffer.st_mode & _S_IFDIR ) != 0 ? FOLDER_YES : FOLDER_NO;
};

/*
==============
Sys_ListFiles
==============
*/
int Sys_ListFiles( const char *directory, const char *extension, idStrList &list )
{
	idStr		search;
	struct _finddata_t findinfo;
	// RB: 64 bit fixes, changed int to intptr_t
	intptr_t	findhandle;
	// RB end
	int			flag;

	if ( !extension)
		extension = "";

	// passing a slash as extension will find directories
	if ( extension[0] == '/' && extension[1] == 0 )
	{
		extension = "";
		flag = 0;
	else
		flag = _A_SUBDIR;

	sprintf( search, "%s\\*%s", directory, extension );

	// search
	list.Clear();

	findhandle = _findfirst( search, &findinfo );
	if ( findhandle == -1 )
		return -1;

	do
	{
		if ( flag ^ ( findinfo.attrib & _A_SUBDIR ) )
			list.Append( findinfo.name );
	}
	while ( _findnext( findhandle, &findinfo ) != -1 );

	_findclose( findhandle );

	return list.Num();
};

/*
==============
Sys_EXEPath
==============
*/
const char *Sys_EXEPath()
{
	static char exe[ MAX_OSPATH ];
	GetModuleFileName( nullptr, exe, sizeof( exe ) - 1 );
	return exe;
};

/*
==============
Sys_Cwd
==============
*/
const char *Sys_Cwd()
{
	static char cwd[MAX_OSPATH];

	_getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH-1] = 0;

	return cwd;
};

/*
==============
Sys_DefaultBasePath
==============
*/
const char *Sys_DefaultBasePath()
{
	return Sys_Cwd();
};

// Vista shit
typedef HRESULT (WINAPI * SHGetKnownFolderPath_t)( const GUID & rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath );
// NOTE: FOLIDERID_SavedGames is already exported from in shell32.dll in Windows 7.  We can only detect
// the compiler version, but that doesn't doesn't tell us which version of the OS we're linking against.
// This GUID value should never change, so we name it something other than FOLDERID_SavedGames to get
// around this problem.
const GUID FOLDERID_SavedGames_IdTech5 = { 0x4c5c32ff, 0xbb9d, 0x43b0, { 0xb5, 0xb4, 0x2d, 0x72, 0xe5, 0x4e, 0xaa, 0xa4 } };

/*
==============
Sys_DefaultSavePath
==============
*/
const char *Sys_DefaultSavePath()
{
	static char savePath[ MAX_PATH ];
	memset( savePath, 0, MAX_PATH );

	HMODULE hShell = LoadLibrary( "shell32.dll" );
	if ( hShell ) {
		SHGetKnownFolderPath_t SHGetKnownFolderPath = (SHGetKnownFolderPath_t)GetProcAddress( hShell, "SHGetKnownFolderPath" );
		if ( SHGetKnownFolderPath ) {
			wchar_t * path;

			// RB FIXME?
#if defined(__MINGW32__)
			if ( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_SavedGames_IdTech5, CSIDL_FLAG_CREATE, 0, &path ) ) )
#else
			if ( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_SavedGames_IdTech5, CSIDL_FLAG_CREATE | CSIDL_FLAG_PER_USER_INIT, 0, &path ) ) )
#endif
			// RB end
			{
				if ( wcstombs( savePath, path, MAX_PATH ) > MAX_PATH )
					savePath[0] = 0;

				CoTaskMemFree( path );
			};
		};
		FreeLibrary( hShell );
	};

	if ( savePath[0] == 0 )
	{
		// RB: looks like a bug in the shlobj.h
#if defined(__MINGW32__)
		SHGetFolderPath( nullptr, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, nullptr, 1, savePath );
#else
		SHGetFolderPath( nullptr, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, nullptr, SHGFP_TYPE_CURRENT, savePath );
#endif
		// RB end
		strcat( savePath, "\\My Games" );
	};

	strcat( savePath, SAVE_PATH );

	return savePath;
};