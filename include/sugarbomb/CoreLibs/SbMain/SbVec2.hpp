/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

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
//	SbVec2 - 2D vector
//
//===============================================================

class SbVec2
{
public:
	float			x;
	float			y;
	
	SbVec2();
	explicit SbVec2( const float x, const float y );
	
	void 			Set( const float x, const float y );
	void			Zero();
	
	float			operator[]( int index ) const;
	float& 			operator[]( int index );
	SbVec2			operator-() const;
	float			operator*( const SbVec2& a ) const;
	SbVec2			operator*( const float a ) const;
	SbVec2			operator/( const float a ) const;
	SbVec2			operator+( const SbVec2& a ) const;
	SbVec2			operator-( const SbVec2& a ) const;
	SbVec2& 		operator+=( const SbVec2& a );
	SbVec2& 		operator-=( const SbVec2& a );
	SbVec2& 		operator/=( const SbVec2& a );
	SbVec2& 		operator/=( const float a );
	SbVec2& 		operator*=( const float a );
	
	friend SbVec2	operator*( const float a, const SbVec2 b );
	
	SbVec2			Scale( const SbVec2& a ) const;
	
	bool			Compare( const SbVec2& a ) const;							// exact compare, no epsilon
	bool			Compare( const SbVec2& a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const SbVec2& a ) const;						// exact compare, no epsilon
	bool			operator!=(	const SbVec2& a ) const;						// exact compare, no epsilon
	
	float			Length() const;
	float			LengthFast() const;
	float			LengthSqr() const;
	float			Normalize();			// returns length
	float			NormalizeFast();		// returns length
	SbVec2			Truncate( float length ) const;	// cap length
	void			Clamp( const SbVec2& min, const SbVec2& max );
	void			Snap();				// snap to closest integer value
	void			SnapInt();			// snap towards integer (floor)
	
	int				GetDimension() const;
	
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
	void			Lerp( const SbVec2& v1, const SbVec2& v2, const float l );
};

extern SbVec2 vec2_origin;
#define vec2_zero vec2_origin

#include "SbVec2.inl"

}; // namespace sbe::SbMain