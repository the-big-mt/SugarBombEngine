/// @file

#include <stdexcept>

#include "GameApp.hpp"

#include "SbGameFramework/IGameFramework.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

namespace f3goaty
{

CGameApp::CGameApp(int argc, char **argv) : SbClientApp(argc, argv)
{
	LoadFrameworkModule();
	
	mpFramework->Init();
	//if(!mpFramework->Init())
		//throw std::runtime_error("Couldn't initialize the game framework!");
};

CGameApp::~CGameApp()
{
	mpFramework->Shutdown();
	
	UnloadFrameworkModule();
};

void CGameApp::LoadFrameworkModule()
{
	mnFrameworkLib = mpSystem->LoadLib("SbGameFramework");
	
	if(!mnFrameworkLib)
		throw std::runtime_error("Failed to load the game framework module!");
	
	using fnGetGameFrameworkAPI = sbe::IGameFramework *(*)(sbe::ISystem *apSystem);
	fnGetGameFrameworkAPI pfnGetGameFrameworkAPI{mpSystem->GetLibSymbol<fnGetGameFrameworkAPI>(mnFrameworkLib, "GetGameFrameworkAPI")};
	
	if(!pfnGetGameFrameworkAPI)
		throw std::runtime_error("No \"GetGameFrameworkAPI\" exported symbol found inside the game framework module! Did you forget to export it?");
	
	mpFramework = pfnGetGameFrameworkAPI(mpSystem);
	
	if(!mpFramework)
		throw std::runtime_error("Couldn't get a valid pointer to the game framework interface!");
};

void CGameApp::UnloadFrameworkModule()
{
	mpSystem->FreeLib(mnFrameworkLib);
};

}; // namespace f3goaty