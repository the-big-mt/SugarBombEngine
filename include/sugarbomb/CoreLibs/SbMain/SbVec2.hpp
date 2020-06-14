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
//	idVec2 - 2D vector
//
//===============================================================

class idVec2
{
public:
	float			x;
	float			y;
	
	idVec2();
	explicit idVec2( const float x, const float y );
	
	void 			Set( const float x, const float y );
	void			Zero();
	
	float			operator[]( int index ) const;
	float& 			operator[]( int index );
	idVec2			operator-() const;
	float			operator*( const idVec2& a ) const;
	idVec2			operator*( const float a ) const;
	idVec2			operator/( const float a ) const;
	idVec2			operator+( const idVec2& a ) const;
	idVec2			operator-( const idVec2& a ) const;
	idVec2& 		operator+=( const idVec2& a );
	idVec2& 		operator-=( const idVec2& a );
	idVec2& 		operator/=( const idVec2& a );
	idVec2& 		operator/=( const float a );
	idVec2& 		operator*=( const float a );
	
	friend idVec2	operator*( const float a, const idVec2 b );
	
	idVec2			Scale( const idVec2& a ) const;
	
	bool			Compare( const idVec2& a ) const;							// exact compare, no epsilon
	bool			Compare( const idVec2& a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const idVec2& a ) const;						// exact compare, no epsilon
	bool			operator!=(	const idVec2& a ) const;						// exact compare, no epsilon
	
	float			Length() const;
	float			LengthFast() const;
	float			LengthSqr() const;
	float			Normalize();			// returns length
	float			NormalizeFast();		// returns length
	idVec2			Truncate( float length ) const;	// cap length
	void			Clamp( const idVec2& min, const idVec2& max );
	void			Snap();				// snap to closest integer value
	void			SnapInt();			// snap towards integer (floor)
	
	int				GetDimension() const;
	
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
	void			Lerp( const idVec2& v1, const idVec2& v2, const float l );
};

extern idVec2 vec2_origin;
#define vec2_zero vec2_origin

#include "SbVec2.inl"

}; // namespace sbe::SbMain