/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#pragma hdrstop

#include "precompiled.h"
//#include <cstring>

#include "SbWaveFile.hpp"

#include "CoreLibs/SbSystem/IFileSystem.hpp"
#include "CoreLibs/SbSystem/IFile.hpp"

#include "idlib/Heap.h"
#include "idlib/Lib.h"
#include "idlib/Swap.h"

/*
================================================================================================
Contains the WaveFile implementation
================================================================================================
*/

//namespace BFG
//{

/*
========================
idWaveFile::Open

Returns true if the Open was successful and the file matches the expected format. If this
returns false, there is no need to call Close.
========================
*/
bool SbWaveFile::Open(sbe::IFileSystem *fileSystem, const char *filename)
{
	Close(fileSystem);

	if(filename == nullptr || filename[0] == 0)
		return false;

	if(mpFile == nullptr)
	{
		mpFile = fileSystem->OpenFileReadMemory(filename);
		if(mpFile == nullptr)
			return false;
	};

	if(mpFile->Length() == 0)
	{
		Close(fileSystem);
		return false;
	};

	struct header_t
	{
		uint32 id;
		uint32 size;
		uint32 format;
	} header;

	mpFile->Read(&header, sizeof(header));
	idSwap::Big(header.id);
	idSwap::Little(header.size);
	idSwap::Big(header.format);

	if(header.id != 'RIFF' || header.format != 'WAVE' || header.size < 4)
	{
		Close(fileSystem);
		idLib::Warning("Header is not RIFF WAVE in %s", filename);
		return false;
	};

	uint32 riffSize = header.size + 8;
	uint32 offset = sizeof(header);

	// Scan the file collecting chunks
	while(offset < riffSize)
	{
		struct chuckHeader_t
		{
			uint32 id;
			uint32 size;
		} chunkHeader;
		
		if(mpFile->Read(&chunkHeader, sizeof(chunkHeader)) != sizeof(chunkHeader))
		{
			// It seems like some tools have extra data after the last chunk for no apparent reason
			// so don't treat this as an error
			return true;
		};
		
		idSwap::Big(chunkHeader.id);
		idSwap::Little(chunkHeader.size);
		offset += sizeof(chunkHeader);

		if(chunks.Num() >= chunks.Max())
		{
			Close(fileSystem);
			idLib::Warning("More than %d chunks in %s", chunks.Max(), filename);
			return false;
		};

		chunk_t *chunk = chunks.Alloc();
		chunk->id = chunkHeader.id;
		chunk->size = chunkHeader.size;
		chunk->offset = offset;
		offset += chunk->size;

		mpFile->Seek(offset, FS_SEEK_SET);
	};

	return true;
};

/*
========================
idWaveFile::SeekToChunk

Seeks to the specified chunk and returns the size of the chunk or 0 if the chunk wasn't found.
========================
*/
uint32 SbWaveFile::SeekToChunk(uint32 id)
{
	for(int i = 0; i < chunks.Num(); i++)
	{
		if(chunks[i].id == id)
		{
			mpFile->Seek(chunks[i].offset, FS_SEEK_SET);
			return chunks[i].size;
		};
	};
	return 0;
};

/*
========================
idWaveFile::GetChunkOffset

Seeks to the specified chunk and returns the size of the chunk or 0 if the chunk wasn't found.
========================
*/
uint32 SbWaveFile::GetChunkOffset(uint32 id)
{
	for(int i = 0; i < chunks.Num(); i++)
	{
		if(chunks[i].id == id)
			return chunks[i].offset;
	};
	return 0;
};

// Used in XMA2WAVEFORMAT for per-stream data
typedef struct XMA2STREAMFORMAT
{
	byte Channels;      // Number of channels in the stream (1 or 2)
	byte RESERVED;      // Reserved for future use
	uint16 ChannelMask; // Spatial positions of the channels in the stream
} XMA2STREAMFORMAT;

