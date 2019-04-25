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

//namespace sbe
//{

/*
================================================
idPlayerProfile

The general rule for using cvars for settings is that if you want the player's profile settings to affect the startup
of the game before there is a player associated with the game, use cvars.  Example: video & volume settings.
================================================
*/
struct IPlayerProfile
{
	virtual bool GetAchievement(const int id) const = 0;
	
	//------------------------
	// Config
	//------------------------
	virtual int GetConfig() const = 0;
	
	virtual void RestoreDefault() = 0;
	
	virtual void SetLeftyFlip(bool lf) = 0;
	virtual bool GetLeftyFlip() const = 0;
};

//}; // namespace sbe