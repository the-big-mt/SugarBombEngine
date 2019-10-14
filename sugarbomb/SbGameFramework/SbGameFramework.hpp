/// @file

#pragma once

#include "SbGameFramework/IGameFramework.hpp"

namespace sbe::SbGameFramework
{

class SbGameFramework : public IGameFramework
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // namespace sbe::SbGameFramework