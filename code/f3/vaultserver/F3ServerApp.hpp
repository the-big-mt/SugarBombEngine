/// @file
/// @brief F3BFG server application

#pragma once

#include "SbApplication/SbApplication.hpp"

namespace sbe
{
struct IGameFramework;
};

namespace f3bfg
{

class CServerApp final : public SbApplication
{
public:
	CServerApp(int argc, const char * const *argv);
	~CServerApp();
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