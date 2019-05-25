/// @file
/// @brief F3BFG client application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct IGameFramework;
};

struct idSoundSystem;

namespace f3bfg
{

class CClientApp final : public SbClientApp
{
public:
	CClientApp(int argc, const char * const *argv);
	~CClientApp();
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	void LogicUpdate() override;
	
	void InitSoundSystem();
	void ShutdownSoundSystem();
	
	void InitFrameworkModule();
	void ShutdownFrameworkModule();
	
	sbe::IGameFramework *mpFramework{nullptr};
	idSoundSystem *soundSystem{nullptr};
	
	int soundDLL{0};
	int frameworkDLL{0};
};

}; // namespace f3bfg