/// @file

#pragma once

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

namespace sbe
{
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
	void LoadFrameworkModule();
	void UnloadFrameworkModule();
	
	sbe::IGameFramework *mpFramework{nullptr};
	
	int mnFrameworkLib{-1};
};

}; // namespace f3goaty