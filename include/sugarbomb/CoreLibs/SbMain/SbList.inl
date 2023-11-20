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

#pragma once

/*
========================
idListArrayNew
========================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void* idListArrayNew( int num, bool zeroBuffer )
{
	_type_ * ptr = NULL;
	if( zeroBuffer )
	{
		ptr = ( _type_* )Mem_ClearedAlloc( sizeof( _type_ ) * num, _tag_ );
	}
	else
	{
		ptr = ( _type_* )Mem_Alloc( sizeof( _type_ ) * num, _tag_ );
	}
	for( int i = 0; i < num; i++ )
	{
		new( &ptr[i] ) _type_;
	}
	return ptr;
}

/*
========================
idListArrayDelete
========================
*/
template< typename _type_ >
ID_INLINE void idListArrayDelete( void* ptr, int num )
{
	// Call the destructors on all the elements
	for( int i = 0; i < num; i++ )
	{
		( ( _type_* )ptr )[i].~_type_();
	}
	Mem_Free( ptr );
}

/*
========================
idListArrayResize
========================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void* idListArrayResize( void* voldptr, int oldNum, int newNum, bool zeroBuffer )
{
	_type_ * oldptr = ( _type_* )voldptr;
	_type_ * newptr = NULL;
	if( newNum > 0 )
	{
		newptr = ( _type_* )idListArrayNew<_type_, _tag_>( newNum, zeroBuffer );
		int overlap = Min( oldNum, newNum );
		for( int i = 0; i < overlap; i++ )
		{
			newptr[i] = oldptr[i];
		}
	}
	idListArrayDelete<_type_>( voldptr, oldNum );
	return newptr;
}

/*
================
idListNewElement<type>
================
*/
template< class type >
ID_INLINE type* idListNewElement()
{
	return new type;
}

