/// @file

#pragma once

#include "CoreLibs/SbSound/ISoundSystem.hpp"

namespace sbe::SbSound
{

class SbSoundSystem : public ISoundSystem
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // namespace sbe::SbSound