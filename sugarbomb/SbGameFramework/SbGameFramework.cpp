/// @file

#include "SbGameFramework.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbGame/IGame.hpp"

namespace sbe::SbGameFramework
{

SbGameFramework::SbGameFramework(ISystem &aSystem) : mSystem(aSystem){}

void SbGameFramework::Init()
{
	LoadGameModule();
	
	mpGame->Init();
};

void SbGameFramework::Shutdown()
{
	mpGame->Shutdown();
	
	UnloadGameModule();
};

void SbGameFramework::LoadGameModule()
{
	mnGameLib = mSystem.get().LoadLib("SbGame");
	
	if(!mnGameLib)
		return;
	
	using fnGetGameAPI = IGame *(*)();
	fnGetGameAPI pfnGetGameAPI{mSystem.get().GetLibSymbol<fnGetGameAPI>(mnGameLib, "GetGameAPI")};
	
	if(!pfnGetGameAPI)
		return;
	
	mpGame = pfnGetGameAPI();
};

void SbGameFramework::UnloadGameModule()
{
	mSystem.get().FreeLib(mnGameLib);
};

}; // namespace sbe::SbGameFramework