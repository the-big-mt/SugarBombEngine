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

#pragma once

//*****************************************************************************

//namespace BFG
//{

/*
================================================================================================
This file defines a set of template functors for generating callbacks, specifically
the OnChange handlers in the CVar system.
================================================================================================
*/

/*
================================================
idCallback
================================================
*/
class idCallback
{
public:
	virtual ~idCallback() {}
	virtual void Call() = 0;
	virtual idCallback* Clone() const = 0;
};

/*
================================================
idCallbackStatic

Callback class that forwards the call to a c-style function
================================================
*/
class idCallbackStatic : public idCallback
{
public:
	idCallbackStatic( void ( *f )() )
	{
		this->f = f;
	}
	void Call()
	{
		f();
	}
	idCallback* Clone() const
	{
		//idScopedGlobalHeap	everythingHereGoesInTheGlobalHeap;
		return new( TAG_FUNC_CALLBACK ) idCallbackStatic( f );
	}
private:
	void ( *f )();
};

/*
================================================
idCallbackBindMem

Callback class that forwards the call to a member function
================================================
*/
template< class T >
class idCallbackBindMem : public idCallback
{
public:
	idCallbackBindMem( T* t, void ( T::*f )() )
	{
		this->t = t;
		this->f = f;
	}
	void Call()
	{
		( t->*f )();
	}
	idCallback* Clone() const
	{
		return new( TAG_FUNC_CALLBACK ) idCallbackBindMem( t, f );
	}
private:
	T* t;
	void ( T::*f )();
};

/*
================================================
idCallbackBindMemArg1

Callback class that forwards the call to a member function with an additional constant parameter
================================================
*/
template< class T, typename A1 >
class idCallbackBindMemArg1 : public idCallback
{
public:
	idCallbackBindMemArg1( T* t_, void ( T::*f_ )( A1 ), A1 a1_ ) :
		t( t_ ),
		f( f_ ),
		a1( a1_ )
	{
	}
	void Call()
	{
		( t->*f )( a1 );
	}
	idCallback* Clone() const
	{
		return new( TAG_FUNC_CALLBACK ) idCallbackBindMemArg1( t, f, a1 );
	}
private:
	T* t;
	void ( T::*f )( A1 );
	A1 a1;
	
	// hack to get to compile on the 360 with reference arguments
	// with this on the PC, the MakeCallback function fails compilation because it's returning a copy
	// therefore, the Arg1 callbacks can't have arguments that are references
	//DISALLOW_COPY_AND_ASSIGN( idCallbackBindMemArg1 );
};

/*
================================================================================================

	These are needed because we can't derive the type of an object from the type passed to the
	constructor. If it weren't for these, we'd have to manually specify the type:

		idCallbackBindMem<idFoo>( this, &idFoo::MyFunction );
	becomes:
		MakeCallback( this, &idFoo::MyFunction );

================================================================================================
*/

/*
========================
MakeCallback
========================
*/
ID_INLINE_EXTERN idCallbackStatic MakeCallback( void ( *f )() )
{
	return idCallbackStatic( f );
}

/*
========================
MakeCallback
========================
*/
template < class T >
ID_INLINE_EXTERN idCallbackBindMem<T> MakeCallback( T* t, void ( T::*f )() )
{
	return idCallbackBindMem<T>( t, f );
}

/*
========================
MakeCallback
========================
*/
template < class T, typename A1 >
ID_INLINE_EXTERN idCallbackBindMemArg1<T, A1> MakeCallback( T* t, void ( T::*f )( A1 ), A1 a1 )
{
	return idCallbackBindMemArg1<T, A1>( t, f, a1 );
}

//} // namespace BFG