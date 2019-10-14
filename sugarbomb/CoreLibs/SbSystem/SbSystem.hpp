/// @file

#pragma once

#include "CoreLibs/SbSystem/ISystem.hpp"

namespace sbe::SbSystem
{

class SbSystem : public ISystem
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // sbe::SbSystem