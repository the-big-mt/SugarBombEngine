/// @file
/// @brief F3BFG client application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

struct sbe::ICommon;

namespace f3bfg
{

class CClientApp final : public SbClientApp
{
public:
	
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	sbe::ICommon *mpCommon{nullptr};
};

}; // namespace f3bfg