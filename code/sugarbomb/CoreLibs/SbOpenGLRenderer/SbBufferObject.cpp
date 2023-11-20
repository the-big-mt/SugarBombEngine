/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
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
#pragma hdrstop
#include "precompiled.h"

#include "framework/CVar.hpp"

//#include "RenderCommon.h"
#include "BufferObject.h"

//namespace sbe
//{

idCVar r_showBuffers("r_showBuffers", "0", CVAR_INTEGER, "");

//static const GLenum bufferUsage = GL_STATIC_DRAW;
static const GLenum bufferUsage = GL_DYNAMIC_DRAW;

// RB begin
#if defined(_WIN32)
/*
==================
IsWriteCombined
==================
*/
bool IsWriteCombined(void *base)
{
	MEMORY_BASIC_INFORMATION info;
	SIZE_T size = VirtualQueryEx(GetCurrentProcess(), base, &info, sizeof(info));
	if(size == 0)
	{
		DWORD error = GetLastError();
		error = error;
		return false;
	};
	bool isWriteCombined = ((info.AllocationProtect & PAGE_WRITECOMBINE) != 0);
	return isWriteCombined;
};
#endif
// RB end

/*
================================================================================================

	Buffer Objects

================================================================================================
*/

/*
========================
UnbindBufferObjects
========================
*/
void UnbindBufferObjects()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

#if defined(SBE_USE_INTRINSICS)

void CopyBuffer(byte *dst, const byte *src, int numBytes)
{
	assert_16_byte_aligned(dst);
	assert_16_byte_aligned(src);

	int i = 0;
	for(; i + 128 <= numBytes; i += 128)
	{
		__m128i d0 = _mm_load_si128((__m128i *)&src[i + 0 * 16]);
		__m128i d1 = _mm_load_si128((__m128i *)&src[i + 1 * 16]);
		__m128i d2 = _mm_load_si128((__m128i *)&src[i + 2 * 16]);
		__m128i d3 = _mm_load_si128((__m128i *)&src[i + 3 * 16]);
		__m128i d4 = _mm_load_si128((__m128i *)&src[i + 4 * 16]);
		__m128i d5 = _mm_load_si128((__m128i *)&src[i + 5 * 16]);
		__m128i d6 = _mm_load_si128((__m128i *)&src[i + 6 * 16]);
		__m128i d7 = _mm_load_si128((__m128i *)&src[i + 7 * 16]);
		_mm_stream_si128((__m128i *)&dst[i + 0 * 16], d0);
		_mm_stream_si128((__m128i *)&dst[i + 1 * 16], d1);
		_mm_stream_si128((__m128i *)&dst[i + 2 * 16], d2);
		_mm_stream_si128((__m128i *)&dst[i + 3 * 16], d3);
		_mm_stream_si128((__m128i *)&dst[i + 4 * 16], d4);
		_mm_stream_si128((__m128i *)&dst[i + 5 * 16], d5);
		_mm_stream_si128((__m128i *)&dst[i + 6 * 16], d6);
		_mm_stream_si128((__m128i *)&dst[i + 7 * 16], d7);
	};
	for(; i + 16 <= numBytes; i += 16)
	{
		__m128i d = _mm_load_si128((__m128i *)&src[i]);
		_mm_stream_si128((__m128i *)&dst[i], d);
	};
	for(; i + 4 <= numBytes; i += 4)
		*(uint32 *)&dst[i] = *(const uint32 *)&src[i];

	for(; i < numBytes; i++)
		dst[i] = src[i];

	_mm_sfence();
};

#else

void CopyBuffer(byte *dst, const byte *src, int numBytes)
{
	assert_16_byte_aligned(dst);
	assert_16_byte_aligned(src);
	memcpy(dst, src, numBytes);
};

#endif

//} // namespace sbe