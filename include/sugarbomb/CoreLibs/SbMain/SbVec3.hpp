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
//	idVec3 - 3D vector
//
//===============================================================

class idVec3
{
public:
	float			x;
	float			y;
	float			z;
	
	idVec3();
	explicit idVec3( const float xyz )
	{
		Set( xyz, xyz, xyz );
	}
	explicit idVec3( const float x, const float y, const float z );
	
	void 			Set( const float x, const float y, const float z );
	void			Zero();
	
	float			operator[]( const int index ) const;
	float& 			operator[]( const int index );
	idVec3			operator-() const;
	idVec3& 		operator=( const idVec3& a );		// required because of a msvc 6 & 7 bug
	float			operator*( const idVec3& a ) const;
	idVec3			operator*( const float a ) const;
	idVec3			operator/( const float a ) const;
	idVec3			operator+( const idVec3& a ) const;
	idVec3			operator-( const idVec3& a ) const;
	idVec3& 		operator+=( const idVec3& a );
	idVec3& 		operator-=( const idVec3& a );
	idVec3& 		operator/=( const idVec3& a );
	idVec3& 		operator/=( const float a );
	idVec3& 		operator*=( const float a );
	
	friend idVec3	operator*( const float a, const idVec3 b );
	
	bool			Compare( const idVec3& a ) const;							// exact compare, no epsilon
	bool			Compare( const idVec3& a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const idVec3& a ) const;						// exact compare, no epsilon
	bool			operator!=(	const idVec3& a ) const;						// exact compare, no epsilon
	
	bool			FixDegenerateNormal();	// fix degenerate axial cases
	bool			FixDenormals();			// change tiny numbers to zero
	
	idVec3			Cross( const idVec3& a ) const;
	idVec3& 		Cross( const idVec3& a, const idVec3& b );
	float			Length() const;
	float			LengthSqr() const;
	float			LengthFast() const;
	float			Normalize();				// returns length
	float			NormalizeFast();			// returns length
	idVec3			Truncate( float length ) const;		// cap length
	void			Clamp( const idVec3& min, const idVec3& max );
	void			Snap();					// snap to closest integer value
	void			SnapInt();				// snap towards integer (floor)
	
	int				GetDimension() const;
	
	float			ToYaw() const;
	float			ToPitch() const;
	idAngles		ToAngles() const;
	idPolar3		ToPolar() const;
	idMat3			ToMat3() const;		// vector should be normalized
	const idVec2& 	ToVec2() const;
	idVec2& 		ToVec2();
	const float* 	ToFloatPtr() const;
	float* 			ToFloatPtr();
	const char* 	ToString( int precision = 2 ) const;
	
	void			NormalVectors( idVec3& left, idVec3& down ) const;	// vector should be normalized
	void			OrthogonalBasis( idVec3& left, idVec3& up ) const;
	
	void			ProjectOntoPlane( const idVec3& normal, const float overBounce = 1.0f );
	bool			ProjectAlongPlane( const idVec3& normal, const float epsilon, const float overBounce = 1.0f );
	void			ProjectSelfOntoSphere( const float radius );
	
	void			Lerp( const idVec3& v1, const idVec3& v2, const float l );
	void			SLerp( const idVec3& v1, const idVec3& v2, const float l );
};

extern idVec3 vec3_origin;
#define vec3_zero vec3_origin

#include "SbVec3.inl"

}; // namespace sbe::SbMain