/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
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

/// @file

//*****************************************************************************

#include "SbBufferObject.hpp"
#include "SbRenderSystem.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

/*
================================================================================================

	idJointBuffer

================================================================================================
*/

/*
========================
idJointBuffer::idJointBuffer
========================
*/
SbJointBuffer::SbJointBuffer(SbRenderSystem &aRenderSystem, ISystem &aSystem) : mRenderSystem(aRenderSystem), mSystem(aSystem)
{
	numJoints = 0;
	offsetInOtherBuffer = OWNS_BUFFER_FLAG;
	apiObject = nullptr;
	SetUnmapped();
};

/*
========================
idJointBuffer::~idJointBuffer
========================
*/
SbJointBuffer::~SbJointBuffer()
{
	FreeBufferObject();
};

/*
========================
idJointBuffer::AllocBufferObject
========================
*/
bool SbJointBuffer::AllocBufferObject(const float *joints, int numAllocJoints)
{
	assert(apiObject == nullptr);
	assert_16_byte_aligned(joints);

	if(numAllocJoints <= 0)
		mSystem.Error("idJointBuffer::AllocBufferObject: joints = %i", numAllocJoints);

	numJoints = numAllocJoints;

	bool allocationFailed = false;

	const int numBytes = GetAllocedSize();

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferData(GL_UNIFORM_BUFFER, numBytes, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	apiObject = reinterpret_cast<void *>(buffer);

	if(mRenderSystem.mbShowBuffers) // TODO: was if(r_showBuffers.GetBool())
		mSystem.Printf("joint buffer alloc %p, api %p (%i joints)\n", this, GetAPIObject(), GetNumJoints());

	// copy the data
	if(joints != nullptr)
		Update(joints, numAllocJoints);

	return !allocationFailed;
};

/*
========================
idJointBuffer::FreeBufferObject
========================
*/
void SbJointBuffer::FreeBufferObject()
{
	if(IsMapped())
		UnmapBuffer();

	// if this is a sub-allocation inside a larger buffer, don't actually free anything.
	if(OwnsBuffer() == false)
	{
		ClearWithoutFreeing();
		return;
	};

	if(apiObject == nullptr)
		return;

	if(mRenderSystem.mbShowBuffers) // TODO: if(r_showBuffers.GetBool())
		mSystem.Printf("joint buffer free %p, api %p (%i joints)\n", this, GetAPIObject(), GetNumJoints());

	// RB: 64 bit fixes, changed GLuint to GLintptrARB
	GLintptr buffer = reinterpret_cast<GLintptr>(apiObject);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDeleteBuffers(1, (const GLuint *)&buffer);
	// RB end

	ClearWithoutFreeing();
};

/*
========================
idJointBuffer::Reference
========================
*/
void SbJointBuffer::Reference(const SbJointBuffer &other)
{
	assert(IsMapped() == false);
	assert(other.IsMapped() == false);
	assert(other.GetAPIObject() != nullptr);
	assert(other.GetNumJoints() > 0);

	FreeBufferObject();
	numJoints = other.GetNumJoints();        // this strips the MAPPED_FLAG
	offsetInOtherBuffer = other.GetOffset(); // this strips the OWNS_BUFFER_FLAG
	apiObject = other.apiObject;
	assert(OwnsBuffer() == false);
};

/*
========================
idJointBuffer::Reference
========================
*/
void SbJointBuffer::Reference(const SbJointBuffer &other, int jointRefOffset, int numRefJoints)
{
	assert(IsMapped() == false);
	assert(other.IsMapped() == false);
	assert(other.GetAPIObject() != nullptr);
	assert(jointRefOffset >= 0);
	assert(numRefJoints >= 0);
	assert(jointRefOffset + numRefJoints * sizeof(idJointMat) <= other.GetNumJoints() * sizeof(idJointMat));
	assert_16_byte_aligned(numRefJoints * 3 * 4 * sizeof(float));

	FreeBufferObject();
	numJoints = numRefJoints;
	offsetInOtherBuffer = other.GetOffset() + jointRefOffset;
	apiObject = other.apiObject;
	assert(OwnsBuffer() == false);
};

/*
========================
idJointBuffer::Update
========================
*/
void SbJointBuffer::Update(const float *joints, int numUpdateJoints) const
{
	assert(apiObject != nullptr);
	assert(IsMapped() == false);
	assert_16_byte_aligned(joints);
	assert((GetOffset() & 15) == 0);

	if(numUpdateJoints > numJoints)
		mSystem.FatalError("idJointBuffer::Update: size overrun, %i > %i\n", numUpdateJoints, numJoints);

	const int numBytes = numUpdateJoints * 3 * 4 * sizeof(float);

	// RB: 64 bit fixes, changed GLuint to GLintptrARB
	glBindBuffer(GL_UNIFORM_BUFFER, reinterpret_cast<GLintptr>(apiObject));
	// RB end

	glBufferSubData(GL_UNIFORM_BUFFER, GetOffset(), (GLsizeiptr)numBytes, joints);
};

/*
========================
idJointBuffer::MapBuffer
========================
*/
float *SbJointBuffer::MapBuffer(bufferMapType_t mapType) const
{
	assert(IsMapped() == false);
	assert(mapType == BM_WRITE);
	assert(apiObject != nullptr);

	int numBytes = GetAllocedSize();

	void *buffer = nullptr;

	// RB: 64 bit fixes, changed GLuint to GLintptrARB
	glBindBuffer(GL_UNIFORM_BUFFER, reinterpret_cast<GLintptr>(apiObject));
	// RB end

	numBytes = numBytes;
	assert(GetOffset() == 0);
	//buffer = glMapBufferARB( GL_UNIFORM_BUFFER, GL_WRITE_ONLY_ARB );

	// RB: removed GL_MAP_INVALIDATE_RANGE_BIT as it breaks with an optimization in the Nvidia WHQL drivers >= 344.11
	buffer = glMapBufferRange(GL_UNIFORM_BUFFER, 0, GetAllocedSize(), GL_MAP_WRITE_BIT /*| GL_MAP_INVALIDATE_RANGE_BIT*/ | GL_MAP_UNSYNCHRONIZED_BIT);
	if(buffer != nullptr)
		buffer = (byte *)buffer + GetOffset();

	SetMapped();

	if(buffer == nullptr)
		mSystem.FatalError("idJointBuffer::MapBuffer: failed");

	return (float *)buffer;
};

/*
========================
idJointBuffer::UnmapBuffer
========================
*/
void SbJointBuffer::UnmapBuffer() const
{
	assert(apiObject != nullptr);
	assert(IsMapped());

	// RB: 64 bit fixes, changed GLuint to GLintptrARB
	glBindBuffer(GL_UNIFORM_BUFFER, reinterpret_cast<GLintptr>(apiObject));
	// RB end

	if(!glUnmapBuffer(GL_UNIFORM_BUFFER))
		mSystem.Printf("idJointBuffer::UnmapBuffer failed\n");

	SetUnmapped();
};

/*
========================
idJointBuffer::ClearWithoutFreeing
========================
*/
void SbJointBuffer::ClearWithoutFreeing()
{
	numJoints = 0;
	offsetInOtherBuffer = OWNS_BUFFER_FLAG;
	apiObject = nullptr;
};

/*
========================
idJointBuffer::Swap
========================
*/
void SbJointBuffer::Swap(SbJointBuffer &other)
{
	// Make sure the ownership of the buffer is not transferred to an unintended place.
	assert(other.OwnsBuffer() == OwnsBuffer());

	SwapValues(other.numJoints, numJoints);
	SwapValues(other.offsetInOtherBuffer, offsetInOtherBuffer);
	SwapValues(other.apiObject, apiObject);
};

}; // namespace sbe::SbGLCoreRenderer