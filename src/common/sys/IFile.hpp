/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2019 BlackPhrase

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

namespace sbe
{

// mode parm for Seek
typedef enum
{
	FS_SEEK_CUR,
	FS_SEEK_END,
	FS_SEEK_SET
} fsOrigin_t;

struct IFile
{
	// Get the name of the file.
	virtual const char* 	GetName() const = 0;
	// Get the full file path.
	virtual const char* 	GetFullPath() const = 0;
	// Read data from the file to the buffer.
	virtual int				Read( void* buffer, int len ) = 0;
	// Write data from the buffer to the file.
	virtual int				Write( const void* buffer, int len ) = 0;
	// Returns the length of the file.
	virtual int				Length() const;
	// Return a time value for reload operations.
	virtual ID_TIME_T		Timestamp() const = 0;
	// Returns offset in file.
	virtual int				Tell() const = 0;
	// Forces flush on files being writting to.
	virtual void			ForceFlush() = 0;
	// Causes any buffered data to be written to the file.
	virtual void			Flush() = 0;
	// Seek on a file.
	virtual int				Seek( long offset, fsOrigin_t origin ) = 0;
	// Go back to the beginning of the file.
	virtual void			Rewind() = 0;
	// Like fprintf.
	virtual int				Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	// Like fprintf but with argument pointer
	virtual int				VPrintf( const char* fmt, va_list arg ) = 0;
	// Write a string with high precision floating point numbers to the file.
	virtual int				WriteFloatString( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	// Endian portable alternatives to Read(...)
	virtual int				ReadInt( int& value ) = 0;
	virtual int				ReadUnsignedInt( unsigned int& value ) = 0;
	virtual int				ReadShort( short& value ) = 0;
	virtual int				ReadUnsignedShort( unsigned short& value ) = 0;
	virtual int				ReadChar( char& value ) = 0;
	virtual int				ReadUnsignedChar( unsigned char& value ) = 0;
	virtual int				ReadFloat( float& value ) = 0;
	virtual int				ReadBool( bool& value ) = 0;
	virtual int				ReadString( idStr& string ) = 0;
	virtual int				ReadVec2( idVec2& vec ) = 0;
	virtual int				ReadVec3( idVec3& vec ) = 0;
	virtual int				ReadVec4( idVec4& vec ) = 0;
	virtual int				ReadVec6( idVec6& vec ) = 0;
	virtual int				ReadMat3( idMat3& mat ) = 0;
	
	// Endian portable alternatives to Write(...)
	virtual int				WriteInt( const int value ) = 0;
	virtual int				WriteUnsignedInt( const unsigned int value ) = 0;
	virtual int				WriteShort( const short value ) = 0;
	virtual int				WriteUnsignedShort( unsigned short value ) = 0;
	virtual int				WriteChar( const char value ) = 0;
	virtual int				WriteUnsignedChar( const unsigned char value ) = 0;
	virtual int				WriteFloat( const float value ) = 0;
	virtual int				WriteBool( const bool value ) = 0;
	virtual int				WriteString( const char* string ) = 0;
	virtual int				WriteVec2( const idVec2& vec ) = 0;
	virtual int				WriteVec3( const idVec3& vec ) = 0;
	virtual int				WriteVec4( const idVec4& vec ) = 0;
	virtual int				WriteVec6( const idVec6& vec ) = 0;
	virtual int				WriteMat3( const idMat3& mat ) = 0;

	template <typename type>
	ID_INLINE size_t ReadBig( type& c )
	{
		size_t r = Read( &c, sizeof( c ) );
		idSwap::Big( c );
		return r;
	};
	
	template <typename type>
	ID_INLINE size_t ReadBigArray( type* c, int count )
	{
		size_t r = Read( c, sizeof( c[0] ) * count );
		idSwap::BigArray( c, count );
		return r;
	};
	
	template <typename type>
	ID_INLINE size_t WriteBig( const type& c )
	{
		type b = c;
		idSwap::Big( b );
		return Write( &b, sizeof( b ) );
	};
	
	template <typename type>
	ID_INLINE size_t WriteBigArray( const type* c, int count )
	{
		size_t r = 0;
		for( int i = 0; i < count; i++ )
			r += WriteBig( c[i] );

		return r;
	};
};
class idFile
{
public:
	virtual					~idFile() {};
	// Get the name of the file.
	virtual const char* 	GetName() const;
	// Get the full file path.
	virtual const char* 	GetFullPath() const;
	// Read data from the file to the buffer.
	virtual int				Read( void* buffer, int len );
	// Write data from the buffer to the file.
	virtual int				Write( const void* buffer, int len );
	// Returns the length of the file.
	virtual int				Length() const;
	// Return a time value for reload operations.
	virtual ID_TIME_T		Timestamp() const;
	// Returns offset in file.
	virtual int				Tell() const;
	// Forces flush on files being writting to.
	virtual void			ForceFlush();
	// Causes any buffered data to be written to the file.
	virtual void			Flush();
	// Seek on a file.
	virtual int				Seek( long offset, fsOrigin_t origin );
	// Go back to the beginning of the file.
	virtual void			Rewind();
	// Like fprintf.
	virtual int				Printf( VERIFY_FORMAT_STRING const char* fmt, ... );
	// Like fprintf but with argument pointer
	virtual int				VPrintf( const char* fmt, va_list arg );
	// Write a string with high precision floating point numbers to the file.
	virtual int				WriteFloatString( VERIFY_FORMAT_STRING const char* fmt, ... );
	
	// Endian portable alternatives to Read(...)
	virtual int				ReadInt( int& value );
	virtual int				ReadUnsignedInt( unsigned int& value );
	virtual int				ReadShort( short& value );
	virtual int				ReadUnsignedShort( unsigned short& value );
	virtual int				ReadChar( char& value );
	virtual int				ReadUnsignedChar( unsigned char& value );
	virtual int				ReadFloat( float& value );
	virtual int				ReadBool( bool& value );
	virtual int				ReadString( idStr& string );
	virtual int				ReadVec2( idVec2& vec );
	virtual int				ReadVec3( idVec3& vec );
	virtual int				ReadVec4( idVec4& vec );
	virtual int				ReadVec6( idVec6& vec );
	virtual int				ReadMat3( idMat3& mat );
	
	// Endian portable alternatives to Write(...)
	virtual int				WriteInt( const int value );
	virtual int				WriteUnsignedInt( const unsigned int value );
	virtual int				WriteShort( const short value );
	virtual int				WriteUnsignedShort( unsigned short value );
	virtual int				WriteChar( const char value );
	virtual int				WriteUnsignedChar( const unsigned char value );
	virtual int				WriteFloat( const float value );
	virtual int				WriteBool( const bool value );
	virtual int				WriteString( const char* string );
	virtual int				WriteVec2( const idVec2& vec );
	virtual int				WriteVec3( const idVec3& vec );
	virtual int				WriteVec4( const idVec4& vec );
	virtual int				WriteVec6( const idVec6& vec );
	virtual int				WriteMat3( const idMat3& mat );
};

}; // namespace sbe