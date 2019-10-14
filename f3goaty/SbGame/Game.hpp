/// @file

#pragma once

#include "SbGame/IGame.hpp"

namespace f3goaty
{

class CGame : public sbe::IGame
{
public:
	void Init() override;
	void Shutdown() override;
};

}; // namespace f3goaty