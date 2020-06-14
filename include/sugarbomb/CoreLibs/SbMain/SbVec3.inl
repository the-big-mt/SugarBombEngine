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

ID_INLINE idVec3::idVec3()
{
}

ID_INLINE idVec3::idVec3( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE float idVec3::operator[]( const int index ) const
{
	return ( &x )[ index ];
}

ID_INLINE float& idVec3::operator[]( const int index )
{
	return ( &x )[ index ];
}

ID_INLINE void idVec3::Set( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ID_INLINE void idVec3::Zero()
{
	x = y = z = 0.0f;
}

ID_INLINE idVec3 idVec3::operator-() const
{
	return idVec3( -x, -y, -z );
}

ID_INLINE idVec3& idVec3::operator=( const idVec3& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

ID_INLINE idVec3 idVec3::operator-( const idVec3& a ) const
{
	return idVec3( x - a.x, y - a.y, z - a.z );
}

ID_INLINE float idVec3::operator*( const idVec3& a ) const
{
	return x * a.x + y * a.y + z * a.z;
}

ID_INLINE idVec3 idVec3::operator*( const float a ) const
{
	return idVec3( x * a, y * a, z * a );
}

ID_INLINE idVec3 idVec3::operator/( const float a ) const
{
	float inva = 1.0f / a;
	return idVec3( x * inva, y * inva, z * inva );
}

ID_INLINE idVec3 operator*( const float a, const idVec3 b )
{
	return idVec3( b.x * a, b.y * a, b.z * a );
}

ID_INLINE idVec3 operator/( const float a, const idVec3 b )
{
	return idVec3( a / b.x, a / b.y, a / b.z );
}

ID_INLINE idVec3 idVec3::operator+( const idVec3& a ) const
{
	return idVec3( x + a.x, y + a.y, z + a.z );
}

ID_INLINE idVec3& idVec3::operator+=( const idVec3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	
	return *this;
}

ID_INLINE idVec3& idVec3::operator/=( const idVec3& a )
{
	x /= a.x;
	y /= a.y;
	z /= a.z;
	
	return *this;
}

ID_INLINE idVec3& idVec3::operator/=( const float a )
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	
	return *this;
}

ID_INLINE idVec3& idVec3::operator-=( const idVec3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	
	return *this;
}

ID_INLINE idVec3& idVec3::operator*=( const float a )
{
	x *= a;
	y *= a;
	z *= a;
	
	return *this;
}

ID_INLINE bool idVec3::Compare( const idVec3& a ) const
{
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) );
}

ID_INLINE bool idVec3::Compare( const idVec3& a, const float epsilon ) const
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
	
	return true;
}

ID_INLINE bool idVec3::operator==( const idVec3& a ) const
{
	return Compare( a );
}

ID_INLINE bool idVec3::operator!=( const idVec3& a ) const
{
	return !Compare( a );
}

