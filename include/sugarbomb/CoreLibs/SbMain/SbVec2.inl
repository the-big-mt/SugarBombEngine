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

ID_INLINE idVec2::idVec2()
{
}

ID_INLINE idVec2::idVec2( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

ID_INLINE void idVec2::Set( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

ID_INLINE void idVec2::Zero()
{
	x = y = 0.0f;
}

ID_INLINE bool idVec2::Compare( const idVec2& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) );
}

ID_INLINE bool idVec2::Compare( const idVec2& a, const float epsilon ) const
{
	if( idMath::Fabs( x - a.x ) > epsilon )
	{
		return false;
	}
	
	if( idMath::Fabs( y - a.y ) > epsilon )
	{
		return false;
	}
	
	return true;
}

ID_INLINE bool idVec2::operator==( const idVec2& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec2::operator!=( const idVec2& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec2::operator[]( int index ) const
{
	return ( &x )[ index ];
}

ID_INLINE float& idVec2::operator[]( int index )
{
	return ( &x )[ index ];
}

ID_INLINE float idVec2::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y );
}

ID_INLINE float idVec2::LengthFast() const
{
	float sqrLength;
	
	sqrLength = x * x + y * y;
	return sqrLength * idMath::InvSqrt( sqrLength );
}

ID_INLINE float idVec2::LengthSqr() const
{
	return ( x * x + y * y );
}

ID_INLINE float idVec2::Normalize()
{
	float sqrLength, invLength;
	
	sqrLength = x * x + y * y;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	return invLength * sqrLength;
}

ID_INLINE float idVec2::NormalizeFast()
{
	float lengthSqr, invLength;
	
	lengthSqr = x * x + y * y;
	invLength = idMath::InvSqrt( lengthSqr );
	x *= invLength;
	y *= invLength;
	return invLength * lengthSqr;
}

ID_INLINE idVec2 idVec2::Truncate( float length ) const
{
	if( length < idMath::FLT_SMALLEST_NON_DENORMAL )
	{
		return vec2_zero;
	}
	else
	{
		float length2 = LengthSqr();
		if( length2 > length * length )
		{
			float ilength = length * idMath::InvSqrt( length2 );
			return *this * ilength;
		}
	}
	return *this;
}

ID_INLINE void idVec2::Clamp( const idVec2& min, const idVec2& max )
{
	if( x < min.x )
	{
		x = min.x;
	}
	else if( x > max.x )
	{
		x = max.x;
	}
	if( y < min.y )
	{
		y = min.y;
	}
	else if( y > max.y )
	{
		y = max.y;
	}
}

ID_INLINE void idVec2::Snap()
{
	x = floor( x + 0.5f );
	y = floor( y + 0.5f );
}

ID_INLINE void idVec2::SnapInt()
{
	x = float( int( x ) );
	y = float( int( y ) );
}

ID_INLINE idVec2 idVec2::operator-() const
{
	return idVec2( -x, -y );
}

ID_INLINE idVec2 idVec2::operator-( const idVec2& a ) const
{
	return idVec2( x - a.x, y - a.y );
}

ID_INLINE float idVec2::operator*( const idVec2& a ) const
{
	return x * a.x + y * a.y;
}

ID_INLINE idVec2 idVec2::operator*( const float a ) const
{
	return idVec2( x * a, y * a );
}

ID_INLINE idVec2 idVec2::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec2( x * inva, y * inva );
}

ID_INLINE idVec2 operator*( const float a, const idVec2 b )
{
	return idVec2( b.x * a, b.y * a );
}

ID_INLINE idVec2 idVec2::operator+( const idVec2& a ) const
{
	return idVec2( x + a.x, y + a.y );
}

ID_INLINE idVec2& idVec2::operator+=( const idVec2& a )
{
	x += a.x;
	y += a.y;
	
	return *this;
}

ID_INLINE idVec2& idVec2::operator/=( const idVec2& a )
{
	x /= a.x;
	y /= a.y;
	
	return *this;
}

ID_INLINE idVec2& idVec2::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	
	return *this;
}

ID_INLINE idVec2& idVec2::operator-=( const idVec2& a )
{
	x -= a.x;
	y -= a.y;
	
	return *this;
}

ID_INLINE idVec2& idVec2::operator*=( const float a )
{
	x *= a;
	y *= a;
	
	return *this;
}

ID_INLINE idVec2 idVec2::Scale( const idVec2& a ) const
{
	return idVec2( x * a.x, y * a.y );
}

ID_INLINE int idVec2::GetDimension() const
{
	return 2;
}

ID_INLINE const float* idVec2::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec2::ToFloatPtr()
{
	return &x;
}