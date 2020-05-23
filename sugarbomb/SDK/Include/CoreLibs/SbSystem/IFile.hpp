/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2020 SugarBombEngine Developers

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
/// @brief abstract file interface

//*****************************************************************************

#pragma once

// TODO: temp
#define VERIFY_FORMAT_STRING
#define ID_TIME_T int

//*****************************************************************************

namespace sbe
{

struct IFile
{
	/// Read data from the file to the buffer
	virtual int Read(void *apBuffer, int anLen) = 0;

	/// Write data from the buffer to the file
	virtual int Write(const void *apBuffer, int anLen) = 0;

	/// Like fprintf
	virtual int Printf(VERIFY_FORMAT_STRING const char *asFmt, ...) = 0;

	/// Mode param for Seek
	enum class SeekMode : int
	{
		Current, // TODO: was FS_SEEK_CUR
		End, // TODO: was FS_SEEK_END
		Set // TODO: was FS_SEEK_SET
	};

	/// Seek on a file
	virtual int Seek(long anOffset, SeekMode aeOrigin) = 0;

	/// Returns offset in file
	virtual int Tell() const = 0;

	/// Go back to the beginning of the file
	virtual void Rewind() = 0;

	/// Causes any buffered data to be written to the file.
	/// @param abForce - Forces flush on files being writting to
	virtual void Flush(bool abForce) = 0;

	/// Get the name of the file
	virtual const char *GetName() const = 0;

	/// Get the full file path
	virtual const char *GetFullPath() const = 0;

	/// Returns the length of the file
	virtual int GetSize() const;

	/// Return a time value for reload operations
	virtual ID_TIME_T GetTimeStamp() const = 0;
};

}; // namespace sbe