ID_INLINE float idVec3::NormalizeFast()
{
	float sqrLength, invLength;
	
	sqrLength = x * x + y * y + z * z;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

ID_INLINE bool idVec3::FixDegenerateNormal()
{
	if( x == 0.0f )
	{
		if( y == 0.0f )
		{
			if( z > 0.0f )
			{
				if( z != 1.0f )
				{
					z = 1.0f;
					return true;
				}
			}
			else
			{
				if( z != -1.0f )
				{
					z = -1.0f;
					return true;
				}
			}
			return false;
		}
		else if( z == 0.0f )
		{
			if( y > 0.0f )
			{
				if( y != 1.0f )
				{
					y = 1.0f;
					return true;
				}
			}
			else
			{
				if( y != -1.0f )
				{
					y = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	else if( y == 0.0f )
	{
		if( z == 0.0f )
		{
			if( x > 0.0f )
			{
				if( x != 1.0f )
				{
					x = 1.0f;
					return true;
				}
			}
			else
			{
				if( x != -1.0f )
				{
					x = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	if( idMath::Fabs( x ) == 1.0f )
	{
		if( y != 0.0f || z != 0.0f )
		{
			y = z = 0.0f;
			return true;
		}
		return false;
	}
	else if( idMath::Fabs( y ) == 1.0f )
	{
		if( x != 0.0f || z != 0.0f )
		{
			x = z = 0.0f;
			return true;
		}
		return false;
	}
	else if( idMath::Fabs( z ) == 1.0f )
	{
		if( x != 0.0f || y != 0.0f )
		{
			x = y = 0.0f;
			return true;
		}
		return false;
	}
	return false;
}

ID_INLINE bool idVec3::FixDenormals()
{
	bool denormal = false;
	if( fabs( x ) < 1e-30f )
	{
		x = 0.0f;
		denormal = true;
	}
	if( fabs( y ) < 1e-30f )
	{
		y = 0.0f;
		denormal = true;
	}
	if( fabs( z ) < 1e-30f )
	{
		z = 0.0f;
		denormal = true;
	}
	return denormal;
}

ID_INLINE idVec3 idVec3::Cross( const idVec3& a ) const
{
	return idVec3( y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x );
}

ID_INLINE idVec3& idVec3::Cross( const idVec3& a, const idVec3& b )
{
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;
	
	return *this;
}

ID_INLINE float idVec3::Length() const
{
	return ( float )idMath::Sqrt( x * x + y * y + z * z );
}

ID_INLINE float idVec3::LengthSqr() const
{
	return ( x * x + y * y + z * z );
}

ID_INLINE float idVec3::LengthFast() const
{
	float sqrLength;
	
	sqrLength = x * x + y * y + z * z;
	return sqrLength * idMath::InvSqrt( sqrLength );
}

ID_INLINE float idVec3::Normalize()
{
	float sqrLength, invLength;
	
	sqrLength = x * x + y * y + z * z;
	invLength = idMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

ID_INLINE idVec3 idVec3::Truncate( float length ) const
{
	if( length < idMath::FLT_SMALLEST_NON_DENORMAL )
	{
		return vec3_zero;
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

ID_INLINE void idVec3::Clamp( const idVec3& min, const idVec3& max )
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
	if( z < min.z )
	{
		z = min.z;
	}
	else if( z > max.z )
	{
		z = max.z;
	}
}

ID_INLINE void idVec3::Snap()
{
	x = floor( x + 0.5f );
	y = floor( y + 0.5f );
	z = floor( z + 0.5f );
}

ID_INLINE void idVec3::SnapInt()
{
	x = float( int( x ) );
	y = float( int( y ) );
	z = float( int( z ) );
}

ID_INLINE int idVec3::GetDimension() const
{
	return 3;
}

ID_INLINE const SbVec2& SbVec3::ToVec2() const
{
	return *reinterpret_cast<const SbVec2*>( this );
}

ID_INLINE SbVec2& SbVec3::ToVec2()
{
	return *reinterpret_cast<SbVec2*>( this );
}

ID_INLINE const float* idVec3::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec3::ToFloatPtr()
{
	return &x;
}

ID_INLINE void idVec3::NormalVectors( idVec3& left, idVec3& down ) const
{
	float d;
	
	d = x * x + y * y;
	if( !d )
	{
		left[0] = 1;
		left[1] = 0;
		left[2] = 0;
	}
	else
	{
		d = idMath::InvSqrt( d );
		left[0] = -y * d;
		left[1] = x * d;
		left[2] = 0;
	}
	down = left.Cross( *this );
}

ID_INLINE void idVec3::OrthogonalBasis( idVec3& left, idVec3& up ) const
{
	float l, s;
	
	if( idMath::Fabs( z ) > 0.7f )
	{
		l = y * y + z * z;
		s = idMath::InvSqrt( l );
		up[0] = 0;
		up[1] = z * s;
		up[2] = -y * s;
		left[0] = l * s;
		left[1] = -x * up[2];
		left[2] = x * up[1];
	}
	else
	{
		l = x * x + y * y;
		s = idMath::InvSqrt( l );
		left[0] = -y * s;
		left[1] = x * s;
		left[2] = 0;
		up[0] = -z * left[1];
		up[1] = z * left[0];
		up[2] = l * s;
	}
}

ID_INLINE void idVec3::ProjectOntoPlane( const idVec3& normal, const float overBounce )
{
	float backoff;
	
	backoff = *this * normal;
	
	if( overBounce != 1.0 )
	{
		if( backoff < 0 )
		{
			backoff *= overBounce;
		}
		else
		{
			backoff /= overBounce;
		}
	}
	
	*this -= backoff * normal;
}

ID_INLINE bool idVec3::ProjectAlongPlane( const idVec3& normal, const float epsilon, const float overBounce )
{
	idVec3 cross;
	float len;
	
	cross = this->Cross( normal ).Cross( ( *this ) );
	// normalize so a fixed epsilon can be used
	cross.Normalize();
	len = normal * cross;
	if( idMath::Fabs( len ) < epsilon )
	{
		return false;
	}
	cross *= overBounce * ( normal * ( *this ) ) / len;
	( *this ) -= cross;
	return true;
}