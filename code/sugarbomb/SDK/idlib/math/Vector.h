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

#include <cstring> // for memcpy

//#include "idlib/containers/Array.h" // for idTupleSize
#include "idlib/math/Math.h"
#include "idlib/sys/sys_defines.h"
#include "idlib/sys/sys_assert.h"

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

  Vector classes

===============================================================================
*/

#include "../containers/Array.h" // for idTupleSize

#define VECTOR_EPSILON		0.001f

class idAngles;
class idPolar3;
class idMat3;

//===============================================================
//
//	idTupleSize< idVec3 > - Specialization to get the size
//	of an idVec3 generically.
//
//===============================================================

template<>
struct idTupleSize< idVec3 >
{
	enum { value = 3 };
};

//===============================================================
//
//	idPolar3
//
//===============================================================

class idPolar3
{
public:
	float			radius, theta, phi;
	
	idPolar3();
	explicit idPolar3( const float radius, const float theta, const float phi );
	
	void 			Set( const float radius, const float theta, const float phi );
	
	float			operator[]( const int index ) const;
	float& 			operator[]( const int index );
	idPolar3		operator-() const;
	idPolar3& 		operator=( const idPolar3& a );
	
	idVec3			ToVec3() const;
};

ID_INLINE idPolar3::idPolar3()
{
}

ID_INLINE idPolar3::idPolar3( const float radius, const float theta, const float phi )
{
	assert( radius > 0 );
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}

ID_INLINE void idPolar3::Set( const float radius, const float theta, const float phi )
{
	assert( radius > 0 );
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}

ID_INLINE float idPolar3::operator[]( const int index ) const
{
	return ( &radius )[ index ];
}

ID_INLINE float& idPolar3::operator[]( const int index )
{
	return ( &radius )[ index ];
}

ID_INLINE idPolar3 idPolar3::operator-() const
{
	return idPolar3( radius, -theta, -phi );
}

ID_INLINE idPolar3& idPolar3::operator=( const idPolar3& a )
{
	radius = a.radius;
	theta = a.theta;
	phi = a.phi;
	return *this;
}

ID_INLINE idVec3 idPolar3::ToVec3() const
{
	float sp, cp, st, ct;
	idMath::SinCos( phi, sp, cp );
	idMath::SinCos( theta, st, ct );
	return idVec3( cp * radius * ct, cp * radius * st, radius * sp );
}


/*
===============================================================================

	Old 3D vector macros, should no longer be used.

===============================================================================
*/

#define	VectorMA( v, s, b, o )		((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

//} // namespace BFG

#endif /* !__MATH_VECTOR_H__ */
