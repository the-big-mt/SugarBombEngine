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

#include "SbVec5.inl"

}; // namespace sbe::SbMain