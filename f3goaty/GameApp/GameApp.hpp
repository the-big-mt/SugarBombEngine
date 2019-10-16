/// @file

#pragma once

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct ISoundSystem;
struct IGameFramework;
};

namespace f3goaty
{

class CGameApp final : public SbClientApp
{
public:
	CGameApp(int argc, char **argv);
	~CGameApp();
private:
	void LoadSoundModule();
	void UnloadSoundModule();
	
	void LoadFrameworkModule();
	void UnloadFrameworkModule();
	
	sbe::ISoundSystem *mpSoundSystem{nullptr};
	sbe::IGameFramework *mpFramework{nullptr};
	
	int mnSoundLib{-1};
	int mnFrameworkLib{-1};
};

}; // namespace f3goaty