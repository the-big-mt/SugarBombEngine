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
	
	const SbVec3& 	SubVec3( int index ) const;
	SbVec3& 		SubVec3( int index );
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
private:
	float			p[6];
};

extern idVec6 vec6_origin;
#define vec6_zero vec6_origin
extern idVec6 vec6_infinity;

#include "SbVec6.inl"

}; // namespace sbe::SbMain