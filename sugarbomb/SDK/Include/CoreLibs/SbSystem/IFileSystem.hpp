/*
*******************************************************************************

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

*******************************************************************************
*/

/// @file
/// @brief virtual file system interface

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct IFile;

struct IFileSystem
{
	///
	virtual void Init() = 0;
	
	///

	/// This will shut down the entire vfs
	virtual void Shutdown() = 0;

	/**
	 * This method will try to open a file located following the specified path
	 *
	 * @param asPath - path to the file + file name itself
	 * @return a valid pointer to a IFile structure or nullptr on failure
	 */
	virtual IFile *OpenFile(const char *asPath) = 0;

	/**
	 * Use this method to close the previously opened file
	 *
	 * @param apFile - a valid pointer to the IFile structure
	 */
	virtual void CloseFile(IFile *apFile) = 0;
};

}; // namespace sbe