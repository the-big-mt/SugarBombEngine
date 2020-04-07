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

//#include "../idlib/sys/sys_defines.h"
//#include "../idlib/sys/sys_assert.h"

//*****************************************************************************

//namespace BFG
//{

static ID_INLINE int MinInt( int a, int b )
{
	return ( a ) < ( b ) ? ( a ) : ( b );
}
static ID_INLINE int MaxInt( int a, int b )
{
	return ( a ) < ( b ) ? ( b ) : ( a );
}

class idVec2i
{
public:
	int      x, y;
	
	idVec2i() {}
	idVec2i( int _x, int _y ) : x( _x ), y( _y ) {}
	
	void		Set( int _x, int _y )
	{
		x = _x;
		y = _y;
	}
	int			Area() const
	{
		return x * y;
	};
	
	void		Min( idVec2i& v )
	{
		x = MinInt( x, v.x );
		y = MinInt( y, v.y );
	}
	void		Max( idVec2i& v )
	{
		x = MaxInt( x, v.x );
		y = MaxInt( y, v.y );
	}
	
	int			operator[]( const int index ) const
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}
	int& 		operator[]( const int index )
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}
	
	idVec2i 	operator-() const
	{
		return idVec2i( -x, -y );
	}
	idVec2i 	operator!() const
	{
		return idVec2i( !x, !y );
	}
	
	idVec2i 	operator>>( const int a ) const
	{
		return idVec2i( x >> a, y >> a );
	}
	idVec2i 	operator<<( const int a ) const
	{
		return idVec2i( x << a, y << a );
	}
	idVec2i 	operator&( const int a ) const
	{
		return idVec2i( x & a, y & a );
	}
	idVec2i 	operator|( const int a ) const
	{
		return idVec2i( x | a, y | a );
	}
	idVec2i 	operator^( const int a ) const
	{
		return idVec2i( x ^ a, y ^ a );
	}
	idVec2i 	operator*( const int a ) const
	{
		return idVec2i( x * a, y * a );
	}
	idVec2i 	operator/( const int a ) const
	{
		return idVec2i( x / a, y / a );
	}
	idVec2i 	operator+( const int a ) const
	{
		return idVec2i( x + a, y + a );
	}
	idVec2i 	operator-( const int a ) const
	{
		return idVec2i( x - a, y - a );
	}
	
	bool		operator==( const idVec2i& a ) const
	{
		return a.x == x && a.y == y;
	};
	bool		operator!=( const idVec2i& a ) const
	{
		return a.x != x || a.y != y;
	};
	
	idVec2i		operator>>( const idVec2i& a ) const
	{
		return idVec2i( x >> a.x, y >> a.y );
	}
	idVec2i		operator<<( const idVec2i& a ) const
	{
		return idVec2i( x << a.x, y << a.y );
	}
	idVec2i		operator&( const idVec2i& a ) const
	{
		return idVec2i( x & a.x, y & a.y );
	}
	idVec2i		operator|( const idVec2i& a ) const
	{
		return idVec2i( x | a.x, y | a.y );
	}
	idVec2i		operator^( const idVec2i& a ) const
	{
		return idVec2i( x ^ a.x, y ^ a.y );
	}
	idVec2i		operator*( const idVec2i& a ) const
	{
		return idVec2i( x * a.x, y * a.y );
	}
	idVec2i		operator/( const idVec2i& a ) const
	{
		return idVec2i( x / a.x, y / a.y );
	}
	idVec2i		operator+( const idVec2i& a ) const
	{
		return idVec2i( x + a.x, y + a.y );
	}
	idVec2i		operator-( const idVec2i& a ) const
	{
		return idVec2i( x - a.x, y - a.y );
	}
	
	idVec2i& 	operator+=( const int a )
	{
		x += a;
		y += a;
		return *this;
	}
	idVec2i& 	operator-=( const int a )
	{
		x -= a;
		y -= a;
		return *this;
	}
	idVec2i& 	operator/=( const int a )
	{
		x /= a;
		y /= a;
		return *this;
	}
	idVec2i& 	operator*=( const int a )
	{
		x *= a;
		y *= a;
		return *this;
	}
	idVec2i& 	operator>>=( const int a )
	{
		x >>= a;
		y >>= a;
		return *this;
	}
	idVec2i& 	operator<<=( const int a )
	{
		x <<= a;
		y <<= a;
		return *this;
	}
	idVec2i& 	operator&=( const int a )
	{
		x &= a;
		y &= a;
		return *this;
	}
	idVec2i& 	operator|=( const int a )
	{
		x |= a;
		y |= a;
		return *this;
	}
	idVec2i& 	operator^=( const int a )
	{
		x ^= a;
		y ^= a;
		return *this;
	}
	
	idVec2i& 	operator>>=( const idVec2i& a )
	{
		x >>= a.x;
		y >>= a.y;
		return *this;
	}
	idVec2i& 	operator<<=( const idVec2i& a )
	{
		x <<= a.x;
		y <<= a.y;
		return *this;
	}
	idVec2i& 	operator&=( const idVec2i& a )
	{
		x &= a.x;
		y &= a.y;
		return *this;
	}
	idVec2i& 	operator|=( const idVec2i& a )
	{
		x |= a.x;
		y |= a.y;
		return *this;
	}
	idVec2i& 	operator^=( const idVec2i& a )
	{
		x ^= a.x;
		y ^= a.y;
		return *this;
	}
	idVec2i& 	operator+=( const idVec2i& a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}
	idVec2i& 	operator-=( const idVec2i& a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
	idVec2i& 	operator/=( const idVec2i& a )
	{
		x /= a.x;
		y /= a.y;
		return *this;
	}
	idVec2i& 	operator*=( const idVec2i& a )
	{
		x *= a.x;
		y *= a.y;
		return *this;
	}
};

//} // namespace BFG