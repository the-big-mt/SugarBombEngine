/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

/*
===============================================================================

	Sound API.

===============================================================================
*/

constexpr auto SOUND_API_VERSION{1};

struct ISystem;
struct idCmdSystem;
struct idCVarSystem;
struct IFileSystem;
struct IDeclManager;

struct soundImport_t
{
	int							version;				///< API version
	ISystem* 					sys;					///< non-portable system services
	idCmdSystem* 				cmdSystem;				///< console command system
	idCVarSystem* 				cvarSystem;				///< console variable system
	IFileSystem* 				fileSystem;				///< file system
	//IRenderSystem* 			renderSystem;			///< render system
	//IRenderModelManager* 		renderModelManager;		///< render model manager
	//IUserInterfaceManager* 	uiManager;				///< user interface manager
	IDeclManager* 				declManager;			///< declaration manager
	//IAASFileManager* 			AASFileManager;			///< AAS file manager
	//ICollisionModelManager* 	collisionModelManager;	///< collision model manager
};

struct soundExport_t
{
	int							version;				///< API version
	ISoundSystem* 				soundSystem;			///< sound system
};

extern "C" using GetSoundAPI_t = soundExport_t *(*)( soundImport_t* import );

}; // namespace sbe