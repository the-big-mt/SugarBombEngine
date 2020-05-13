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
/// @brief general application framework

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct ISystem;

class SbApplication
{
public:
	/**
	 * Application's platform-independent entry point
	 *
	 * @param argc - number of the app's launch command-line arguments
	 * @param argv - array of strings containing all the command-line arguments
	 * @return either EXIT_SUCCESS or EXIT_FAILURE dependiing on the app's execution result
	 */
	static int Main(int argc, char **argv);
public:
	SbApplication(ISystem &aSystem, int argc, char **argv);
	virtual ~SbApplication();

	/// This method first initializes the app and runs it
	void Run();
protected:
	/// Call this method if you want to stop the app's execution and close it
	void Stop();

	/// @return true if the app wants stop its execution and be closed
	bool CloseRequested() const {return mbWantClose;}

	/**
	 * This method is getting called before the start of each frame,
	 * override it if you want to prevent the frame execution under some conditions
	 *
	 * @return true if the frame is allowed to be executed, false otherwise
	 */
	virtual bool PreFrame(){return true;}

	/// This method should contain stuff that needs to be executed each frame
	virtual void RunFrame(){}

	/// This method can be used to do things at the end of each frame
	virtual void PostFrame(){}

	ISystem &mSystem;
private:
	void Init();
	
	bool mbInitialized{false};
	bool mbWantClose{false};
};

}; // namespace sbe