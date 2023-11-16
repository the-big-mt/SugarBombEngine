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

#include "SbKeyboardSDL2.hpp"

//*****************************************************************************

SbKeyboardSDL2::SbKeyboardSDL2()
{
	kbd_polls.SetGranularity( 64 );
};

SbKeyboardSDL2::~SbKeyboardSDL2()
{
	kbd_polls.Clear();
};

/*
================
Sys_PollKeyboardInputEvents
================
*/
int SbKeyboardSDL2::PollInputEvents()
{
	return kbd_polls.Num();
};

/*
================
Sys_ReturnKeyboardInputEvent
================
*/
int SbKeyboardSDL2::ReturnKeyboardInputEvent( const int n, int& key, bool& state )
{
	if( n >= kbd_polls.Num() )
		return 0;
		
	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
};

/*
================
Sys_EndKeyboardInputEvents
================
*/
void SbKeyboardSDL2::EndKeyboardInputEvents()
{
	kbd_polls.SetNum( 0 );
};