// Legacy XMA2 format structure (big-endian byte ordering)
typedef struct XMA2WAVEFORMAT
{
	byte Version; // XMA encoder version that generated the file.
	// Always 3 or higher for XMA2 files.
	byte NumStreams;          // Number of interleaved audio streams
	byte RESERVED;            // Reserved for future use
	byte LoopCount;           // Number of loop repetitions; 255 = infinite
	uint32 LoopBegin;         // Loop begin point, in samples
	uint32 LoopEnd;           // Loop end point, in samples
	uint32 SampleRate;        // The file's decoded sample rate
	uint32 EncodeOptions;     // Options for the XMA encoder/decoder
	uint32 PsuedoBytesPerSec; // Used internally by the XMA encoder
	uint32 BlockSizeInBytes;  // Size in bytes of this file's XMA blocks (except
	// possibly the last one).  Always a multiple of
	// 2Kb, since XMA blocks are arrays of 2Kb packets.
	uint32 SamplesEncoded;  // Total number of PCM samples encoded in this file
	uint32 SamplesInSource; // Actual number of PCM samples in the source
	// material used to generate this file
	uint32 BlockCount; // Number of XMA blocks in this file (and hence
	                   // also the number of entries in its seek table)
} XMA2WAVEFORMAT;

/*
========================
idWaveFile::ReadWaveFormat

Reads a wave format header, returns nullptr if it found one and read it.
otherwise, returns a human-readable error message.
========================
*/
const char *SbWaveFile::ReadWaveFormat(waveFmt_t &format)
{
	memset(&format, 0, sizeof(format));

	uint32 formatSize = SeekToChunk(waveFmt_t::id);
	if(formatSize == 0)
		return "No format chunk";
	if(formatSize < sizeof(format.basic))
		return "Format chunk too small";

	Read(&format.basic, sizeof(format.basic));

	idSwapClass<waveFmt_t::basic_t> swap;
	swap.Little(format.basic.formatTag);
	swap.Little(format.basic.numChannels);
	swap.Little(format.basic.samplesPerSec);
	swap.Little(format.basic.avgBytesPerSec);
	swap.Little(format.basic.blockSize);
	swap.Little(format.basic.bitsPerSample);

	if(format.basic.formatTag == FORMAT_PCM)
	{
	}
	else if(format.basic.formatTag == FORMAT_ADPCM)
	{
		Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::adpcm_t))
			return "Incorrect number of coefficients in ADPCM file";

		Read(&format.extra.adpcm, sizeof(format.extra.adpcm));
		idSwapClass<waveFmt_t::extra_t::adpcm_t> swap;
		swap.Little(format.extra.adpcm.samplesPerBlock);
		swap.Little(format.extra.adpcm.numCoef);
		for(int i = 0; i < format.extra.adpcm.numCoef; i++)
		{
			swap.Little(format.extra.adpcm.aCoef[i].coef1);
			swap.Little(format.extra.adpcm.aCoef[i].coef2);
		};
	}
	else if(format.basic.formatTag == FORMAT_XMA2)
	{
		Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::xma2_t))
			return "Incorrect chunk size in XMA2 file";

		Read(&format.extra.xma2, sizeof(format.extra.xma2));
		idSwapClass<waveFmt_t::extra_t::xma2_t> swap;
		swap.Little(format.extra.xma2.numStreams);
		swap.Little(format.extra.xma2.channelMask);
		swap.Little(format.extra.xma2.samplesEncoded);
		swap.Little(format.extra.xma2.bytesPerBlock);
		swap.Little(format.extra.xma2.playBegin);
		swap.Little(format.extra.xma2.playLength);
		swap.Little(format.extra.xma2.loopBegin);
		swap.Little(format.extra.xma2.loopLength);
		swap.Little(format.extra.xma2.loopCount);
		swap.Little(format.extra.xma2.encoderVersion);
		swap.Little(format.extra.xma2.blockCount);
	}
	else if(format.basic.formatTag == FORMAT_EXTENSIBLE)
	{
		Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::extensible_t))
			return "Incorrect chunk size in extensible wave file";

		Read(&format.extra.extensible, sizeof(format.extra.extensible));
		
		idSwapClass<waveFmt_t::extra_t::extensible_t> swap;
		swap.Little(format.extra.extensible.validBitsPerSample);
		swap.Little(format.extra.extensible.channelMask);
		swap.Little(format.extra.extensible.subFormat.data1);
		swap.Little(format.extra.extensible.subFormat.data2);
		swap.Little(format.extra.extensible.subFormat.data3);
		swap.Little(format.extra.extensible.subFormat.data4);
		swap.LittleArray(format.extra.extensible.subFormat.data5, 6);
		
		waveFmt_t::extra_t::extensible_t::guid_t pcmGuid =
		{
		  FORMAT_PCM,
		  0x0000,
		  0x0010,
		  0x8000,
		  { 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
		};
		if(memcmp(&pcmGuid, &format.extra.extensible.subFormat, sizeof(pcmGuid)) != 0)
			return "Unsupported Extensible format";
	}
	else
		return "Unknown wave format tag";

	return nullptr;
};

