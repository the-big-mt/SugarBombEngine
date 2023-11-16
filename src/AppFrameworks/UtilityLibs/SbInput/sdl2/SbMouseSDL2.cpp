/*
*******************************************************************************

Copyright (C) 2013 Daniel Gibson
Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include "SbMouseSDL2.hpp"

//*****************************************************************************

namespace sbe::SbInput
{

SbMouseSDL2::SbMouseSDL2()
{
	mouse_polls.SetGranularity(64);
};

SbMouseSDL2::~SbMouseSDL2()
{
	mouse_polls.Clear();
};

/*
================
Sys_PollMouseInputEvents
================
*/
int SbMouseSDL2::PollInputEvents(int mouseEvents[MAX_MOUSE_EVENTS][2])
{
	int numEvents = mouse_polls.Num();
	
	if( numEvents > MAX_MOUSE_EVENTS )
		numEvents = MAX_MOUSE_EVENTS;
	
	for( int i = 0; i < numEvents; i++ )
	{
		const mouse_poll_t& mp = mouse_polls[i];
		
		mouseEvents[i][0] = mp.action;
		mouseEvents[i][1] = mp.value;
	};
	
	mouse_polls.SetNum( 0 );
	
	return numEvents;
};

}; // namespace sbe::SbInput