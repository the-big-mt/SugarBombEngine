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

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Binary Search templates

	The array elements have to be ordered in increasing order.

===============================================================================
*/

/*
====================
idBinSearch_GreaterEqual

	Finds the last array element which is smaller than the given value.
====================
*/
template< class type >
ID_INLINE int idBinSearch_Less( const type* array, const int arraySize, const type& value )
{
	int len = arraySize;
	int mid = len;
	int offset = 0;
	while( mid > 0 )
	{
		mid = len >> 1;
		if( array[offset + mid] < value )
		{
			offset += mid;
		}
		len -= mid;
	}
	return offset;
}

/*
====================
idBinSearch_GreaterEqual

	Finds the last array element which is smaller than or equal to the given value.
====================
*/
template< class type >
ID_INLINE int idBinSearch_LessEqual( const type* array, const int arraySize, const type& value )
{
	int len = arraySize;
	int mid = len;
	int offset = 0;
	while( mid > 0 )
	{
		mid = len >> 1;
		if( array[offset + mid] <= value )
		{
			offset += mid;
		}
		len -= mid;
	}
	return offset;
}

/*
====================
idBinSearch_Greater

	Finds the first array element which is greater than the given value.
====================
*/
template< class type >
ID_INLINE int idBinSearch_Greater( const type* array, const int arraySize, const type& value )
{
	int len = arraySize;
	int mid = len;
	int offset = 0;
	int res = 0;
	while( mid > 0 )
	{
		mid = len >> 1;
		if( array[offset + mid] > value )
		{
			res = 0;
		}
		else
		{
			offset += mid;
			res = 1;
		}
		len -= mid;
	}
	return offset + res;
}

/*
====================
idBinSearch_GreaterEqual

	Finds the first array element which is greater than or equal to the given value.
====================
*/
template< class type >
ID_INLINE int idBinSearch_GreaterEqual( const type* array, const int arraySize, const type& value )
{
	int len = arraySize;
	int mid = len;
	int offset = 0;
	int res = 0;
	while( mid > 0 )
	{
		mid = len >> 1;
		if( array[offset + mid] >= value )
		{
			res = 0;
		}
		else
		{
			offset += mid;
			res = 1;
		}
		len -= mid;
	}
	return offset + res;
}

//} // namespace BFG