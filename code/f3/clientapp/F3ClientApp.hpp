/// @file
/// @brief F3BFG client application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct ICommon;
};

struct idSoundSystem;

namespace f3bfg
{

class CClientApp final : public SbClientApp
{
public:
	
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	void LogicUpdate() override;
	
	void InitSoundSystem();
	void ShutdownSoundSystem();
	
	void InitFrameworkModule();
	void ShutdownFrameworkModule();
	
	sbe::ICommon *mpCommon{nullptr};
	idSoundSystem *soundSystem{nullptr};
	
	int soundDLL{0};
	int frameworkDLL{0};
};

}; // namespace f3bfg