/*
================
idList<_type_,_tag_>::idList( int )
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE SbList<_type_, _tag_>::SbList( int newgranularity )
{
	assert( newgranularity > 0 );
	
	list		= NULL;
	granularity	= newgranularity;
	memTag		= _tag_;
	Clear();
}

/*
================
idList<_type_,_tag_>::idList( const idList< _type_, _tag_ > &other )
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE SbList<_type_, _tag_>::SbList( const SbList& other )
{
	list = NULL;
	*this = other;
}

/*
================
idList<_type_,_tag_>::~idList< _type_, _tag_ >
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE SbList<_type_, _tag_>::~SbList()
{
	Clear();
}

/*
================
idList<_type_,_tag_>::Clear

Frees up the memory allocated by the list.  Assumes that _type_ automatically handles freeing up memory.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::Clear()
{
	if( list )
	{
		idListArrayDelete< _type_ >( list, size );
	}
	
	list	= NULL;
	num		= 0;
	size	= 0;
}

/*
================
idList<_type_,_tag_>::DeleteContents

Calls the destructor of all elements in the list.  Conditionally frees up memory used by the list.
Note that this only works on lists containing pointers to objects and will cause a compiler error
if called with non-pointers.  Since the list was not responsible for allocating the object, it has
no information on whether the object still exists or not, so care must be taken to ensure that
the pointers are still valid when this function is called.  Function will set all pointers in the
list to NULL.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::DeleteContents( bool clear )
{
	int i;
	
	for( i = 0; i < num; i++ )
	{
		delete list[ i ];
		list[ i ] = NULL;
	}
	
	if( clear )
	{
		Clear();
	}
	else
	{
		memset( list, 0, size * sizeof( _type_ ) );
	}
}

/*
================
idList<_type_,_tag_>::Allocated

return total memory allocated for the list in bytes, but doesn't take into account additional memory allocated by _type_
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE size_t SbList<_type_, _tag_>::Allocated() const
{
	return size * sizeof( _type_ );
}

/*
================
idList<_type_,_tag_>::Size

return total size of list in bytes, but doesn't take into account additional memory allocated by _type_
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE size_t SbList<_type_, _tag_>::Size() const
{
	return sizeof( SbList< _type_, _tag_ > ) + Allocated();
}

/*
================
idList<_type_,_tag_>::MemoryUsed
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE size_t SbList<_type_, _tag_>::MemoryUsed() const
{
	return num * sizeof( *list );
}

/*
================
idList<_type_,_tag_>::Num

Returns the number of elements currently contained in the list.
Note that this is NOT an indication of the memory allocated.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::Num() const
{
	return num;
}

/*
================
idList<_type_,_tag_>::NumAllocated

Returns the number of elements currently allocated for.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::NumAllocated() const
{
	return size;
}

/*
================
idList<_type_,_tag_>::SetNum
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::SetNum( int newnum )
{
	assert( newnum >= 0 );
	if( newnum > size )
	{
		Resize( newnum );
	}
	num = newnum;
}

/*
================
idList<_type_,_tag_>::SetGranularity

Sets the base size of the array and resizes the array to match.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::SetGranularity( int newgranularity )
{
	int newsize;
	
	assert( newgranularity > 0 );
	granularity = newgranularity;
	
	if( list )
	{
		// resize it to the closest level of granularity
		newsize = num + granularity - 1;
		newsize -= newsize % granularity;
		if( newsize != size )
		{
			Resize( newsize );
		}
	}
}

/*
================
idList<_type_,_tag_>::GetGranularity

Get the current granularity.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::GetGranularity() const
{
	return granularity;
}

/*
================
idList<_type_,_tag_>::Condense

Resizes the array to exactly the number of elements it contains or frees up memory if empty.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::Condense()
{
	if( list )
	{
		if( num )
		{
			Resize( num );
		}
		else
		{
			Clear();
		}
	}
}

/*
================
idList<_type_,_tag_>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::Resize( int newsize )
{
	assert( newsize >= 0 );
	
	// free up the list if no data is being reserved
	if( newsize <= 0 )
	{
		Clear();
		return;
	}
	
	if( newsize == size )
	{
		// not changing the size, so just exit
		return;
	}
	
	list = ( _type_* )idListArrayResize< _type_, _tag_ >( list, size, newsize, false );
	size = newsize;
	if( size < num )
	{
		num = size;
	}
}

/*
================
idList<_type_,_tag_>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::Resize( int newsize, int newgranularity )
{
	assert( newsize >= 0 );
	
	assert( newgranularity > 0 );
	granularity = newgranularity;
	
	// free up the list if no data is being reserved
	if( newsize <= 0 )
	{
		Clear();
		return;
	}
	
	list = ( _type_* )idListArrayResize< _type_, _tag_ >( list, size, newsize, false );
	size = newsize;
	if( size < num )
	{
		num = size;
	}
}

/*
================
idList<_type_,_tag_>::AssureSize

Makes sure the list has at least the given number of elements.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::AssureSize( int newSize )
{
	int newNum = newSize;
	
	if( newSize > size )
	{
	
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		Resize( newSize );
	}
	
	num = newNum;
}

/*
================
idList<_type_,_tag_>::AssureSize

Makes sure the list has at least the given number of elements and initialize any elements not yet initialized.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::AssureSize( int newSize, const _type_ &initValue )
{
	int newNum = newSize;
	
	if( newSize > size )
	{
	
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );
		
		for( int i = num; i < newSize; i++ )
		{
			list[i] = initValue;
		}
	}
	
	num = newNum;
}

/*
================
idList<_type_,_tag_>::AssureSizeAlloc

Makes sure the list has at least the given number of elements and allocates any elements using the allocator.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::AssureSizeAlloc( int newSize, new_t* allocator )
{
	int newNum = newSize;
	
	if( newSize > size )
	{
	
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );
		
		for( int i = num; i < newSize; i++ )
		{
			list[i] = ( *allocator )();
		}
	}
	
	num = newNum;
}

/*
================
idList<_type_,_tag_>::operator=

Copies the contents and size attributes of another list.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE SbList<_type_, _tag_>& SbList<_type_, _tag_>::operator=( const SbList<_type_, _tag_>& other )
{
	int	i;
	
	Clear();
	
	num			= other.num;
	size		= other.size;
	granularity	= other.granularity;
	memTag		= other.memTag;
	
	if( size )
	{
		list = ( _type_* )idListArrayNew< _type_, _tag_ >( size, false );
		for( i = 0; i < num; i++ )
		{
			list[ i ] = other.list[ i ];
		}
	}
	
	return *this;
}

/*
================
idList<_type_,_tag_>::operator[] const

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE const _type_& SbList<_type_, _tag_>::operator[]( int index ) const
{
	assert( index >= 0 );
	assert( index < num );
	
	return list[ index ];
}

/*
================
idList<_type_,_tag_>::operator[]

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE _type_& SbList<_type_, _tag_>::operator[]( int index )
{
	assert( index >= 0 );
	assert( index < num );
	
	return list[ index ];
}

/*
================
idList<_type_,_tag_>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE _type_* SbList<_type_, _tag_>::Ptr()
{
	return list;
}

/*
================
idList<_type_,_tag_>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< typename _type_, memTag_t _tag_ >
const ID_INLINE _type_* SbList<_type_, _tag_>::Ptr() const
{
	return list;
}

/*
================
idList<_type_,_tag_>::Alloc

Returns a reference to a new data element at the end of the list.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE _type_& SbList<_type_, _tag_>::Alloc()
{
	if( !list )
	{
		Resize( granularity );
	}
	
	if( num == size )
	{
		Resize( size + granularity );
	}
	
	return list[ num++ ];
}

/*
================
idList<_type_,_tag_>::Append

Increases the size of the list by one element and copies the supplied data into it.

Returns the index of the new element.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::Append( _type_ const& obj )
{
	if( !list )
	{
		Resize( granularity );
	}
	
	if( num == size )
	{
		int newsize;
		
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}
	
	list[ num ] = obj;
	num++;
	
	return num - 1;
}


/*
================
idList<_type_,_tag_>::Insert

Increases the size of the list by at leat one element if necessary
and inserts the supplied data into it.

Returns the index of the new element.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::Insert( _type_ const& obj, int index )
{
	if( !list )
	{
		Resize( granularity );
	}
	
	if( num == size )
	{
		int newsize;
		
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}
	
	if( index < 0 )
	{
		index = 0;
	}
	else if( index > num )
	{
		index = num;
	}
	for( int i = num; i > index; --i )
	{
		list[i] = list[i - 1];
	}
	num++;
	list[index] = obj;
	return index;
}

/*
================
idList<_type_,_tag_>::Append

adds the other list to this one

Returns the size of the new combined list
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::Append( const SbList< _type_, _tag_ >& other )
{
	if( !list )
	{
		if( granularity == 0 )  	// this is a hack to fix our memset classes
		{
			granularity = 16;
		}
		Resize( granularity );
	}
	
	int n = other.Num();
	for( int i = 0; i < n; i++ )
	{
		Append( other[i] );
	}
	
	return Num();
}

/*
================
idList<_type_,_tag_>::AddUnique

Adds the data to the list if it doesn't already exist.  Returns the index of the data in the list.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::AddUnique( _type_ const& obj )
{
	int index;
	
	index = FindIndex( obj );
	if( index < 0 )
	{
		index = Append( obj );
	}
	
	return index;
}

/*
================
idList<_type_,_tag_>::FindIndex

Searches for the specified data in the list and returns it's index.  Returns -1 if the data is not found.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::FindIndex( _type_ const& obj ) const
{
	int i;
	
	for( i = 0; i < num; i++ )
	{
		if( list[ i ] == obj )
		{
			return i;
		}
	}
	
	// Not found
	return -1;
}

/*
================
idList<_type_,_tag_>::Find

Searches for the specified data in the list and returns it's address. Returns NULL if the data is not found.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE _type_* SbList<_type_, _tag_>::Find( _type_ const& obj ) const
{
	int i;
	
	i = FindIndex( obj );
	if( i >= 0 )
	{
		return &list[ i ];
	}
	
	return NULL;
}

/*
================
idList<_type_,_tag_>::FindNull

Searches for a NULL pointer in the list.  Returns -1 if NULL is not found.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::FindNull() const
{
	int i;
	
	for( i = 0; i < num; i++ )
	{
		if( list[ i ] == NULL )
		{
			return i;
		}
	}
	
	// Not found
	return -1;
}

/*
================
idList<_type_,_tag_>::IndexOf

Takes a pointer to an element in the list and returns the index of the element.
This is NOT a guarantee that the object is really in the list.
Function will assert in debug builds if pointer is outside the bounds of the list,
but remains silent in release builds.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE int SbList<_type_, _tag_>::IndexOf( _type_ const* objptr ) const
{
	int index;
	
	index = objptr - list;
	
	assert( index >= 0 );
	assert( index < num );
	
	return index;
}

/*
================
idList<_type_,_tag_>::RemoveIndex

Removes the element at the specified index and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the index is outside the bounds of the list.
Note that the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE bool SbList<_type_, _tag_>::RemoveIndex( int index )
{
	int i;
	
	assert( list != NULL );
	assert( index >= 0 );
	assert( index < num );
	
	if( ( index < 0 ) || ( index >= num ) )
	{
		return false;
	}
	
	num--;
	for( i = index; i < num; i++ )
	{
		list[ i ] = list[ i + 1 ];
	}
	
	return true;
}

/*
========================
idList<_type_,_tag_>::RemoveIndexFast

Removes the element at the specified index and moves the last element into its spot, rather
than moving the whole array down by one. Of course, this doesn't maintain the order of
elements! The number of elements in the list is reduced by one.

return:	bool	- false if the data is not found in the list.

NOTE:	The element is not destroyed, so any memory used by it may not be freed until the
		destruction of the list.
========================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE bool SbList<_type_, _tag_>::RemoveIndexFast( int index )
{

	if( ( index < 0 ) || ( index >= num ) )
	{
		return false;
	}
	
	num--;
	if( index != num )
	{
		list[ index ] = list[ num ];
	}
	
	return true;
}

/*
================
idList<_type_,_tag_>::Remove

Removes the element if it is found within the list and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the data is not found in the list.  Note that
the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE bool SbList<_type_, _tag_>::Remove( _type_ const& obj )
{
	int index;
	
	index = FindIndex( obj );
	if( index >= 0 )
	{
		return RemoveIndex( index );
	}
	
	return false;
}
//
///*
//================
//idList<_type_,_tag_>::Sort
//
//Performs a qsort on the list using the supplied comparison function.  Note that the data is merely moved around the
//list, so any pointers to data within the list may no longer be valid.
//================
//*/
//template< typename _type_, memTag_t _tag_ >
//ID_INLINE void SbList<_type_,_tag_>::Sort( cmp_t *compare ) {
//	if ( !list ) {
//		return;
//	}
//	typedef int cmp_c(const void *, const void *);
//
//	cmp_c *vCompare = (cmp_c *)compare;
//	qsort( ( void * )list, ( size_t )num, sizeof( _type_ ), vCompare );
//}

