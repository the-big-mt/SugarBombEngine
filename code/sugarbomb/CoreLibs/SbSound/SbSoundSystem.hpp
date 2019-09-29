/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2018-2019 SugarBombEngine Developers

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

/// @file

#pragma once

#include "CoreLibs/SbSound/ISoundSystem.hpp"
#include "snd_local.h" // for idSoundHardware

#include "idlib/Thread.h"
#include "idlib/containers/StaticList.h"
#include "idlib/containers/List.h"
#include "idlib/containers/HashIndex.h"
#include "idlib/math/Random.h"

#include "framework/CVar.hpp"

//namespace sbe
//{

// We may need up to 3 buffers for each hardware voice if they are all long sounds
constexpr auto MAX_SOUND_BUFFERS{MAX_HARDWARE_VOICES * 3};

class SbSoundSample;
class SbSoundVoice;

class SbSoundWorldLocal;

#if defined(USE_OPENAL)
	class idSoundVoice_OpenAL;
	class idSoundSample_OpenAL;
#elif defined(_MSC_VER) // XAudio backend
	class idSoundVoice_XAudio2;
	class idSoundSample_XAudio2;
#endif // _MSC_VER ; DG end

class SbSoundSystemLocal : public idSoundSystem
{
public:
	// all non-hardware initialization
	virtual void Init() override;

	// shutdown routine
	virtual void Shutdown() override;

	virtual ISoundWorld *AllocSoundWorld(IRenderWorld *rw) override;
	virtual void FreeSoundWorld(ISoundWorld *sw) override;

	// specifying nullptr will cause silence to be played
	virtual void SetPlayingSoundWorld(ISoundWorld *soundWorld) override;

	// some tools, like the sound dialog, may be used in both the game and the editor
	// This can return nullptr, so check!
	virtual ISoundWorld *GetPlayingSoundWorld() const override;

	// sends the current playing sound world information to the sound hardware
	virtual void Render(float afTimeStep) override;

	// Mutes the SSG_MUSIC group
	virtual void MuteBackgroundMusic(bool mute) override
	{
		musicMuted = mute;
	};

	// sets the final output volume to 0
	// This should only be used when the app is deactivated
	// Since otherwise there will be problems with different subsystems muting and unmuting at different times
	virtual void SetMute(bool mute) override
	{
		muted = mute;
	};
	virtual bool IsMuted() const override
	{
		return muted;
	};

	virtual void OnReloadSound(const idDecl *sound) override;

	virtual void StopAllSounds() override;

	virtual void InitStreamBuffers() override;
	virtual void FreeStreamBuffers() override;

	virtual void *GetIXAudio2() const override; // FIXME: stupid name; get rid of this? not sure if it's really needed..

	// RB begin
	virtual void *GetOpenALDevice() const override;
	// RB end

	// for the sound level meter window
	virtual cinData_t ImageForTime(const int milliseconds, const bool waveform) const override;

	// Free all sounds loaded during the last map load
	virtual void BeginLevelLoad() override;

	// We might want to defer the loading of new sounds to this point
	virtual void EndLevelLoad() override;

	// prints memory info
	virtual void PrintMemInfo(MemInfo_t *mi) override;

	//-------------------------

	// Before a sound is reloaded, any active voices using it must
	// be stopped.  Returns true if any were playing, and should be
	// restarted after the sound is reloaded.
	void StopVoicesWithSample(const idSoundSample *const sample) override;

	void Restart() override;
	void SetNeedsRestart() override
	{
		needsRestart = true;
	};

	int SoundTime() const override;

	// may return NULL if there are no more voices left
	idSoundVoice *AllocateVoice(const idSoundSample *leadinSample, const idSoundSample *loopingSample) override;
	void FreeVoice(idSoundVoice *) override;

	idSoundSample *LoadSample(const char *name) override;

	virtual void Preload(idPreloadManifest &preload) override;

	struct bufferContext_t
	{
		bufferContext_t() = default;

#if defined(SBE_USE_OPENAL)
		SbSoundVoice_OpenAL *voice{nullptr};
		SbSoundSample_OpenAL *sample{nullptr};
#elif defined(_MSC_VER) // XAudio backend
		// DG: because the inheritance is kinda strange (idSoundVoice is derived
		// from idSoundVoice_XAudio2), casting the latter to the former isn't possible
		// so we need this ugly #ifdef ..
		SbSoundVoice_XAudio2 *voice{nullptr};
		SbSoundSample_XAudio2 *sample{nullptr};
#else // not _MSC_VER
		// from stub or something..
		SbSoundVoice *voice{nullptr};
		SbSoundSample *sample{nullptr};
#endif // _MSC_VER ; DG end

		int bufferNumber{0};
	};

	// Get a stream buffer from the free pool, returns nullptr if none are available
	bufferContext_t *ObtainStreamBufferContext() const;
	void ReleaseStreamBufferContext(bufferContext_t *p);

	idSysMutex streamBufferMutex;
	idStaticList<bufferContext_t *, MAX_SOUND_BUFFERS> freeStreamBufferContexts;
	idStaticList<bufferContext_t *, MAX_SOUND_BUFFERS> activeStreamBufferContexts;
	idStaticList<bufferContext_t, MAX_SOUND_BUFFERS> bufferContexts;

	SbSoundWorldLocal *currentSoundWorld{nullptr};
	idStaticList<SbSoundWorldLocal *, 32> soundWorlds;

	idList<SbSoundSample *, TAG_AUDIO> samples;
	idHashIndex sampleHash;

	idSoundHardware hardware;

	idRandom2 random;

	int soundTime{0};
	bool muted{false};
	bool musicMuted{false};
	bool needsRestart{false};

	bool insideLevelLoad{false};

	//-------------------------

	SbSoundSystemLocal() = default;
};

extern SbSoundSystemLocal soundSystemLocal;

//}; // namespace sbe