/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 SugarBombEngine Developers

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

#pragma hdrstop
#include "precompiled.h"
//#include "../idlib/containers/HashIndex.h"

//#include <cstdlib>
//#include <cstring>

//#include "../idlib/math/Math.h"
//#include "Heap.h"
//#include "sys/sys_assert.h"

//namespace BFG
//{

int idHashIndex::INVALID_INDEX[1] = { -1 };

/*
================
idHashIndex::Init
================
*/
void idHashIndex::Init( const int initialHashSize, const int initialIndexSize )
{
	assert( idMath::IsPowerOfTwo( initialHashSize ) );
	
	hashSize = initialHashSize;
	hash = INVALID_INDEX;
	indexSize = initialIndexSize;
	indexChain = INVALID_INDEX;
	granularity = DEFAULT_HASH_GRANULARITY;
	hashMask = hashSize - 1;
	lookupMask = 0;
}

/*
================
idHashIndex::Allocate
================
*/
void idHashIndex::Allocate( const int newHashSize, const int newIndexSize )
{
	assert( idMath::IsPowerOfTwo( newHashSize ) );
	
	Free();
	hashSize = newHashSize;
	hash = new( TAG_IDLIB_HASH ) int[hashSize];
	memset( hash, 0xff, hashSize * sizeof( hash[0] ) );
	indexSize = newIndexSize;
	indexChain = new( TAG_IDLIB_HASH ) int[indexSize];
	memset( indexChain, 0xff, indexSize * sizeof( indexChain[0] ) );
	hashMask = hashSize - 1;
	lookupMask = -1;
}

/*
================
idHashIndex::Free
================
*/
void idHashIndex::Free()
{
	if( hash != INVALID_INDEX )
	{
		delete[] hash;
		hash = INVALID_INDEX;
	}
	if( indexChain != INVALID_INDEX )
	{
		delete[] indexChain;
		indexChain = INVALID_INDEX;
	}
	lookupMask = 0;
}

/*
================
idHashIndex::ResizeIndex
================
*/
void idHashIndex::ResizeIndex( const int newIndexSize )
{
	int* oldIndexChain, mod, newSize;
	
	if( newIndexSize <= indexSize )
	{
		return;
	}
	
	mod = newIndexSize % granularity;
	if( !mod )
	{
		newSize = newIndexSize;
	}
	else
	{
		newSize = newIndexSize + granularity - mod;
	}
	
	if( indexChain == INVALID_INDEX )
	{
		indexSize = newSize;
		return;
	}
	
	oldIndexChain = indexChain;
	indexChain = new( TAG_IDLIB_HASH ) int[newSize];
	memcpy( indexChain, oldIndexChain, indexSize * sizeof( int ) );
	memset( indexChain + indexSize, 0xff, ( newSize - indexSize ) * sizeof( int ) );
	delete[] oldIndexChain;
	indexSize = newSize;
}

/*
================
idHashIndex::GetSpread
================
*/
int idHashIndex::GetSpread() const
{
	int i, index, totalItems, *numHashItems, average, error, e;
	
	if( hash == INVALID_INDEX )
	{
		return 100;
	}
	
	totalItems = 0;
	numHashItems = new( TAG_IDLIB_HASH ) int[hashSize];
	for( i = 0; i < hashSize; i++ )
	{
		numHashItems[i] = 0;
		for( index = hash[i]; index >= 0; index = indexChain[index] )
		{
			numHashItems[i]++;
		}
		totalItems += numHashItems[i];
	}
	// if no items in hash
	if( totalItems <= 1 )
	{
		delete[] numHashItems;
		return 100;
	}
	average = totalItems / hashSize;
	error = 0;
	for( i = 0; i < hashSize; i++ )
	{
		e = abs( numHashItems[i] - average );
		if( e > 1 )
		{
			error += e - 1;
		}
	}
	delete[] numHashItems;
	return 100 - ( error * 100 / totalItems );
}

//} // namespace BFG
