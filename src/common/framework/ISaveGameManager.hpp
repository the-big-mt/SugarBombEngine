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

#define MAX_SAVEGAMES						16

// common descriptors for savegame description fields
#define SAVEGAME_DETAIL_FIELD_EXPANSION		"expansion"
#define SAVEGAME_DETAIL_FIELD_MAP			"mapName"
#define SAVEGAME_DETAIL_FIELD_MAP_LOCATE	"mapLocation"
#define SAVEGAME_DETAIL_FIELD_DIFFICULTY	"difficulty"
#define SAVEGAME_DETAIL_FIELD_PLAYTIME		"playTime"
#define SAVEGAME_DETAIL_FIELD_LANGUAGE		"language"
#define	SAVEGAME_DETAIL_FIELD_SAVE_VERSION	"saveVersion"
#define	SAVEGAME_DETAIL_FIELD_CHECKSUM		"checksum"

//namespace sbe
//{

/*
================================================
idSaveGameDetails
================================================
*/
class idSaveGameDetails
{
public:
	idSaveGameDetails();
	~idSaveGameDetails()
	{
		Clear();
	}
	
	void	Clear();
	bool	operator==( const idSaveGameDetails& other ) const
	{
		return ( idStr::Icmp( slotName, other.slotName ) == 0 );
	}
	idSaveGameDetails& 	operator=( const idSaveGameDetails& other )
	{
		descriptors.Clear();
		descriptors = other.descriptors;
		damaged = other.damaged;
		date = other.date;
		slotName = other.slotName;
		return *this;
	}
	// for std::sort, sort newer (larger date) towards start of list
	bool	operator<( const idSaveGameDetails& other ) const
	{
		return date > other.date;
	}
	
	idStr	GetMapName() const
	{
		return descriptors.GetString( SAVEGAME_DETAIL_FIELD_MAP, "" );
	}
	idStr	GetLocation() const
	{
		return descriptors.GetString( SAVEGAME_DETAIL_FIELD_MAP_LOCATE, "" );
	}
	idStr	GetLanguage() const
	{
		return descriptors.GetString( SAVEGAME_DETAIL_FIELD_LANGUAGE, "" );
	}
	int		GetPlaytime() const
	{
		return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_PLAYTIME, 0 );
	}
	int		GetExpansion() const
	{
		return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_EXPANSION, 0 );
	}
	int		GetDifficulty() const
	{
		return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_DIFFICULTY, -1 );
	}
	int		GetSaveVersion() const
	{
		return descriptors.GetInt( SAVEGAME_DETAIL_FIELD_SAVE_VERSION, 0 );
	}
	
public:
	idDict				descriptors;						// [in] Descriptors available to be shown on the save/load screen.  Each game can define their own, e.g. Difficulty, level, map, score, time.
	bool				damaged;							// [out]
	time_t				date;								// [out] read from the filesystem, not set by client
	idStrStatic< MAX_FOLDER_NAME_LENGTH >	slotName;		// [out] folder/slot name, e.g. AUTOSAVE
};

typedef idStaticList< idSaveGameDetails, MAX_SAVEGAMES > saveGameDetailsList_t;

/*
================================================
idSaveGameManager

Why all the object-oriented nonsense?
- Savegames need to be processed asynchronously, saving/loading/deleting files should happen during the game frame
	so there is a common way to update the render device.
- When executing commands, if no "strategy"s are used, the pump() method would need to have a switch statement,
	extending the manager for other commands would mean modifying the manager itself for various commands.
	By making it a strategy, we are able to create custom commands and define the behavior within game code and keep
	the manager code in the engine static.
================================================
*/
struct idSaveGameManager
{
	// Check to see if a processor is set within the manager
	virtual bool IsWorking() const = 0;
	
	virtual const saveGameDetailsList_t& GetEnumeratedSavegames() const = 0;
};

//}; // namespace sbe