/*
========================
idWaveFile::ReadWaveFormatDirect

Reads a wave format header from a file ptr,
========================
*/
bool SbWaveFile::ReadWaveFormatDirect(waveFmt_t &format, sbe::IFile *file)
{
	file->Read(&format.basic, sizeof(format.basic));
	
	idSwapClass<waveFmt_t::basic_t> swap;
	swap.Little(format.basic.formatTag);
	swap.Little(format.basic.numChannels);
	swap.Little(format.basic.samplesPerSec);
	swap.Little(format.basic.avgBytesPerSec);
	swap.Little(format.basic.blockSize);
	swap.Little(format.basic.bitsPerSample);

	if(format.basic.formatTag == FORMAT_PCM)
	{
	}
	else if(format.basic.formatTag == FORMAT_ADPCM)
	{
		file->Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::adpcm_t))
			return false;

		file->Read(&format.extra.adpcm, sizeof(format.extra.adpcm));
		idSwapClass<waveFmt_t::extra_t::adpcm_t> swap;
		swap.Little(format.extra.adpcm.samplesPerBlock);
		swap.Little(format.extra.adpcm.numCoef);
		for(int i = 0; i < format.extra.adpcm.numCoef; i++)
		{
			swap.Little(format.extra.adpcm.aCoef[i].coef1);
			swap.Little(format.extra.adpcm.aCoef[i].coef2);
		};
	}
	else if(format.basic.formatTag == FORMAT_XMA2)
	{
		file->Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::xma2_t))
			return false;

		file->Read(&format.extra.xma2, sizeof(format.extra.xma2));
		idSwapClass<waveFmt_t::extra_t::xma2_t> swap;
		swap.Little(format.extra.xma2.numStreams);
		swap.Little(format.extra.xma2.channelMask);
		swap.Little(format.extra.xma2.samplesEncoded);
		swap.Little(format.extra.xma2.bytesPerBlock);
		swap.Little(format.extra.xma2.playBegin);
		swap.Little(format.extra.xma2.playLength);
		swap.Little(format.extra.xma2.loopBegin);
		swap.Little(format.extra.xma2.loopLength);
		swap.Little(format.extra.xma2.loopCount);
		swap.Little(format.extra.xma2.encoderVersion);
		swap.Little(format.extra.xma2.blockCount);
	}
	else if(format.basic.formatTag == FORMAT_EXTENSIBLE)
	{
		file->Read(&format.extraSize, sizeof(format.extraSize));
		idSwap::Little(format.extraSize);
		if(format.extraSize != sizeof(waveFmt_t::extra_t::extensible_t))
			return false;

		file->Read(&format.extra.extensible, sizeof(format.extra.extensible));
		idSwapClass<waveFmt_t::extra_t::extensible_t> swap;
		swap.Little(format.extra.extensible.validBitsPerSample);
		swap.Little(format.extra.extensible.channelMask);
		swap.Little(format.extra.extensible.subFormat.data1);
		swap.Little(format.extra.extensible.subFormat.data2);
		swap.Little(format.extra.extensible.subFormat.data3);
		swap.Little(format.extra.extensible.subFormat.data4);
		swap.LittleArray(format.extra.extensible.subFormat.data5, 6);
		waveFmt_t::extra_t::extensible_t::guid_t pcmGuid =
		{
		  FORMAT_PCM,
		  0x0000,
		  0x0010,
		  0x8000,
		  { 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
		};
		if(memcmp(&pcmGuid, &format.extra.extensible.subFormat, sizeof(pcmGuid)) != 0)
			return false;
	}
	else
		return false;

	return true;
};

