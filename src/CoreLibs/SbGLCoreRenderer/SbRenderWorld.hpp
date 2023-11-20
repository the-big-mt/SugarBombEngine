/*
*******************************************************************************

Copyright (C) 2008-2018 OpenMW contributors
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

#include "CoreLibs/SbRenderer/IRenderWorld.hpp"

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

class SbRenderWorld final : public IRenderWorld
{
public:
	void SetSunDirection(const osg::Vec3f &direction) override;
	void SetSunColour(const osg::Vec4f &diffuse, const osg::Vec4f &specular) override;

	void ConfigureAmbient(const ESM::Cell *cell) override;
	
	void ConfigureFog(const ESM::Cell *cell) override;
	void ConfigureFog(float fogDepth, float underwaterFog, float dlFactor, float dlOffset, const osg::Vec4f &colour) override;
	
	void AddCell(const MWWorld::CellStore *store) override;
	void RemoveCell(const MWWorld::CellStore *store) override;
private:
	void SetFogColor(const osg::Vec4f& color);
private:
	osg::Vec4f mFogColor;
	osg::Vec4f mAmbientColor;
};

}; // namespace sbe::SbGLCoreRenderer