/// @file

#include <stdexcept>

#include "GameApp.hpp"

#include "SbGameFramework/IGameFramework.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbSound/ISoundSystem.hpp"

namespace f3goaty
{

CGameApp::CGameApp(int argc, char **argv) : SbClientApp(argc, argv)
{
	LoadSoundModule();
	
	mpSoundSystem->Init();
	
	LoadFrameworkModule();
	
	mpFramework->Init();
	//if(!mpFramework->Init())
		//throw std::runtime_error("Couldn't initialize the game framework!");
};

CGameApp::~CGameApp()
{
	mpFramework->Shutdown();
	
	UnloadFrameworkModule();
	
	mpSoundSystem->Shutdown();
	
	UnloadSoundModule();
};

void CGameApp::LoadSoundModule()
{
	mnSoundLib = mpSystem->LoadLib("SbSound");
	
	if(!mnSoundLib)
		throw std::runtime_error("Failed to load the sound module!");
	
	using fnGetSoundSystemAPI = sbe::ISoundSystem *(*)();
	fnGetSoundSystemAPI pfnGetSoundSystemAPI{mpSystem->GetLibSymbol<fnGetSoundSystemAPI>(mnSoundLib, "GetSoundSystemAPI")};
	
	if(!pfnGetSoundSystemAPI)
		throw std::runtime_error("No \"GetSoundSystemAPI\" exported symbol found inside the sound module! Did you forget to export it?");
	
	mpSoundSystem = pfnGetSoundSystemAPI();
	
	if(!mpSoundSystem)
		throw std::runtime_error("Couldn't get a valid pointer to the sound system interface!");
};

void CGameApp::UnloadSoundModule()
{
	mpSystem->FreeLib(mnSoundLib);
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