/*
========================
idWaveFile::WriteWaveFormatDirect

Writes a wave format header to a file ptr,
========================
*/
bool SbWaveFile::WriteWaveFormatDirect(waveFmt_t &format, sbe::IFile *file)
{
	//idSwapClass<waveFmt_t::basic_t> swap;
	//swap.Little( format.basic.formatTag );
	//swap.Little( format.basic.numChannels );
	//swap.Little( format.basic.samplesPerSec );
	//swap.Little( format.basic.avgBytesPerSec );
	//swap.Little( format.basic.blockSize );
	//swap.Little( format.basic.bitsPerSample );
	file->Write(&format.basic, sizeof(format.basic));
	
	if(format.basic.formatTag == FORMAT_PCM)
	{
		//file->Write( &format.basic, sizeof( format.basic ) );
	}
	else if(format.basic.formatTag == FORMAT_ADPCM)
	{
		//file->Write( &format.basic, sizeof( format.basic ) );
		file->Write(&format.extraSize, sizeof(format.extraSize));
		file->Write(&format.extra.adpcm, sizeof(format.extra.adpcm));
	}
	else if(format.basic.formatTag == FORMAT_XMA2)
	{
		//file->Write( &format.basic, sizeof( format.basic ) );
		file->Write(&format.extraSize, sizeof(format.extraSize));
		file->Write(&format.extra.xma2, sizeof(format.extra.xma2));
	}
	else if(format.basic.formatTag == FORMAT_EXTENSIBLE)
	{
		//file->Write( &format.basic, sizeof( format.basic ) );
		file->Write(&format.extraSize, sizeof(format.extraSize));
		file->Write(&format.extra.extensible, sizeof(format.extra.extensible));
	}
	else
		return false;

	return true;
};

/*
========================
idWaveFile::WriteWaveFormatDirect

Writes a wave format header to a file ptr,
========================
*/
bool SbWaveFile::WriteSampleDataDirect(idList<sampleData_t> &sampleData, sbe::IFile *file)
{
	static const uint32 sample{'smpl'};
	
	file->WriteBig(sample);
	
	// TODO: magics
	uint32 samplerData{sampleData.Num() * 24};
	uint32 chunkSize{36 + samplerData};
	uint32 zero{0};
	uint32 numSamples{sampleData.Num()};

	file->Write(&chunkSize, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&zero, sizeof(uint32));
	file->Write(&numSamples, sizeof(uint32));
	file->Write(&samplerData, sizeof(uint32));

	for(int i = 0; i < sampleData.Num(); ++i)
	{
		file->Write(&zero, sizeof(uint32));
		file->Write(&zero, sizeof(uint32));
		file->Write(&sampleData[i].start, sizeof(uint32));
		file->Write(&sampleData[i].end, sizeof(uint32));
		file->Write(&zero, sizeof(uint32));
		file->Write(&zero, sizeof(uint32));
	};
	return true;
};

/*
========================
idWaveFile::WriteWaveFormatDirect

Writes a data chunk to a file ptr
========================
*/
bool SbWaveFile::WriteDataDirect(char *_data, uint32 size, sbe::IFile *file)
{
	static const uint32 data{'data'};
	
	file->WriteBig(data);
	file->Write(&size, sizeof(uint32));
	file->WriteBigArray(_data, size);
	return true;
};

/*
========================
idWaveFile::WriteWaveFormatDirect

Writes a wave header to a file ptr,
========================
*/
bool SbWaveFile::WriteHeaderDirect(uint32 fileSize, sbe::IFile *file)
{
	static const uint32 riff{'RIFF'};
	static const uint32 wave{'WAVE'};
	
	file->WriteBig(riff);
	file->WriteBig(fileSize);
	file->WriteBig(wave);
	return true;
};

/*
========================
idWaveFile::ReadLoopPoint

Reads a loop point from a 'smpl' chunk in a wave file, returns 0 if none are found.
========================
*/
bool SbWaveFile::ReadLoopData(int &start, int &end)
{
	uint32 chunkSize = SeekToChunk(samplerChunk_t::id);
	if(chunkSize < sizeof(samplerChunk_t))
		return false;

	samplerChunk_t smpl;
	Read(&smpl, sizeof(smpl));
	idSwap::Little(smpl.numSampleLoops);

	if(smpl.numSampleLoops < 1)
		return false; // this is possible returning false lets us know there are more then 1 sample look in the file and is not appropriate for traditional looping

	sampleData_t smplData;
	Read(&smplData, sizeof(smplData));
	idSwap::Little(smplData.start);
	idSwap::Little(smplData.end);

	if(smplData.type != 0)
	{
		idLib::Warning("Invalid loop type in %s", mpFile->GetName());
		return false;
	};

	start = smplData.start;
	end = smplData.end;
	return true;
};

/*
========================
idWaveFile::Close

Closes the file and frees resources.
========================
*/
void SbWaveFile::Close(sbe::IFileSystem *apFileSystem)
{
	if(mpFile != nullptr)
	{
		delete mpFile; // TODO: apFileSystem->CloseFile(mpFile);
		mpFile = nullptr;
	};
	chunks.SetNum(0);
};

//} // namespace BFG