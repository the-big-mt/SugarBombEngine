/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#ifndef __FILE_H__
#define __FILE_H__

//#include <minizip/ioapi.h>

//#include "../idlib/Swap.h"
//#include "../idlib/Str.h"
//#include "../idlib/BitMsg.h"

//namespace BFG
//{

/*
==============================================================

  File Streams.

==============================================================
*/

class idFileSystemLocal;

#include "framework/IFile.hpp"

/*
================================================
idFile_Memory
================================================
*/
class idFile_Memory : public idFile
{
	friend class			idFileSystemLocal;
	
public:
	idFile_Memory();	// file for writing without name
	idFile_Memory( const char* name );	// file for writing
	idFile_Memory( const char* name, char* data, int length );	// file for writing
	idFile_Memory( const char* name, const char* data, int length );	// file for reading
	virtual					~idFile_Memory();
	
	virtual const char* 	GetName() const
	{
		return name.c_str();
	}
	virtual const char* 	GetFullPath() const
	{
		return name.c_str();
	}
	virtual int				Read( void* buffer, int len );
	virtual int				Write( const void* buffer, int len );
	virtual int				Length() const;
	virtual void			SetLength( size_t len );
	virtual ID_TIME_T		Timestamp() const;
	virtual int				Tell() const;
	virtual void			ForceFlush();
	virtual void			Flush();
	virtual int				Seek( long offset, fsOrigin_t origin );
	
	// Set the given length and don't allow the file to grow.
	void					SetMaxLength( size_t len );
	// changes memory file to read only
	void					MakeReadOnly();
	// Change the file to be writable
	void					MakeWritable();
	// clear the file
	virtual void			Clear( bool freeMemory = true );
	// set data for reading
	void					SetData( const char* data, int length );
	// returns const pointer to the memory buffer
	const char* 			GetDataPtr() const
	{
		return filePtr;
	}
	// returns pointer to the memory buffer
	char* 					GetDataPtr()
	{
		return filePtr;
	}
	// set the file granularity
	void					SetGranularity( int g )
	{
		assert( g > 0 );
		granularity = g;
	}
	void					PreAllocate( size_t len );
	
	// Doesn't change how much is allocated, but allows you to set the size of the file to smaller than it should be.
	// Useful for stripping off a checksum at the end of the file
	void					TruncateData( size_t len );
	
	void					TakeDataOwnership();
	
	size_t					GetMaxLength()
	{
		return maxSize;
	}
	size_t					GetAllocated()
	{
		return allocated;
	}
	
protected:
	idStr					name;			// name of the file
private:
	int						mode;			// open mode
	size_t					maxSize;		// maximum size of file
	size_t					fileSize;		// size of the file
	size_t					allocated;		// allocated size
	int						granularity;	// file granularity
	char* 					filePtr;		// buffer holding the file data
	char* 					curPtr;			// current read/write pointer
};


class idFile_BitMsg : public idFile
{
	friend class			idFileSystemLocal;
	
public:
	idFile_BitMsg( idBitMsg& msg );
	idFile_BitMsg( const idBitMsg& msg );
	virtual					~idFile_BitMsg();
	
	virtual const char* 	GetName() const
	{
		return name.c_str();
	}
	virtual const char* 	GetFullPath() const
	{
		return name.c_str();
	}
	virtual int				Read( void* buffer, int len );
	virtual int				Write( const void* buffer, int len );
	virtual int				Length() const;
	virtual ID_TIME_T		Timestamp() const;
	virtual int				Tell() const;
	virtual void			ForceFlush();
	virtual void			Flush();
	virtual int				Seek( long offset, fsOrigin_t origin );
	
private:
	idStr					name;			// name of the file
	int						mode;			// open mode
	idBitMsg* 				msg;
};


class idFile_Permanent : public idFile
{
	friend class			idFileSystemLocal;
	
public:
	idFile_Permanent();
	virtual					~idFile_Permanent();
	
	virtual const char* 	GetName() const
	{
		return name.c_str();
	}
	virtual const char* 	GetFullPath() const
	{
		return fullPath.c_str();
	}
	virtual int				Read( void* buffer, int len );
	virtual int				Write( const void* buffer, int len );
	virtual int				Length() const;
	virtual ID_TIME_T		Timestamp() const;
	virtual int				Tell() const;
	virtual void			ForceFlush();
	virtual void			Flush();
	virtual int				Seek( long offset, fsOrigin_t origin );
	
	// returns file pointer
	idFileHandle			GetFilePtr()
	{
		return o;
	}
	
private:
	idStr					name;			// relative path of the file - relative path
	idStr					fullPath;		// full file path - OS path
	int						mode;			// open mode
	int						fileSize;		// size of the file
	idFileHandle			o;				// file handle
	bool					handleSync;		// true if written data is immediately flushed
};

class idFile_Cached : public idFile_Permanent
{
	friend class			idFileSystemLocal;
public:
	idFile_Cached();
	virtual					~idFile_Cached();
	
	void					CacheData( uint64 offset, uint64 length );
	
	virtual int				Read( void* buffer, int len );
	
	virtual int				Tell() const;
	virtual int				Seek( long offset, fsOrigin_t origin );
	
private:
	uint64				internalFilePos;
	uint64				bufferedStartOffset;
	uint64				bufferedEndOffset;
	byte* 				buffered;
};


class idFile_InZip : public idFile
{
	friend class			idFileSystemLocal;
	
public:
	idFile_InZip();
	virtual					~idFile_InZip();
	
	virtual const char* 	GetName() const
	{
		return name.c_str();
	}
	virtual const char* 	GetFullPath() const
	{
		return fullPath.c_str();
	}
	virtual int				Read( void* buffer, int len );
	virtual int				Write( const void* buffer, int len );
	virtual int				Length() const;
	virtual ID_TIME_T		Timestamp() const;
	virtual int				Tell() const;
	virtual void			ForceFlush();
	virtual void			Flush();
	virtual int				Seek( long offset, fsOrigin_t origin );
	
private:
	idStr					name;			// name of the file in the pak
	idStr					fullPath;		// full file path including pak file name
	// DG: use ZPOS64_T, it's the type minizip uses and should also work with zip64 files > 2GB
	ZPOS64_T				zipFilePos;		// zip file info position in pak
	// DG end
	int						fileSize;		// size of the file
	void* 					z;				// unzip info
};

#if 1
class idFile_InnerResource : public idFile
{
	friend class			idFileSystemLocal;
	
public:
	idFile_InnerResource( const char* _name, idFile* rezFile, int _offset, int _len );
	virtual					~idFile_InnerResource();
	
	virtual const char* 	GetName() const
	{
		return name.c_str();
	}
	virtual const char* 	GetFullPath() const
	{
		return name.c_str();
	}
	virtual int				Read( void* buffer, int len );
	virtual int				Write( const void* buffer, int len )
	{
		assert( false );
		return 0;
	}
	virtual int				Length() const
	{
		return length;
	}
	virtual ID_TIME_T		Timestamp() const
	{
		return 0;
	}
	virtual int				Tell() const;
	virtual int				Seek( long offset, fsOrigin_t origin );
	void					SetResourceBuffer( byte* buf )
	{
		resourceBuffer = buf;
		internalFilePos = 0;
	}
	
private:
	idStr				name;				// name of the file in the pak
	int					offset;				// offset in the resource file
	int					length;				// size
	idFile* 			resourceFile;		// actual file
	int					internalFilePos;	// seek offset
	byte* 				resourceBuffer;		// if using the temp save memory
};
#endif

//} // namespace BFG

#endif /* !__FILE_H__ */
