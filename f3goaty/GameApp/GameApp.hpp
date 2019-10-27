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
	sbe::ISoundSystem *mpSoundSystem{nullptr};
	sbe::IGameFramework *mpFramework{nullptr};
};

}; // namespace f3goaty