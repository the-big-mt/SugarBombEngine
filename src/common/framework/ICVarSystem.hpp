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

/// @file

//*****************************************************************************

#pragma once

#include "idlib/sys/sys_types.h"

//*****************************************************************************

/*
===============================================================================

	Console Variables (CVars) are used to hold scalar or string variables
	that can be changed or displayed at the console as well as accessed
	directly in code.

	CVars are mostly used to hold settings that can be changed from the
	console or saved to and loaded from configuration files. CVars are also
	occasionally used to communicate information between different modules
	of the program.

	CVars are restricted from having the same names as console commands to
	keep the console interface from being ambiguous.

	CVars can be accessed from the console in three ways:
	cvarName			prints the current value
	cvarName X			sets the value to X if the variable exists
	set cvarName X		as above, but creates the CVar if not present

	CVars may be declared in the global namespace, in classes and in functions.
	However declarations in classes and functions should always be static to
	save space and time. Making CVars static does not change their
	functionality due to their global nature.

	CVars should be contructed only through one of the constructors with name,
	value, flags and description. The name, value and description parameters
	to the constructor have to be static strings, do not use va() or the like
	functions returning a string.

	CVars may be declared multiple times using the same name string. However,
	they will all reference the same value and changing the value of one CVar
	changes the value of all CVars with the same name.

	CVars should always be declared with the correct type flag: CVAR_BOOL,
	CVAR_INTEGER or CVAR_FLOAT. If no such flag is specified the CVar
	defaults to type string. If the CVAR_BOOL flag is used there is no need
	to specify an argument auto-completion function because the CVar gets
	one assigned automatically.

	CVars are automatically range checked based on their type and any min/max
	or valid string set specified in the constructor.

	CVars are always considered cheats except when CVAR_NOCHEAT, CVAR_INIT,
	CVAR_ROM, CVAR_ARCHIVE, CVAR_SERVERINFO, CVAR_NETWORKSYNC
	is set.

===============================================================================
*/

namespace sbe
{

typedef enum
{
	CVAR_ALL				= -1,		// all flags
	CVAR_BOOL				= BIT( 0 ),	// variable is a boolean
	CVAR_INTEGER			= BIT( 1 ),	// variable is an integer
	CVAR_FLOAT				= BIT( 2 ),	// variable is a float
	CVAR_SYSTEM				= BIT( 3 ),	// system variable
	CVAR_RENDERER			= BIT( 4 ),	// renderer variable
	CVAR_SOUND				= BIT( 5 ),	// sound variable
	CVAR_GUI				= BIT( 6 ),	// gui variable
	CVAR_GAME				= BIT( 7 ),	// game variable
	CVAR_TOOL				= BIT( 8 ),	// tool variable
	// original doom3 used to have CVAR_USERINFO ("sent to servers, available to menu") here
	CVAR_SERVERINFO			= BIT( 10 ),	// sent from servers, available to menu
	CVAR_NETWORKSYNC		= BIT( 11 ),	// cvar is synced from the server to clients
	CVAR_STATIC				= BIT( 12 ),	// statically declared, not user created
	CVAR_CHEAT				= BIT( 13 ),	// variable is considered a cheat
	CVAR_NOCHEAT			= BIT( 14 ),	// variable is not considered a cheat
	CVAR_INIT				= BIT( 15 ),	// can only be set from the command-line
	CVAR_ROM				= BIT( 16 ),	// display only, cannot be set by user at all
	CVAR_ARCHIVE			= BIT( 17 ),	// set to cause it to be saved to a config file
	CVAR_MODIFIED			= BIT( 18 )	// set when the variable is modified
} cvarFlags_t;

/*
===============================================================================

	idCVarSystem

===============================================================================
*/

struct idCVar;
struct idCmdArgs;
struct idFile;
struct idDict;

struct SbCVarSystem
{
	virtual void			Init() = 0;
	virtual void			Shutdown() = 0;
	virtual bool			IsInitialized() const = 0;
	
	/// Registers a CVar
	virtual void			Register( idCVar* cvar ) = 0;
	
	/// Finds the CVar with the given name
	/// Returns nullptr if there is no CVar with the given name
	virtual idCVar* 		Find( const char* name ) = 0;
	
	/// Sets the value of a CVar by name
	virtual void			SetCVarString( const char* name, const char* value, int flags = 0 ) = 0;
	virtual void			SetCVarBool( const char* name, const bool value, int flags = 0 ) = 0;
	virtual void			SetCVarInteger( const char* name, const int value, int flags = 0 ) = 0;
	virtual void			SetCVarFloat( const char* name, const float value, int flags = 0 ) = 0;
	
	/// Gets the value of a CVar by name
	virtual const char* 	GetCVarString( const char* name ) const = 0;
	virtual bool			GetCVarBool( const char* name ) const = 0;
	virtual int				GetCVarInteger( const char* name ) const = 0;
	virtual float			GetCVarFloat( const char* name ) const = 0;
	
	/// Called by the command system when argv(0) doesn't match a known command
	/// Returns true if argv(0) is a variable reference and prints or changes the CVar
	virtual bool			Command( const idCmdArgs& args ) = 0;
	
	/// Command and argument completion using callback for each valid string
	virtual void			CommandCompletion( void( *callback )( const char* s ) ) = 0;
	virtual void			ArgCompletion( const char* cmdString, void( *callback )( const char* s ) ) = 0;
	
	/// Sets/gets/clears modified flags that tell what kind of CVars have changed
	virtual void			SetModifiedFlags( int flags ) = 0;
	virtual int				GetModifiedFlags() const = 0;
	virtual void			ClearModifiedFlags( int flags ) = 0;
	
	/// Resets variables with one of the given flags set
	virtual void			ResetFlaggedVariables( int flags ) = 0;
	
	/// Removes auto-completion from the flagged variables
	virtual void			RemoveFlaggedAutoCompletion( int flags ) = 0;
	
	/// Writes variables with one of the given flags set to the given file
	virtual void			WriteFlaggedVariables( int flags, const char* setCmd, idFile* f ) const = 0;
	
	/// Moves CVars to and from dictionaries
	virtual void			MoveCVarsToDict( int flags, idDict& dict, bool onlyModified = false ) const = 0;
	virtual void			SetCVarsFromDict( const idDict& dict ) = 0;
};

struct idCVarSystem : public SbCVarSystem
{
	virtual ~idCVarSystem() = default;
};

}; // namespace sbe