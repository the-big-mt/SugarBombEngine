/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 BlackPhrase

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

/*
================================================
idFileLocal is a FileStream wrapper that automatically closes a file when the
class variable goes out of scope. Note that the pointer passed in to the constructor can be for
any type of File Stream that ultimately inherits from idFile, and that this is not actually a
SmartPointer, as it does not keep a reference count.
================================================
*/
class idFileLocal
{
public:
	// Constructor that accepts and stores the file pointer.
	idFileLocal( idFile* _file )	: file( _file )
	{
	}
	
	// Destructor that will destroy (close) the file when this wrapper class goes out of scope.
	~idFileLocal();
	
	// Cast to a file pointer.
	operator idFile* () const
	{
		return file;
	}
	
	// Member access operator for treating the wrapper as if it were the file, itself.
	idFile* operator -> () const
	{
		return file;
	}
	
protected:
	idFile* file;	// The managed file pointer.
};