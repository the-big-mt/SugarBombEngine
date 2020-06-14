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
//	idVec5 - 5D vector
//
//===============================================================

class idVec5
{
public:
	float			x;
	float			y;
	float			z;
	float			s;
	float			t;
	
	idVec5();
	explicit idVec5( const idVec3& xyz, const idVec2& st );
	explicit idVec5( const float x, const float y, const float z, const float s, const float t );
	
	float			operator[]( int index ) const;
	float& 			operator[]( int index );
	idVec5& 		operator=( const idVec3& a );
	
	int				GetDimension() const;
	
	const idVec3& 	ToVec3() const;
	idVec3& 		ToVec3();
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
	void			Lerp( const idVec5& v1, const idVec5& v2, const float l );
};

extern idVec5 vec5_origin;
#define vec5_zero vec5_origin

ID_INLINE idVec5::idVec5()
{
}

ID_INLINE idVec5::idVec5( const idVec3& xyz, const idVec2& st )
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	s = st[0];
	t = st[1];
}

ID_INLINE idVec5::idVec5( const float x, const float y, const float z, const float s, const float t )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->s = s;
	this->t = t;
}

ID_INLINE float idVec5::operator[]( int index ) const
{
	return ( &x )[ index ];
}

ID_INLINE float& idVec5::operator[]( int index )
{
	return ( &x )[ index ];
}

ID_INLINE idVec5& idVec5::operator=( const idVec3& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	s = t = 0;
	return *this;
}

ID_INLINE int idVec5::GetDimension() const
{
	return 5;
}

ID_INLINE const idVec3& idVec5::ToVec3() const
{
	return *reinterpret_cast<const idVec3*>( this );
}

ID_INLINE idVec3& idVec5::ToVec3()
{
	return *reinterpret_cast<idVec3*>( this );
}

ID_INLINE const float* idVec5::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec5::ToFloatPtr()
{
	return &x;
}


//===============================================================
//
//	idVec6 - 6D vector
//
//===============================================================

class idVec6
{
public:
	idVec6();
	explicit idVec6( const float* a );
	explicit idVec6( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );
	
	void 			Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 );
	void			Zero();
	
	float			operator[]( const int index ) const;
	float& 			operator[]( const int index );
	idVec6			operator-() const;
	idVec6			operator*( const float a ) const;
	idVec6			operator/( const float a ) const;
	float			operator*( const idVec6& a ) const;
	idVec6			operator-( const idVec6& a ) const;
	idVec6			operator+( const idVec6& a ) const;
	idVec6& 		operator*=( const float a );
	idVec6& 		operator/=( const float a );
	idVec6& 		operator+=( const idVec6& a );
	idVec6& 		operator-=( const idVec6& a );
	
	friend idVec6	operator*( const float a, const idVec6 b );
	
	bool			Compare( const idVec6& a ) const;							// exact compare, no epsilon
	bool			Compare( const idVec6& a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const idVec6& a ) const;						// exact compare, no epsilon
	bool			operator!=(	const idVec6& a ) const;						// exact compare, no epsilon
	
	float			Length() const;
	float			LengthSqr() const;
	float			Normalize();			// returns length
	float			NormalizeFast();		// returns length
	
	int				GetDimension() const;
	
	const idVec3& 	SubVec3( int index ) const;
	idVec3& 		SubVec3( int index );
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
private:
	float			p[6];
};

extern idVec6 vec6_origin;
#define vec6_zero vec6_origin
extern idVec6 vec6_infinity;

ID_INLINE idVec6::idVec6()
{
}

ID_INLINE idVec6::idVec6( const float* a )
{
	std::memcpy( p, a, 6 * sizeof( float ) );
}

