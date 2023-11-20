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

//#include "../idlib/containers/List.h"
//#include "../idlib/containers/HashIndex.h"
//#include "../idlib/Str.h"

//*****************************************************************************

//namespace BFG
//{

class idLangKeyValue
{
public:
	idLangKeyValue() : key( NULL ), value( NULL ) { }
	idLangKeyValue( char* k, char* v ) : key( k ), value( v ) { }
	char* key;
	char* value;
};

class idStrId;

/*
================================================
idLangDict is a simple Dictionary used specifically for the
LocalizedStringTables.
================================================
*/
class idLangDict
{
public:
	static const char* 		KEY_PREFIX;
	static const int		KEY_PREFIX_LEN;
	static const int		MAX_REDIRECTION_DEPTH = 2;
	
	idLangDict();
	~idLangDict();
	
	void					Clear();
	bool					Load( const byte* buffer, const int bufferLen, const char* name );
	bool					Save( const char* fileName );
	
	const char* 			GetString( const char* str ) const;		// returns str if string not found
	const char* 			FindString( const char* str ) const;		// returns NULL if string not found
	
	const char* 			AddString( const char* str );				// returns a randomly generated key
	bool					DeleteString( const char* key );			// returns false if the key doesn't exist
	bool					RenameStringKey( const char* oldKey, const char* newKey );
	
	bool					SetString( const char* key, const char* val );	// Returns false if the key doesn't exist
	void					AddKeyVal( const char* key, const char* val );	// Like SetString, but adds it if it doesn't already exist
	
	int						GetNumKeyVals() const;
	const idLangKeyValue* 	GetKeyVal( int i ) const;
	bool					DeleteString( const int idx );
	
	const char* 			GetLocalizedString( const idStrId& strId ) const;
	
	// returns true if the string starts with the KEY_PREFIX string
	static bool				IsStringId( const char* str );
	
private:
	idDynamicBlockAlloc< char, 100 * 1024, 16 > blockAlloc;
	idList< idLangKeyValue >	keyVals;
	idHashIndex keyIndex;
	
private:
	int						FindStringIndex( const char* str ) const;
	const char* 			FindString_r( const char* str, int& depth ) const;
	
	friend class idStrId;
};

/*
================================================
idLocalization
================================================
*/
class idLocalization
{
public:
	static const char* 		GetString( const char* inString );		// returns inString if string not found
	static const char* 		FindString( const char* inString );	// Returns NULL if string not found
	
	static void				ClearDictionary();
	static bool				LoadDictionary( const byte* buffer, const int bufferLen, const char* name );
	
	// This is only here for tools, normal code should only ever call GetString
	static idLangDict& 		GetDictionary()
	{
		return languageDict;
	}
	
	static utf8Encoding_t	VerifyUTF8( const uint8* buffer, const int bufferLen, const char* name );
	
private:
	static idLangDict					languageDict;
	friend class idStrId;
};

/*
================================================
idStrId represents a localized String as a String ID.
================================================
*/
class idStrId
{
public:
	idStrId() : index( -1 ) { }
	idStrId( const idStrId& other ) : index( other.index ) { }
	
	explicit idStrId( int i ) : index( i ) { }
	explicit idStrId( const char* key )
	{
		Set( key );
	}
	explicit idStrId( const idStr& key )
	{
		Set( key );
	}
	
	void operator=( const char* key )
	{
		Set( key );
	}
	void operator=( const idStr& key )
	{
		Set( key );
	}
	void operator=( const idStrId& other )
	{
		index = other.index;
	}
	
	bool operator==( const idStrId& other ) const
	{
		return index == other.index;
	}
	bool operator!=( const idStrId& other ) const
	{
		return index != other.index;
	}
	
	void			Set( const char* key );
	
	void			Empty()
	{
		index = -1;
	}
	bool			IsEmpty() const
	{
		return index < 0;
	}
	
	const char* 	GetKey() const;
	const char* 	GetLocalizedString() const;
	
	int				GetIndex() const
	{
		return index;
	}
	void			SetIndex( int i )
	{
		index = i;
	}
	
private:
	int index;	// Index into the language dictionary
};

//} // namespace BFG