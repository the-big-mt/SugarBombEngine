/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

#pragma once

//#include "idlib/geometry/DrawVert.h"  // for idDrawVert
#include "idlib/sys/sys_defines.h"

// TODO: triIndex_t

//namespace BFG
//{

/*
================================================================================================

	Buffer Objects

================================================================================================
*/

struct ISystem;

class idIndexBuffer;
class idDrawVert;

enum bufferMapType_t
{
	BM_READ, // map for reading
	BM_WRITE // map for writing
};

// Returns all targets to virtual memory use instead of buffer object use.
// Call this before doing any conventional buffer reads, like screenshots.
void UnbindBufferObjects();

/*
================================================
idVertexBuffer
================================================
*/
class SbVertexBuffer
{
public:
	SbVertexBuffer(ISystem &aSystem);
	~SbVertexBuffer();

	// Allocate or free the buffer.
	bool AllocBufferObject(const void *data, int allocSize);
	void FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void Reference(const SbVertexBuffer &other);
	void Reference(const SbVertexBuffer &other, int refOffset, int refSize);

	// Copies data to the buffer. 'size' may be less than the originally allocated size.
	void Update(const void *data, int updateSize) const;

	void *MapBuffer(bufferMapType_t mapType) const;
	SbDrawVert *MapVertexBuffer(bufferMapType_t mapType) const {return static_cast<SbDrawVert *>(MapBuffer(mapType));}
	void UnmapBuffer() const;
	bool IsMapped() const {return (size & MAPPED_FLAG) != 0;}

	int GetSize() const {return (size & ~MAPPED_FLAG);}
	int GetAllocedSize() const {return ((size & ~MAPPED_FLAG) + 15) & ~15;}
	void *GetAPIObject() const {return apiObject;}
	int GetOffset() const {return (offsetInOtherBuffer & ~OWNS_BUFFER_FLAG);}
private:
	int size;                // size in bytes
	int offsetInOtherBuffer; // offset in bytes
	void *apiObject;

	// sizeof() confuses typeinfo...
	static const int MAPPED_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
	static const int OWNS_BUFFER_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
private:
	void ClearWithoutFreeing();
	void SetMapped() const {const_cast<int &>(size) |= MAPPED_FLAG;}
	void SetUnmapped() const {const_cast<int &>(size) &= ~MAPPED_FLAG;}
	bool OwnsBuffer() const {return ((offsetInOtherBuffer & OWNS_BUFFER_FLAG) != 0);}

	DISALLOW_COPY_AND_ASSIGN(SbVertexBuffer);
private:
	ISystem &mSystem;
};

/*
================================================
idIndexBuffer
================================================
*/
class SbIndexBuffer
{
public:
	SbIndexBuffer(ISystem &aSystem);
	~SbIndexBuffer();

	// Allocate or free the buffer.
	bool AllocBufferObject(const void *data, int allocSize);
	void FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void Reference(const SbIndexBuffer &other);
	void Reference(const SbIndexBuffer &other, int refOffset, int refSize);

	// Copies data to the buffer. 'size' may be less than the originally allocated size.
	void Update(const void *data, int updateSize) const;

	void *MapBuffer(bufferMapType_t mapType) const;
	triIndex_t *MapIndexBuffer(bufferMapType_t mapType) const {return static_cast<triIndex_t *>(MapBuffer(mapType));}
	void UnmapBuffer() const;
	bool IsMapped() const {return (size & MAPPED_FLAG) != 0;}

	int GetSize() const {return (size & ~MAPPED_FLAG);}
	int GetAllocedSize() const {return ((size & ~MAPPED_FLAG) + 15) & ~15;}
	void *GetAPIObject() const {return apiObject;}
	int GetOffset() const {return (offsetInOtherBuffer & ~OWNS_BUFFER_FLAG);}
private:
	int size;                // size in bytes
	int offsetInOtherBuffer; // offset in bytes
	void *apiObject;

	// sizeof() confuses typeinfo...
	static const int MAPPED_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
	static const int OWNS_BUFFER_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
private:
	void ClearWithoutFreeing();
	void SetMapped() const {const_cast<int &>(size) |= MAPPED_FLAG;}
	void SetUnmapped() const {const_cast<int &>(size) &= ~MAPPED_FLAG;}
	bool OwnsBuffer() const {return ((offsetInOtherBuffer & OWNS_BUFFER_FLAG) != 0);}

	DISALLOW_COPY_AND_ASSIGN(SbIndexBuffer);
private:
	ISystem &mSystem;
};

/*
================================================
idJointBuffer

IMPORTANT NOTICE: on the PC, binding to an offset in uniform buffer objects
is limited to GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, which is 256 on current nvidia cards,
so joint offsets, which are multiples of 48 bytes, must be in multiples of 16 = 768 bytes.
================================================
*/
class SbJointBuffer
{
public:
	SbJointBuffer(ISystem &aSystem);
	~SbJointBuffer();

	// Allocate or free the buffer.
	bool AllocBufferObject(const float *joints, int numAllocJoints);
	void FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void Reference(const SbJointBuffer &other);
	void Reference(const SbJointBuffer &other, int jointRefOffset, int numRefJoints);

	// Copies data to the buffer. 'numJoints' may be less than the originally allocated size.
	void Update(const float *joints, int numUpdateJoints) const;

	float *MapBuffer(bufferMapType_t mapType) const;
	void UnmapBuffer() const;
	bool IsMapped() const {return (numJoints & MAPPED_FLAG) != 0;}

	int GetNumJoints() const {return (numJoints & ~MAPPED_FLAG);}
	int GetAllocedSize() const {return (numJoints & ~MAPPED_FLAG) * 3 * 4 * sizeof(float);}
	void *GetAPIObject() const {return apiObject;}
	int GetOffset() const {return (offsetInOtherBuffer & ~OWNS_BUFFER_FLAG);}

	void Swap(SbJointBuffer &other);
private:
	int numJoints;
	int offsetInOtherBuffer; // offset in bytes
	void *apiObject;

	// sizeof() confuses typeinfo...
	static const int MAPPED_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
	static const int OWNS_BUFFER_FLAG = 1 << (4 /* sizeof( int ) */ * 8 - 1);
private:
	void ClearWithoutFreeing();
	
	void SetMapped() const {const_cast<int &>(numJoints) |= MAPPED_FLAG;}
	void SetUnmapped() const {const_cast<int &>(numJoints) &= ~MAPPED_FLAG;}
	bool OwnsBuffer() const {return ((offsetInOtherBuffer & OWNS_BUFFER_FLAG) != 0);}

	DISALLOW_COPY_AND_ASSIGN(SbJointBuffer);
private:
	ISystem &mSystem;
};

//} // namespace sbe