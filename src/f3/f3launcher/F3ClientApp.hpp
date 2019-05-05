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
	
	void InitSoundSystem();
	void ShutdownSoundSystem();
	
	sbe::ICommon *mpCommon{nullptr};
	
	int soundDLL{0};
};

}; // namespace f3bfg