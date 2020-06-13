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

#include "SbGameThread.hpp"

//*****************************************************************************

namespace sbe::SbGameFramework
{

/*
===============
idGameThread::Run

Run in a background thread for performance, but can also
be called directly in the foreground thread for comparison.
===============
*/
int idGameThread::Run()
{
	commonLocal.frameTiming.startGameTime = Sys_Microseconds();
	
	// debugging tool to test frame dropping behavior
	if( com_sleepGame.GetInteger() )
	{
		Sys_Sleep( com_sleepGame.GetInteger() );
	}
	
	if( numGameFrames == 0 )
	{
		// Ensure there's no stale gameReturn data from a paused game
		ret = gameReturn_t();
	}
	
	if( isClient )
	{
		// run the game logic
		for( int i = 0; i < numGameFrames; i++ )
		{
			SCOPED_PROFILE_EVENT( "Client Prediction" );
			if( userCmdMgr )
			{
				game->ClientRunFrame( *userCmdMgr, ( i == numGameFrames - 1 ), ret );
			}
			if( ret.syncNextGameFrame || ret.sessionCommand[0] != 0 )
			{
				break;
			}
		}
	}
	else
	{
		// run the game logic
		for( int i = 0; i < numGameFrames; i++ )
		{
			SCOPED_PROFILE_EVENT( "GameTic" );
			if( userCmdMgr )
			{
				game->RunFrame( *userCmdMgr, ret );
			}
			if( ret.syncNextGameFrame || ret.sessionCommand[0] != 0 )
			{
				break;
			}
		}
	}
	
	// we should have consumed all of our usercmds
	if( userCmdMgr )
	{
		if( userCmdMgr->HasUserCmdForPlayer( game->GetLocalClientNum() ) )
		{
			idLib::Printf( "idGameThread::Run: didn't consume all usercmds\n" );
		}
	}
	
	commonLocal.frameTiming.finishGameTime = Sys_Microseconds();
	
	SetThreadGameTime( ( commonLocal.frameTiming.finishGameTime - commonLocal.frameTiming.startGameTime ) / 1000 );
	
	// build render commands and geometry
	{
		SCOPED_PROFILE_EVENT( "Draw" );
		commonLocal.Draw();
	}
	
	commonLocal.frameTiming.finishDrawTime = Sys_Microseconds();
	
	SetThreadRenderTime( ( commonLocal.frameTiming.finishDrawTime - commonLocal.frameTiming.finishGameTime ) / 1000 );
	
	SetThreadTotalTime( ( commonLocal.frameTiming.finishDrawTime - commonLocal.frameTiming.startGameTime ) / 1000 );
	
	return 0;
}

/*
===============
idGameThread::RunGameAndDraw

===============
*/
gameReturn_t idGameThread::RunGameAndDraw( int numGameFrames_, idUserCmdMgr& userCmdMgr_, bool isClient_, int startGameFrame )
{
	// this should always immediately return
	this->WaitForThread();
	
	// save the usercmds for the background thread to pick up
	userCmdMgr = &userCmdMgr_;
	
	isClient = isClient_;
	
	// grab the return value created by the last thread execution
	gameReturn_t latchedRet = ret;
	
	numGameFrames = numGameFrames_;
	
	// start the thread going
	if( com_smp.GetInteger() <= 0 )
	{
		// run it in the main thread so PIX profiling catches everything
		Run();
	}
	else
	{
		this->SignalWork();
	}
	
	// return the latched result while the thread runs in the background
	return latchedRet;
}

}; // namespace sbe::SbGameFramework