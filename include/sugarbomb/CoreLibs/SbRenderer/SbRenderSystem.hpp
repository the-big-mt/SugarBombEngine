/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020, 2023 SugarBombEngine Developers

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

#pragma once

//*****************************************************************************

namespace sbe
{

struct SbRenderWorld;
struct SbWindow;

struct SbRenderSystem
{
	/// Set up cvars and basic data structures, but don't
	/// init OpenGL, so it can also be used for dedicated servers
	// BP: why dedicated servers even need this module for? model caching shouldn't be here anyway...
	virtual void Init(const SbWindow &aWindow) = 0; // TODO: IRender(ing)Surface? // BP: Also might be better having this as pointer instead 
																				 // for offscreen rendering support (we don't need that atm, though)
	
	/// Only called before quitting
	virtual void Shutdown() = 0;
	
	/// Allocate a render world to be used for drawing
	virtual SbRenderWorld *AllocWorld() = 0;
	
	///
	virtual void FreeWorld(SbRenderWorld *apWorld) = 0;
	
	///
	virtual void BeginLevelLoad() = 0;
	
	///
	virtual void EndLevelLoad() = 0;
	
	enum class Stereo3DMode : int
	{
		Off,
		
		/// Half-resolution, non-square pixel views
		SideBySideCompressed,
		TopAndBottomCompressed,
		
		/// Two full resolution views side by side, as for a dual cable display
		SideBySide,
		
		Interlaced,
		
		/// OpenGL quad buffer
		QuadBuffer,
		
		/// Two full resolution views stacked with a 30 pixel guard band
		/// On the PC this can be configured as a custom video timing, but
		/// it definitely isn't a consumer level task.  The quad_buffer
		/// support can handle 720P-3D with apropriate driver support.
		HDMI720
	};

	///
	virtual void EnableStereoScopicRendering(const Stereo3DMode mode) /*const*/ = 0;
	
	/// GetWidth() / GetHeight() return the size of a single eye
	/// view, which may be replicated twice in a stereo display
	virtual Stereo3DMode GetStereo3DMode() const = 0;
	virtual Stereo3DMode GetStereoScopicRenderingMode() const = 0;
	
	///
	virtual bool IsStereoScopicRenderingSupported() const = 0;
	
	///
	virtual bool HasQuadBufferSupport() const = 0;
	
	///
	virtual void SetShowBuffers(const bool abShow) = 0;
};

}; // namespace sbe