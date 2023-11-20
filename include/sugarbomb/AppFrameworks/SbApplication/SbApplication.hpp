/*
*******************************************************************************

Copyright (C) 2019-2020, 2022 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @brief general application framework

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct SbSystem;

class SbApplication
{
public:
	/**
	 * Application's platform-independent entry point
	 *
	 * @param argc - number of the app's launch command-line arguments
	 * @param argv - array of strings containing all the command-line arguments
	 * @return either EXIT_SUCCESS or EXIT_FAILURE depending on the app's execution result
	 */
	static int Main(int argc, char **argv); // TODO: -> SbMain free func?
public:
	SbApplication(SbSystem &aSystem, int argc, char **argv);
	virtual ~SbApplication();

	/// This method first initializes the app and then runs it
	/// @return execution result
	virtual int Run() = 0;
protected:
	/// Call this method if you want to stop the app's execution and close it
	void Stop();

	/// @return true if the app wants stop its execution and be closed
	bool CloseRequested() const {return mbWantClose;}

	///
	//virtual void OnInit(){}

	///
	//virtual void OnShutdown(){}

	SbSystem &mSystem; // TODO: unused?
private:
	void Init();

	void ParseCommandLine(int argc, const char * const *argv);
private:
	bool mbInitialized{false};
	bool mbWantClose{false};
};

}; // namespace sbe