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
	mpFramework->Init();
	//if(!mpFramework->Init())
		//throw std::runtime_error("Couldn't initialize the game framework!");
};

CGameApp::~CGameApp()
{
	mpFramework->Shutdown();
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

{
};

{
};

}; // namespace f3goaty