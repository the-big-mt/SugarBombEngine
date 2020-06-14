/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

ID_INLINE void idVec4::Set( const float x, const float y, const float z, const float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

ID_INLINE void idVec4::Zero()
{
	x = y = z = w = 0.0f;
}

ID_INLINE float idVec4::operator[]( int index ) const
{
	return ( &x )[ index ];
}

ID_INLINE float& idVec4::operator[]( int index )
{
	return ( &x )[ index ];
}

ID_INLINE idVec4 idVec4::operator-() const
{
	return idVec4( -x, -y, -z, -w );
}

ID_INLINE idVec4 idVec4::operator-( const idVec4& a ) const
{
	return idVec4( x - a.x, y - a.y, z - a.z, w - a.w );
}

ID_INLINE float idVec4::operator*( const idVec4& a ) const
{
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

ID_INLINE idVec4 idVec4::operator*( const float a ) const
{
	return idVec4( x * a, y * a, z * a, w * a );
}

ID_INLINE idVec4 idVec4::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec4( x * inva, y * inva, z * inva, w * inva );
}

ID_INLINE idVec4 operator*( const float a, const idVec4 b )
{
	return idVec4( b.x * a, b.y * a, b.z * a, b.w * a );
}

ID_INLINE idVec4 idVec4::operator+( const idVec4& a ) const
{
	return idVec4( x + a.x, y + a.y, z + a.z, w + a.w );
}

ID_INLINE idVec4& idVec4::operator+=( const idVec4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	
	return *this;
}

ID_INLINE idVec4& idVec4::operator/=( const idVec4& a )
{
	x /= a.x;
	y /= a.y;
	z /= a.z;
	w /= a.w;
	
	return *this;
}

ID_INLINE idVec4& idVec4::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	w *= inva;
	
	return *this;
}

ID_INLINE idVec4& idVec4::operator-=( const idVec4& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	
	return *this;
}

ID_INLINE idVec4& idVec4::operator*=( const float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	
	return *this;
}

ID_INLINE idVec4 idVec4::Multiply( const idVec4& a ) const
{
	return idVec4( x * a.x, y * a.y, z * a.z, w * a.w );
}

ID_INLINE bool idVec4::Compare( const idVec4& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) && w == a.w );
}

ID_INLINE bool idVec4::Compare( const idVec4& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( y - a.y ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( z - a.z ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( w - a.w ) > epsilon )
	{
		return false;
	}
	
	return true;
}

ID_INLINE bool idVec4::operator==( const idVec4& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec4::operator!=( const idVec4& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec4::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y + z * z + w * w );
}

ID_INLINE float idVec4::LengthSqr() const
{
	return ( x * x + y * y + z * z + w * w );
}

ID_INLINE float idVec4::Normalize()
{
	float sqrLength, invLength;
	
	sqrLength = x * x + y * y + z * z + w * w;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec4::NormalizeFast()
{
	float sqrLength, invLength;
	
	sqrLength = x * x + y * y + z * z + w * w;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

ID_INLINE int idVec4::GetDimension() const
{
	return 4;
}

ID_INLINE const idVec2& idVec4::ToVec2() const
{
	return *reinterpret_cast<const idVec2*>( this );
}

ID_INLINE idVec2& idVec4::ToVec2()
{
	return *reinterpret_cast<idVec2*>( this );
}

ID_INLINE const idVec3& idVec4::ToVec3() const
{
	return *reinterpret_cast<const idVec3*>( this );
}

ID_INLINE idVec3& idVec4::ToVec3()
{
	return *reinterpret_cast<idVec3*>( this );
}

ID_INLINE const float* idVec4::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec4::ToFloatPtr()
{
	return &x;
}