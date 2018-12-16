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

#ifndef __CVARSYSTEM_H__
#define __CVARSYSTEM_H__

#include "framework/ICVarSystem.hpp"

/*
===============================================================================

	idCVar

===============================================================================
*/

class idCVar
{
public:
	// Never use the default constructor.
	idCVar()
	{
		assert( typeid( this ) != typeid( idCVar ) );
	}
	
	// Always use one of the following constructors.
	idCVar( const char* name, const char* value, int flags, const char* description,
			argCompletion_t valueCompletion = NULL );
	idCVar( const char* name, const char* value, int flags, const char* description,
			float valueMin, float valueMax, argCompletion_t valueCompletion = NULL );
	idCVar( const char* name, const char* value, int flags, const char* description,
			const char** valueStrings, argCompletion_t valueCompletion = NULL );
			
	virtual					~idCVar() {}
	
	const char* 			GetName() const
	{
		return internalVar->name;
	}
	int						GetFlags() const
	{
		return internalVar->flags;
	}
	const char* 			GetDescription() const
	{
		return internalVar->description;
	}
	float					GetMinValue() const
	{
		return internalVar->valueMin;
	}
	float					GetMaxValue() const
	{
		return internalVar->valueMax;
	}
	const char** 			GetValueStrings() const
	{
		return valueStrings;
	}
	argCompletion_t			GetValueCompletion() const
	{
		return valueCompletion;
	}
	
	bool					IsModified() const
	{
		return ( internalVar->flags & CVAR_MODIFIED ) != 0;
	}
	void					SetModified()
	{
		internalVar->flags |= CVAR_MODIFIED;
	}
	void					ClearModified()
	{
		internalVar->flags &= ~CVAR_MODIFIED;
	}
	
	const char* 			GetDefaultString() const
	{
		return internalVar->InternalGetResetString();
	}
	const char* 			GetString() const
	{
		return internalVar->value;
	}
	bool					GetBool() const
	{
		return ( internalVar->integerValue != 0 );
	}
	int						GetInteger() const
	{
		return internalVar->integerValue;
	}
	float					GetFloat() const
	{
		return internalVar->floatValue;
	}
	
	void					SetString( const char* value )
	{
		internalVar->InternalSetString( value );
	}
	void					SetBool( const bool value )
	{
		internalVar->InternalSetBool( value );
	}
	void					SetInteger( const int value )
	{
		internalVar->InternalSetInteger( value );
	}
	void					SetFloat( const float value )
	{
		internalVar->InternalSetFloat( value );
	}
	
	void					SetInternalVar( idCVar* cvar )
	{
		internalVar = cvar;
	}
	
	static void				RegisterStaticVars();
	
protected:
	const char* 			name;					// name
	const char* 			value;					// value
	const char* 			description;			// description
	int						flags;					// CVAR_? flags
	float					valueMin;				// minimum value
	float					valueMax;				// maximum value
	const char** 			valueStrings;			// valid value strings
	argCompletion_t			valueCompletion;		// value auto-completion function
	int						integerValue;			// atoi( string )
	float					floatValue;				// atof( value )
	idCVar* 				internalVar;			// internal cvar
	idCVar* 				next;					// next statically declared cvar
	
private:
	void					Init( const char* name, const char* value, int flags, const char* description,
								  float valueMin, float valueMax, const char** valueStrings, argCompletion_t valueCompletion );
								  
	virtual void			InternalSetString( const char* newValue ) {}
	virtual void			InternalSetBool( const bool newValue ) {}
	virtual void			InternalSetInteger( const int newValue ) {}
	virtual void			InternalSetFloat( const float newValue ) {}
	
	virtual const char* 	InternalGetResetString() const
	{
		return value;
	}
	
	static idCVar* 			staticVars;
};

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description,
						  argCompletion_t valueCompletion )
{
	if( !valueCompletion && ( flags & CVAR_BOOL ) )
	{
		valueCompletion = idCmdSystem::ArgCompletion_Boolean;
	}
	Init( name, value, flags, description, 1, -1, NULL, valueCompletion );
}

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description,
						  float valueMin, float valueMax, argCompletion_t valueCompletion )
{
	Init( name, value, flags, description, valueMin, valueMax, NULL, valueCompletion );
}

ID_INLINE idCVar::idCVar( const char* name, const char* value, int flags, const char* description,
						  const char** valueStrings, argCompletion_t valueCompletion )
{
	Init( name, value, flags, description, 1, -1, valueStrings, valueCompletion );
}

extern idCVarSystem* 		cvarSystem;


/*
===============================================================================

	CVar Registration

	Each DLL using CVars has to declare a private copy of the static variable
	idCVar::staticVars like this: idCVar * idCVar::staticVars = NULL;
	Furthermore idCVar::RegisterStaticVars() has to be called after the
	cvarSystem pointer is set when the DLL is first initialized.

===============================================================================
*/

ID_INLINE void idCVar::Init( const char* name, const char* value, int flags, const char* description,
							 float valueMin, float valueMax, const char** valueStrings, argCompletion_t valueCompletion )
{
	this->name = name;
	this->value = value;
	this->flags = flags;
	this->description = description;
	this->flags = flags | CVAR_STATIC;
	this->valueMin = valueMin;
	this->valueMax = valueMax;
	this->valueStrings = valueStrings;
	this->valueCompletion = valueCompletion;
	this->integerValue = 0;
	this->floatValue = 0.0f;
	this->internalVar = this;
	if( staticVars != ( idCVar* )UINTPTR_MAX )
	{
		this->next = staticVars;
		staticVars = this;
	}
	else
	{
		cvarSystem->Register( this );
	}
}

ID_INLINE void idCVar::RegisterStaticVars()
{
	if( staticVars != ( idCVar* )UINTPTR_MAX )
	{
		for( idCVar* cvar = staticVars; cvar; cvar = cvar->next )
		{
			cvarSystem->Register( cvar );
		}
		staticVars = ( idCVar* )UINTPTR_MAX;
	}
}

#endif /* !__CVARSYSTEM_H__ */
