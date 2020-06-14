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

ID_INLINE idVec5::idVec5()
{
}

ID_INLINE idVec5::idVec5( const idVec3& xyz, const idVec2& st )
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	s = st[0];
	t = st[1];
}

ID_INLINE idVec5::idVec5( const float x, const float y, const float z, const float s, const float t )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->s = s;
	this->t = t;
}

ID_INLINE float idVec5::operator[]( int index ) const
{
	return ( &x )[ index ];
}

ID_INLINE float& idVec5::operator[]( int index )
{
	return ( &x )[ index ];
}

ID_INLINE idVec5& idVec5::operator=( const idVec3& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	s = t = 0;
	return *this;
}

ID_INLINE int idVec5::GetDimension() const
{
	return 5;
}

ID_INLINE const idVec3& idVec5::ToVec3() const
{
	return *reinterpret_cast<const idVec3*>( this );
}

ID_INLINE idVec3& idVec5::ToVec3()
{
	return *reinterpret_cast<idVec3*>( this );
}

ID_INLINE const float* idVec5::ToFloatPtr() const
{
	return &x;
}

ID_INLINE float* idVec5::ToFloatPtr()
{
	return &x;
}