/*
========================
idList<_type_,_tag_>::SortWithTemplate

Performs a QuickSort on the list using the supplied sort algorithm.

Note:	The data is merely moved around the list, so any pointers to data within the list may
		no longer be valid.
========================
*/
template< typename _type_, memTag_t _tag_ >
ID_INLINE void SbList<_type_, _tag_>::SortWithTemplate( const idSort<_type_>& sort )
{
	if( list == NULL )
	{
		return;
	}
	sort.Sort( Ptr(), Num() );
}
//
///*
//================
//idList<_type_,_tag_>::SortSubSection
//
//Sorts a subsection of the list.
//================
//*/
//template< typename _type_, memTag_t _tag_ >
//ID_INLINE void SbList<_type_,_tag_>::SortSubSection( int startIndex, int endIndex, cmp_t *compare ) {
//	if ( !list ) {
//		return;
//	}
//	if ( startIndex < 0 ) {
//		startIndex = 0;
//	}
//	if ( endIndex >= num ) {
//		endIndex = num - 1;
//	}
//	if ( startIndex >= endIndex ) {
//		return;
//	}
//	typedef int cmp_c(const void *, const void *);
//
//	cmp_c *vCompare = (cmp_c *)compare;
//	qsort( ( void * )( &list[startIndex] ), ( size_t )( endIndex - startIndex + 1 ), sizeof( _type_ ), vCompare );
//}