/// @file

#pragma once

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

namespace sbe::SbRenderer
{

class SbRenderSystem : public IRenderSystem
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // namespace sbe::SbRenderer