/// @file
/// @brief F3BFG server application

#pragma once

#include "SbClientApp/SbClientApp.hpp"

namespace sbe
{
struct IGameFramework;
};

namespace f3bfg
{

class CServerApp final : public SbClientApp
{
public:
	CClientApp(int argc, const char * const *argv);
	~CClientApp();
private:
	void PostInit() override;
	
	void PostFrame() override;
	
	void LogicUpdate() override;
	
	void InitFrameworkModule();
	void ShutdownFrameworkModule();
	
	sbe::IGameFramework *mpFramework{nullptr};
	
	int frameworkDLL{0};
};

}; // namespace f3bfg