/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 2014-2016 Robert Beckebans
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

#pragma once

#include <cstdint>

#include "idlib/Str.h"
#include "idlib/containers/List.h"

//namespace sbe
//{

class idImage;

static const int MAX_SHADOWMAP_RESOLUTIONS = 5;
static const int MAX_BLOOM_BUFFERS = 2;
static const int MAX_SSAO_BUFFERS = 2;
static const int MAX_HIERARCHICAL_ZBUFFERS = 6; // native resolution + 5 MIP LEVELS

#if 1
static int shadowMapResolutions[MAX_SHADOWMAP_RESOLUTIONS] = { 2048, 1024, 512, 512, 256 };
#else
static int shadowMapResolutions[MAX_SHADOWMAP_RESOLUTIONS] = { 1024, 1024, 1024, 1024, 1024 };
#endif

struct idCommon;
struct idCmdSystem;

class Framebuffer
{
public:
	Framebuffer(const char *name, int width, int height, idCommon *apCommon);
	virtual ~Framebuffer();

	static void Init(idCommon *apCommon, idCmdSystem *cmdSystem);
	static void Shutdown();

	static void CheckFramebuffers();

	// deletes OpenGL object but leaves structure intact for reloading
	void PurgeFramebuffer();

	void Bind();
	bool IsBound();
	static void Unbind();
	static bool IsDefaultFramebufferActive();

	void AddColorBuffer(int format, int index, int multiSamples = 0);
	void AddDepthBuffer(int format, int multiSamples = 0);

	void AttachImage2D(int target, const idImage *image, int index, int mipmapLod = 0);
	void AttachImage3D(const idImage *image);
	void AttachImageDepth(int target, const idImage *image);
	void AttachImageDepthLayer(const idImage *image, int layer);

	// check for OpenGL errors
	void Check();
	uint32_t GetFramebuffer() const
	{
		return frameBuffer;
	}

	int GetWidth() const
	{
		return width;
	}

	int GetHeight() const
	{
		return height;
	}

	bool IsMultiSampled() const
	{
		return msaaSamples;
	}

	void Resize(int width_, int height_)
	{
		width = width_;
		height = height_;
	}

private:
	idStr fboName;

	// FBO object
	uint32_t frameBuffer;

	uint32_t colorBuffers[16];
	int colorFormat;

	uint32_t depthBuffer;
	int depthFormat;

	uint32_t stencilBuffer;
	int stencilFormat;

	int width;
	int height;

	bool msaaSamples;

	static idList<Framebuffer *> framebuffers;

private:
	idCommon *common{ nullptr };
};

struct globalFramebuffers_t
{
	Framebuffer *shadowFBO[MAX_SHADOWMAP_RESOLUTIONS];
	Framebuffer *hdrFBO;
#if defined(USE_HDR_MSAA)
	Framebuffer *hdrNonMSAAFBO;
#endif
	//	Framebuffer*				hdrQuarterFBO;
	Framebuffer *hdr64FBO;
	Framebuffer *bloomRenderFBO[MAX_BLOOM_BUFFERS];
	Framebuffer *ambientOcclusionFBO[MAX_SSAO_BUFFERS];
	Framebuffer *csDepthFBO[MAX_HIERARCHICAL_ZBUFFERS];
	//	Framebuffer*				geometryBufferFBO;
	Framebuffer *smaaEdgesFBO;
	Framebuffer *smaaBlendFBO;
};
extern globalFramebuffers_t globalFramebuffers;

//} // namespace sbe