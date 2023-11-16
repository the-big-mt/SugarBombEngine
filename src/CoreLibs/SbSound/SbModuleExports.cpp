/*
*******************************************************************************

Copyright (C) 2019, 2023 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file
/// @file SbSound module exports

//*****************************************************************************

#include "SbSoundSystemLocal.hpp"

#ifdef SBE_USE_OPENAL
#	include "openal/SbSoundHardwareOpenAL.hpp"
#else
#	include "stub/SbSoundHardwareStub.hpp"
#endif

#include <CoreLibs/SbSound/SbModuleAPI.hpp>

#ifdef _WIN32
#	define EXPORT [[dllexport]]
#else
#	define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

//*****************************************************************************

C_EXPORT sbe::soundExport_t *GetSoundAPI(sbe::soundImport_t *apModuleImports)
{
	if(apModuleImports->version == sbe::SOUND_API_VERSION)
	{
#ifdef SBE_USE_OPENAL
		static sbe::SbSound::SbSoundHardwareOpenAL SoundHardware(*apModuleImports->sys);
#else
		static sbe::SbSound::SbSoundHardwareStub SoundHardware;
#endif
		static sbe::SbSound::SbSoundSystemLocal SoundSystem(*apModuleImports->sys, SoundHardware);
		
		static sbe::soundExport_t ModuleExports;
		
		ModuleExports.version = sbe::SOUND_API_VERSION;
		ModuleExports.soundSystem = &SoundSystem;
		
		return &ModuleExports;
	};
	
	return nullptr;
};