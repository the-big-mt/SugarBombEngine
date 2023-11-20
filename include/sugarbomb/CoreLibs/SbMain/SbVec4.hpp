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

namespace sbe::SbMain
{

//===============================================================
//
//	SbVec4 - 4D vector
//
//===============================================================

class SbVec4
{
public:
	float			x;
	float			y;
	float			z;
	float			w;
	
	SbVec4() { }
	explicit SbVec4( const float x )
	{
		Set( x, x, x, x );
	}
	explicit SbVec4( const float x, const float y, const float z, const float w )
	{
		Set( x, y, z, w );
	}
	
	void 			Set( const float x, const float y, const float z, const float w );
	void			Zero();
	
	float			operator[]( const int index ) const;
	float& 			operator[]( const int index );
	SbVec4			operator-() const;
	float			operator*( const SbVec4& a ) const;
	SbVec4			operator*( const float a ) const;
	SbVec4			operator/( const float a ) const;
	SbVec4			operator+( const SbVec4& a ) const;
	SbVec4			operator-( const SbVec4& a ) const;
	SbVec4& 		operator+=( const SbVec4& a );
	SbVec4& 		operator-=( const SbVec4& a );
	SbVec4& 		operator/=( const SbVec4& a );
	SbVec4& 		operator/=( const float a );
	SbVec4& 		operator*=( const float a );
	
	friend SbVec4	operator*( const float a, const SbVec4 b );
	
	SbVec4			Multiply( const SbVec4& a ) const;
	
	bool			Compare( const SbVec4& a ) const;							// exact compare, no epsilon
	bool			Compare( const SbVec4& a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const SbVec4& a ) const;						// exact compare, no epsilon
	bool			operator!=(	const SbVec4& a ) const;						// exact compare, no epsilon
	
	float			Length() const;
	float			LengthSqr() const;
	float			Normalize();			// returns length
	float			NormalizeFast();		// returns length
	
	int				GetDimension() const;
	
	const SbVec2& 	ToVec2() const;
	SbVec2& 		ToVec2();
	const SbVec3& 	ToVec3() const;
	SbVec3& 		ToVec3();
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
	void			Lerp( const SbVec4& v1, const SbVec4& v2, const float l );
};

extern SbVec4 vec4_origin;
#define vec4_zero vec4_origin

#include "SbVec4.inl"

}; // namespace sbe::SbMain