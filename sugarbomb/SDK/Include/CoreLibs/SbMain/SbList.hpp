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

#include <new>

#include "idlib/sys/sys_defines.h"
#include "idlib/sys/sys_assert.h"
#include "idlib/containers/Sort.h"
#include "idlib/Heap.h" // for memTag_t

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	List template
	Does not allocate memory until the first item is added.

===============================================================================
*/

template< typename _type_, memTag_t _tag_ = TAG_IDLIB_LIST >
class idList
{
public:

	typedef int		cmp_t( const _type_*, const _type_* );
	typedef _type_	new_t();
	
	idList( int newgranularity = 16 );
	idList( const idList& other );
	~idList();
	
	void			Clear();											// clear the list
	int				Num() const;										// returns number of elements in list
	int				NumAllocated() const;								// returns number of elements allocated for
	void			SetGranularity( int newgranularity );				// set new granularity
	int				GetGranularity() const;								// get the current granularity
	
	size_t			Allocated() const;									// returns total size of allocated memory
	size_t			Size() const;										// returns total size of allocated memory including size of list _type_
	size_t			MemoryUsed() const;									// returns size of the used elements in the list
	
	idList<_type_, _tag_>& 		operator=( const idList<_type_, _tag_>& other );
	const _type_& 	operator[]( int index ) const;
	_type_& 		operator[]( int index );
	
	void			Condense();											// resizes list to exactly the number of elements it contains
	void			Resize( int newsize );								// resizes list to the given number of elements
	void			Resize( int newsize, int newgranularity );			// resizes list and sets new granularity
	void			SetNum( int newnum );								// set number of elements in list and resize to exactly this number if needed
	void			AssureSize( int newSize );							// assure list has given number of elements, but leave them uninitialized
	void			AssureSize( int newSize, const _type_ &initValue );	// assure list has given number of elements and initialize any new elements
	void			AssureSizeAlloc( int newSize, new_t* allocator );	// assure the pointer list has the given number of elements and allocate any new elements
	
	_type_* 		Ptr();												// returns a pointer to the list
	const _type_* 	Ptr() const;										// returns a pointer to the list
	_type_& 		Alloc();											// returns reference to a new data element at the end of the list
	int				Append( const _type_ & obj );						// append element
	int				Append( const idList& other );						// append list
	int				AddUnique( const _type_ & obj );					// add unique element
	int				Insert( const _type_ & obj, int index = 0 );		// insert the element at the given index
	int				FindIndex( const _type_ & obj ) const;				// find the index for the given element
	_type_* 		Find( _type_ const& obj ) const;					// find pointer to the given element
	int				FindNull() const;									// find the index for the first NULL pointer in the list
	int				IndexOf( const _type_ *obj ) const;					// returns the index for the pointer to an element in the list
	bool			RemoveIndex( int index );							// remove the element at the given index
	// removes the element at the given index and places the last element into its spot - DOES NOT PRESERVE LIST ORDER
	bool			RemoveIndexFast( int index );
	bool			Remove( const _type_ & obj );						// remove the element
//	void			Sort( cmp_t *compare = ( cmp_t * )&idListSortCompare<_type_, _tag_> );
	void			SortWithTemplate( const idSort<_type_>& sort = idSort_QuickDefault<_type_>() );
//	void			SortSubSection( int startIndex, int endIndex, cmp_t *compare = ( cmp_t * )&idListSortCompare<_type_> );
	void			Swap( idList& other );								// swap the contents of the lists
	void			DeleteContents( bool clear = true );				// delete the contents of the list
	
	//------------------------
	// auto-cast to other idList types with a different memory tag
	//------------------------
	
	template< memTag_t _t_ >
	operator idList<_type_, _t_>& ()
	{
		return *reinterpret_cast<idList<_type_, _t_> *>( this );
	}
	
	template< memTag_t _t_>
	operator const idList<_type_, _t_>& () const
	{
		return *reinterpret_cast<const idList<_type_, _t_> *>( this );
	}
	
	//------------------------
	// memTag
	//
	// Changing the memTag when the list has an allocated buffer will
	// result in corruption of the memory statistics.
	//------------------------
	memTag_t		GetMemTag() const
	{
		return ( memTag_t )memTag;
	};
	void			SetMemTag( memTag_t tag_ )
	{
		memTag = ( byte )tag_;
	};
	
private:
	int				num;
	int				size;
	int				granularity;
	_type_* 		list;
	byte			memTag;
};

#include "SbList.inl"

/*
========================
FindFromGeneric

Finds an item in a list based on any another datatype.  Your _type_ must overload operator()== for the _type_.
If your _type_ is a ptr, use the FindFromGenericPtr function instead.
========================
*/
template< typename _type_, memTag_t _tag_, typename _compare_type_ >
_type_* FindFromGeneric( idList<_type_, _tag_>& list, const _compare_type_ & other )
{
	for( int i = 0; i < list.Num(); i++ )
	{
		if( list[ i ] == other )
		{
			return &list[ i ];
		}
	}
	return NULL;
}

/*
========================
FindFromGenericPtr
========================
*/
template< typename _type_, memTag_t _tag_, typename _compare_type_ >
_type_* FindFromGenericPtr( idList<_type_, _tag_>& list, const _compare_type_ & other )
{
	for( int i = 0; i < list.Num(); i++ )
	{
		if( *list[ i ] == other )
		{
			return &list[ i ];
		}
	}
	return NULL;
}

//} // namespace BFG