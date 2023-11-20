
/// @file
/// @brief F3BFG client application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

namespace f3bfg
{

class CClientApp final : public SbClientApp
{
public:
	CClientApp(int argc, const char * const *argv);
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	void LogicUpdate() override;
	
	void InitSoundSystem();
	void ShutdownSoundSystem();
	
	void InitFrameworkModule();
	void ShutdownFrameworkModule();
};

}; // namespace f3bfg