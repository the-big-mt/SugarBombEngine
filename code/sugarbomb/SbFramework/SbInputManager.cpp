/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 SugarBombEngine Developers

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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain
additional terms. You should have received a copy of these additional
terms immediately following the terms and conditions of the GNU General
Public License which accompanied the Doom 3 BFG Edition Source Code.
If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/*
===============
KeysFromBinding()
Returns the key bound to the command
===============
*/
const char *SbInputManager::KeysFromBinding( const char* bind ) const
{
	return idKeyInput::KeysFromBinding( bind );
};

/*
===============
BindingFromKey()
Returns the binding bound to key
===============
*/
const char* SbInputManager::BindingFromKey( const char* key ) const
{
	return idKeyInput::BindingFromKey( key );
};

/*
===============
ButtonState()
Returns the state of the button
===============
*/
int	SbInputManager::ButtonState( int key ) const
{
	return usercmdGen->ButtonState( key );
};

/*
===============
ButtonState()
Returns the state of the key
===============
*/
int	SbInputManager::KeyState( int key ) const
{
	return usercmdGen->KeyState( key );
};