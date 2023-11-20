/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

#include <list> // TODO: temp

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbGLCoreRenderer
{

class SbRenderWorld;
using tRenderWorldList = std::list<SbRenderWorld*>; // TODO: SbList

class SbRenderSystem : public IRenderSystem
{
public:
	SbRenderSystem(const IWindow &aWindow, ISystem &aSystem);
	virtual ~SbRenderSystem() = default;
	
	//ISystem &GetSystem() const {return mSystem;} // TODO
	
	bool mbShowBuffers{false};
public:	
	// External functions
	
	void Init() override;
	void Shutdown() override;
	
	void Restart() override;
	
	//void EndFrame();
	
	IRenderWorld *AllocWorld() override;
	void FreeWorld(IRenderWorld *apWorld) override;
	
	void BeginLevelLoad() override;
	void EndLevelLoad() override;
	
	void EnableStereoScopicRendering(const Stereo3DMode aeMode) override {}
	
	Stereo3DMode GetStereo3DMode() const override {return Stereo3DMode::Off;}
	Stereo3DMode GetStereoScopicRenderingMode() const override {return Stereo3DMode::Off;}
	
	bool IsStereoScopicRenderingSupported() const override {return false;}
	
	bool HasQuadBufferSupport() const override {return false;}
	
	void SetShowBuffers(bool abShow) override {mbShowBuffers = abShow;}
protected:
	virtual bool GLimp_Init(glimpParms_t parms);
	virtual void GLimp_Shutdown();
	
	virtual void GLimp_SwapBuffers();
	
	virtual void GLimp_SetGamma(unsigned short anRed[256], unsigned short anGreen[256], unsigned short anBlue[256]);
private:
	void Clear();
	
	void InitOpenGL(); // TODO: was public
	void ShutdownOpenGL(); // TODO: was public
	
	bool IsInitialized() const {return mbInitialized;}
private:
	tRenderWorldList mlstWorlds;
	
	glimpParms_t glConfig{};
	
	ISystem &mSystem;
	const IWindow &mWindow;
	
	SbRenderWorld *mpPrimaryWorld{nullptr};
	SbRenderContext *mpContext{nullptr};
	
	bool mbInitialized{false};
};

};}; // namespace sbe::SbGLCoreRenderer