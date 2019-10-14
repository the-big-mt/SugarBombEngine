/// @file

#pragma once

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

namespace sbe::SbInput
{

class SbInputSystem : public IInputSystem
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // namespace sbe::SbInput