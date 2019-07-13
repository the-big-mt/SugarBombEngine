/*
*******************************************************************************

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

#pragma once

#include "SbFramework/ISaveGameManager.hpp"

class SbSaveGameManager final : public sbe::ISaveGameManager
{
public:
	SbSaveGameManager();
	~SbSaveGameManager();
	
	virtual bool SaveGame(const char *asSaveName) override;
	virtual bool LoadGame(const char *asSaveName) override;
	
	virtual void OnSaveCompleted(idSaveLoadParms &aParms) override;
	virtual void OnLoadCompleted(idSaveLoadParms &aParms) override;
	virtual void OnLoadFilesCompleted(idSaveLoadParms &aParms) override;
	virtual void OnEnumerationCompleted(idSaveLoadParms &aParms) override;
	virtual void OnDeleteCompleted(idSaveLoadParms &aParms) override;
private:
	bool HandleCommonErrors(const idSaveLoadParms &aParms);
	void HandleInsufficientStorage(const idSaveLoadParms &aParms);
	
	void ScrubSaveGameFileName(idStr &aSaveFileName) const;
};