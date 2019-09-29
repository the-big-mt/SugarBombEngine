/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Daniel Gibson
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

/// @file

/*
 * DG: a stub to get d3 bfg to compile without XAudio2, because that doesn't work with MinGW
 * or on non-Windows platforms.
 *
 * Please note that many methods are *not* virtual, so just inheriting from the stubs for the
 * actual implementations may *not* work!
 * (Making them virtual should be evaluated for performance-loss though, it would make the code
 *  cleaner and may be feasible)
 */

#pragma once

#include "idlib/containers/List.h"
#include "idlib/Str.h"
#include "idlib/sys/sys_defines.h" // TIME_T

#include "SbSoundVoice.hpp"
#include "SbWaveFile.hpp"

//namespace sbe
//{

struct ISys;
struct IFileSystem;

// ok, this one isn't really a stub, because it seems to be XAudio-independent,
// I just copied the class from idSoundSample_XAudio2 and renamed it
class SbSoundSample
{
public:
	idSoundSample(ISys *apSys, IFileSystem *apFileSystem);
	
	~SbSoundSample(); // destructor should be public so lists of  soundsamples can be destroyed etc
	
	// Loads and initializes the resource based on the name.
	virtual void	 LoadResource();
	
	void			SetName( const char* n )
	{
		name = n;
	}
	const char* 	GetName() const
	{
		return name;
	}
	ID_TIME_T		GetTimestamp() const
	{
		return timestamp;
	}
	
	// turns it into a beep
	void			MakeDefault();
	
	// frees all data
	void			FreeData();
	
	int				LengthInMsec() const
	{
		return SamplesToMsec( NumSamples(), SampleRate() );
	}
	int				SampleRate() const
	{
		return format.basic.samplesPerSec;
	}
	int				NumSamples() const
	{
		return playLength;
	}
	int				NumChannels() const
	{
		return format.basic.numChannels;
	}
	int				BufferSize() const
	{
		return totalBufferSize;
	}
	
	bool			IsCompressed() const
	{
		return ( format.basic.formatTag != SbWaveFile::FORMAT_PCM );
	}
	
	bool			IsDefault() const
	{
		return timestamp == FILE_NOT_FOUND_TIMESTAMP;
	}
	bool			IsLoaded() const
	{
		return loaded;
	}
	
	void			SetNeverPurge()
	{
		neverPurge = true;
	}
	bool			GetNeverPurge() const
	{
		return neverPurge;
	}
	
	void			SetLevelLoadReferenced()
	{
		levelLoadReferenced = true;
	}
	void			ResetLevelLoadReferenced()
	{
		levelLoadReferenced = false;
	}
	bool			GetLevelLoadReferenced() const
	{
		return levelLoadReferenced;
	}
	
	int				GetLastPlayedTime() const
	{
		return lastPlayedTime;
	}
	void			SetLastPlayedTime( int t )
	{
		lastPlayedTime = t;
	}
	
	float			GetAmplitude( int timeMS ) const;
	
protected:

	/*
		friend class SbSoundHardware_XAudio2;
		friend class SbSoundVoice_XAudio2;
	*/
	
	bool			LoadWav( const idStr& name );
	bool			LoadAmplitude( const idStr& name );
	void			WriteAllSamples( const idStr& sampleName );
	bool			LoadGeneratedSample( const idStr& name );
	void			WriteGeneratedSample( idFile* fileOut );
	
	struct sampleBuffer_t
	{
		void* buffer;
		int bufferSize;
		int numSamples;
	};
	
	idStr			name;
	
	ID_TIME_T		timestamp;
	bool			loaded;
	
	bool			neverPurge;
	bool			levelLoadReferenced;
	bool			usesMapHeap;
	
	uint32			lastPlayedTime;
	
	int				totalBufferSize;	// total size of all the buffers
	idList<sampleBuffer_t, TAG_AUDIO> buffers;
	
	int				playBegin;
	int				playLength;
	
	SbWaveFile::waveFmt_t	format;
	
	idList<byte, TAG_AMPLITUDE> amplitude;
private:
	ISys *mpSys{nullptr};
	IFileSystem *fileSystem{nullptr};
};

class SbSoundVoice : public SbSoundVoice_Base
{
public:
	void					Create( const SbSoundSample* leadinSample, const SbSoundSample* loopingSample ) {}
	
	// Start playing at a particular point in the buffer.  Does an Update() too
	void					Start( int offsetMS, int ssFlags ) {}
	
	// Stop playing.
	void					Stop() {}
	
	// Stop consuming buffers
	void					Pause() {}
	// Start consuming buffers again
	void					UnPause() {}
	
	// Sends new position/volume/pitch information to the hardware
	bool					Update()
	{
		return false;
	}
	
	// returns the RMS levels of the most recently processed block of audio, SSF_FLICKER must have been passed to Start
	float					GetAmplitude()
	{
		return 0.0f;
	}
	
	// returns true if we can re-use this voice
	bool					CompatibleFormat( SbSoundSample* s )
	{
		return false;
	}
	
	uint32					GetSampleRate() const
	{
		return 0;
	}
	
	// callback function
	void					OnBufferStart( SbSoundSample* sample, int bufferNumber ) {}
};

class idSoundHardware
{
public:
	idSoundHardware() {}
	
	void			Init() {}
	void			Shutdown() {}
	
	void 			Update() {}
	
	// FIXME: this is a bad name when having multiple sound backends... and maybe it's not even needed
	void* 		GetIXAudio2() const // NOTE: originally this returned IXAudio2*, but that was casted to void later anyway
	{
		return NULL;
	}
	
	idSoundVoice* 	AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample )
	{
		return NULL;
	}
	
	void			FreeVoice( SbSoundVoice* voice ) {}
	
	int				GetNumZombieVoices() const
	{
		return 0;
	}
	
	int				GetNumFreeVoices() const
	{
		return 0;
	}
	
};

//} // namespace sbe