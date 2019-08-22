/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
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

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

#ifndef __AL_SOUNDHARDWARE_H__
#define __AL_SOUNDHARDWARE_H__

//#ifdef __APPLE__
//#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
//#else
//#include <AL/al.h>
//#include <AL/alc.h>
//#include <AL/alext.h>
//#endif

//#include "../sound/OpenAL/AL_SoundVoice.h"  // for idSoundVoice_OpenAL

//#include "SbSoundDefines.hpp"

//namespace BFG
//{

class idSoundSample_OpenAL;
class idSoundVoice_OpenAL;
class idSoundHardware_OpenAL;

//class idSoundVoice;
//class idSoundSample;

/*
================================================
idSoundHardware_OpenAL
================================================
*/

class idSoundHardware_OpenAL
{
public:
	idSoundHardware_OpenAL();

	void Init();
	void Shutdown();

	void Update();

	idSoundVoice *AllocateVoice(const idSoundSample *leadinSample, const idSoundSample *loopingSample);
	void FreeVoice(idSoundVoice *voice);

	// listDevices needs this
	ALCdevice *GetOpenALDevice() const
	{
		return openalDevice;
	};

	int GetNumZombieVoices() const
	{
		return zombieVoices.Num();
	}
	int GetNumFreeVoices() const
	{
		return freeVoices.Num();
	}

	// OpenAL info
	static void PrintDeviceList(const char *list);
	static void PrintALCInfo(ALCdevice *device);
	static void PrintALInfo();

protected:
	friend class idSoundSample_OpenAL;
	friend class idSoundVoice_OpenAL;

private:
	/*
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SubmixVoice* pSubmixVoice;
	
	idSoundEngineCallback	soundEngineCallback;
	*/

	ALCdevice *openalDevice;
	ALCcontext *openalContext;

	int lastResetTime;

	//int				outputChannels;
	//int				channelMask;

	//idDebugGraph* 	vuMeterRMS;
	//idDebugGraph* 	vuMeterPeak;
	//int				vuMeterPeakTimes[ 8 ];

	// Can't stop and start a voice on the same frame, so we have to double this to handle the worst case scenario of stopping all voices and starting a full new set
	idStaticList<idSoundVoice_OpenAL, MAX_HARDWARE_VOICES * 2> voices;
	idStaticList<idSoundVoice_OpenAL *, MAX_HARDWARE_VOICES * 2> zombieVoices;
	idStaticList<idSoundVoice_OpenAL *, MAX_HARDWARE_VOICES * 2> freeVoices;
};

/*
================================================
idSoundHardware
================================================
*/
class idSoundHardware : public idSoundHardware_OpenAL
{
};

//} // namespace BFG

#endif // __AL_SOUNDHARDWARE_H__
