/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

//*****************************************************************************

#pragma hdrstop
#include "precompiled.h"

//#include <csignal>                     // for raise, SIGTRAP
//#include <cstddef>

#include "framework/CVar.hpp" //#include "framework/ICVarSystem.hpp" // for ::CVAR_BOOL, etc

//#include "idlib/Lib.h"               // for idLib
//#include "idlib/containers/StaticList.h"      // for idStaticList
//#include "idlib/sys/sys_types.h"     // for nullptr

#ifndef _WIN32
#include <csignal> // for raise()
#endif

//*****************************************************************************

//namespace sbe
//{

/*
================================================================================================
Contains the AssertMacro implementation.
================================================================================================
*/

idCVar com_assertOutOfDebugger( "com_assertOutOfDebugger", "0", CVAR_BOOL, "by default, do not assert while not running under the debugger" );

struct skippedAssertion_t
{
	skippedAssertion_t() :
		file( nullptr ),
		line( -1 )
	{
	}
	const char* 	file;
	int				line;
};
static idStaticList< skippedAssertion_t, 20 > skippedAssertions;

/*
========================
AssertFailed
========================
*/
bool AssertFailed( const char* file, int line, const char* expression )
{
	// Set this to true to skip ALL assertions, including ones YOU CAUSE!
	static volatile bool skipAllAssertions = false;
	if( skipAllAssertions )
	{
		return false;
	}
	
	// Set this to true to skip ONLY this assertion
	static volatile bool skipThisAssertion = false;
	skipThisAssertion = false;
	
	for( int i = 0; i < skippedAssertions.Num(); i++ )
	{
		if( skippedAssertions[i].file == file && skippedAssertions[i].line == line )
		{
			skipThisAssertion = true;
			// Set breakpoint here to re-enable
			if( !skipThisAssertion )
			{
				skippedAssertions.RemoveIndexFast( i );
			}
			return false;
		}
	}
	
	idLib::Warning( "ASSERTION FAILED! %s(%d): '%s'", file, line, expression );
	
// RB begin
#ifdef _WIN32
	if( IsDebuggerPresent() || com_assertOutOfDebugger.GetBool() )
#else
	//if( com_assertOutOfDebugger.GetBool() )
#endif
// RB end
	{
#ifdef _WIN32
#ifdef _MSC_VER
		__debugbreak();
#else
		// DG: mingw support
		DebugBreak();
#endif
#else // not _WIN32
		// DG: POSIX support
		raise( SIGTRAP );
		// DG: end
#endif // _WIN32
	}
	
	if( skipThisAssertion )
	{
		skippedAssertion_t* skipped = skippedAssertions.Alloc();
		skipped->file = file;
		skipped->line = line;
	}
	
	return true;
}

//} // namespace sbe