ID_INLINE idVec6::idVec6( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE idVec6 idVec6::operator-() const
{
	return idVec6( -p[0], -p[1], -p[2], -p[3], -p[4], -p[5] );
}

ID_INLINE float idVec6::operator[]( const int index ) const
{
	return p[index];
}

ID_INLINE float& idVec6::operator[]( const int index )
{
	return p[index];
}

ID_INLINE idVec6 idVec6::operator*( const float a ) const
{
	return idVec6( p[0] * a, p[1] * a, p[2] * a, p[3] * a, p[4] * a, p[5] * a );
}

ID_INLINE float idVec6::operator*( const idVec6& a ) const
{
	return p[0] * a[0] + p[1] * a[1] + p[2] * a[2] + p[3] * a[3] + p[4] * a[4] + p[5] * a[5];
}

ID_INLINE idVec6 idVec6::operator/( const float a ) const
{
	float inva;
	
	assert( a != 0.0f );
	inva = 1.0f / a;
	return idVec6( p[0] * inva, p[1] * inva, p[2] * inva, p[3] * inva, p[4] * inva, p[5] * inva );
}

ID_INLINE idVec6 idVec6::operator+( const idVec6& a ) const
{
	return idVec6( p[0] + a[0], p[1] + a[1], p[2] + a[2], p[3] + a[3], p[4] + a[4], p[5] + a[5] );
}

ID_INLINE idVec6 idVec6::operator-( const idVec6& a ) const
{
	return idVec6( p[0] - a[0], p[1] - a[1], p[2] - a[2], p[3] - a[3], p[4] - a[4], p[5] - a[5] );
}

ID_INLINE idVec6& idVec6::operator*=( const float a )
{
	p[0] *= a;
	p[1] *= a;
	p[2] *= a;
	p[3] *= a;
	p[4] *= a;
	p[5] *= a;
	return *this;
}

ID_INLINE idVec6& idVec6::operator/=( const float a )
{
	float inva;
	
	assert( a != 0.0f );
	inva = 1.0f / a;
	p[0] *= inva;
	p[1] *= inva;
	p[2] *= inva;
	p[3] *= inva;
	p[4] *= inva;
	p[5] *= inva;
	return *this;
}

ID_INLINE idVec6& idVec6::operator+=( const idVec6& a )
{
	p[0] += a[0];
	p[1] += a[1];
	p[2] += a[2];
	p[3] += a[3];
	p[4] += a[4];
	p[5] += a[5];
	return *this;
}

ID_INLINE idVec6& idVec6::operator-=( const idVec6& a )
{
	p[0] -= a[0];
	p[1] -= a[1];
	p[2] -= a[2];
	p[3] -= a[3];
	p[4] -= a[4];
	p[5] -= a[5];
	return *this;
}

ID_INLINE idVec6 operator*( const float a, const idVec6 b )
{
	return b * a;
}

ID_INLINE bool idVec6::Compare( const idVec6& a ) const
{
	return ( ( p[0] == a[0] ) && ( p[1] == a[1] ) && ( p[2] == a[2] ) &&
			 ( p[3] == a[3] ) && ( p[4] == a[4] ) && ( p[5] == a[5] ) );
}

ID_INLINE bool idVec6::Compare( const idVec6& a, const float epsilon ) const
{
	if( idMath::Fabs( p[0] - a[0] ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( p[1] - a[1] ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( p[2] - a[2] ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( p[3] - a[3] ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( p[4] - a[4] ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( p[5] - a[5] ) > epsilon )
	{
		return false;
	}
	
	return true;
}

ID_INLINE bool idVec6::operator==( const idVec6& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec6::operator!=( const idVec6& a ) const
{
	return !Compare( a );
}

ID_INLINE void idVec6::Set( const float a1, const float a2, const float a3, const float a4, const float a5, const float a6 )
{
	p[0] = a1;
	p[1] = a2;
	p[2] = a3;
	p[3] = a4;
	p[4] = a5;
	p[5] = a6;
}

ID_INLINE void idVec6::Zero()
{
	p[0] = p[1] = p[2] = p[3] = p[4] = p[5] = 0.0f;
}

ID_INLINE float idVec6::Length() const
{
	return ( float )idMath::Sqrt( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5] );
}

ID_INLINE float idVec6::LengthSqr() const
{
	return ( p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5] );
}

ID_INLINE float idVec6::Normalize()
{
	float sqrLength, invLength;
	
	sqrLength = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5];
	invLength = idMath::InvSqrt( sqrLength );
	p[0] *= invLength;
	p[1] *= invLength;
	p[2] *= invLength;
	p[3] *= invLength;
	p[4] *= invLength;
	p[5] *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec6::NormalizeFast()
{
	float sqrLength, invLength;
	
	sqrLength = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4] + p[5] * p[5];
	invLength = idMath::InvSqrt( sqrLength );
	p[0] *= invLength;
	p[1] *= invLength;
	p[2] *= invLength;
	p[3] *= invLength;
	p[4] *= invLength;
	p[5] *= invLength;
	return invLength * sqrLength;
}

ID_INLINE int idVec6::GetDimension() const
{
	return 6;
}

ID_INLINE const idVec3& idVec6::SubVec3( int index ) const
{
	return *reinterpret_cast<const idVec3*>( p + index * 3 );
}

ID_INLINE idVec3& idVec6::SubVec3( int index )
{
	return *reinterpret_cast<idVec3*>( p + index * 3 );
}

ID_INLINE const float* idVec6::ToFloatPtr() const
{
	return p;
}

ID_INLINE float* idVec6::ToFloatPtr()
{
	return p;
}

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
