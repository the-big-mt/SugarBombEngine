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
	mpSoundSystem->Init();
	mpFramework->Init();
	//if(!mpFramework->Init())
		//throw std::runtime_error("Couldn't initialize the game framework!");
};

CGameApp::~CGameApp()
{
	mpFramework->Shutdown();
	mpSoundSystem->Shutdown();
};

{
};

{
};

}; // namespace f3goaty