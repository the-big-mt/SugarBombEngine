/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.


*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe::SbGameFramework
{

class SbGameThread : public idSysThread
{
public:
	SbGameThread() :
		gameTime(),
		drawTime(),
		threadTime(),
		threadGameTime(),
		threadRenderTime(),
		userCmdMgr( NULL ),
		ret(),
		numGameFrames(),
		isClient()
	{}
	
	// the gameReturn_t is from the previous frame, the
	// new frame will be running in parallel on exit
	gameReturn_t	RunGameAndDraw( int numGameFrames, idUserCmdMgr& userCmdMgr_, bool isClient_, int startGameFrame );
	
	// Accessors to the stored frame/thread time information
	void			SetThreadTotalTime( const int inTime )
	{
		threadTime = inTime;
	}
	int				GetThreadTotalTime() const
	{
		return threadTime;
	}
	
	void			SetThreadGameTime( const int time )
	{
		threadGameTime = time;
	}
	int				GetThreadGameTime() const
	{
		return threadGameTime;
	}
	
	void			SetThreadRenderTime( const int time )
	{
		threadRenderTime = time;
	}
	int				GetThreadRenderTime() const
	{
		return threadRenderTime;
	}
	
private:
	virtual int	Run();
	
	int				gameTime;
	int				drawTime;
	int				threadTime;					// total time : game time + foreground render time
	int				threadGameTime;				// game time only
	int				threadRenderTime;			// render fg time only
	idUserCmdMgr* 	userCmdMgr;
	gameReturn_t	ret;
	int				numGameFrames;
	bool			isClient;
};

}; // namespace sbe::SbGameFramework