/// @file

#include <stdexcept>

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

SbClientApp::SbClientApp(int argc, char **argv) : SbApplication(argc, argv)
{
	LoadRenderSystemModule();
	
	mpRenderSystem->Init();
	
	LoadInputSystemModule();
	
	mpInputSystem->Init();
};

SbClientApp::~SbClientApp()
{
	mpInputSystem->Shutdown();
	
	UnloadInputSystemModule();
	
	mpRenderSystem->Shutdown();
	
	UnloadRenderSystemModule();
};

void SbClientApp::LoadRenderSystemModule()
{
	mnRenderSystemLib = mpSystem->LoadLib("SbGLCoreRenderer");
	
	if(!mnRenderSystemLib)
		throw std::runtime_error("Failed to load the renderer module!");
	
	using fnGetRenderSystemAPI = sbe::IRenderSystem *(*)();
	fnGetRenderSystemAPI pfnGetRenderSystemAPI{mpSystem->GetLibSymbol<fnGetRenderSystemAPI>(mnRenderSystemLib, "GetRenderSystemAPI")};
	
	if(!pfnGetRenderSystemAPI)
		throw std::runtime_error("");
	
	mpRenderSystem = pfnGetRenderSystemAPI();
	
	if(!mpRenderSystem)
		throw std::runtime_error("");
};

void SbClientApp::UnloadRenderSystemModule()
{
	mpSystem->FreeLib(mnRenderSystemLib);
};

void SbClientApp::LoadInputSystemModule()
{
	mnInputSystemLib = mpSystem->LoadLib("SbInput");
	
	if(!mnInputSystemLib)
		throw std::runtime_error("Failed to load the input module!");
	
	using fnGetInputSystemAPI = sbe::IInputSystem *(*)();
	fnGetInputSystemAPI pfnGetInputSystemAPI{mpSystem->GetLibSymbol<fnGetInputSystemAPI>(mnInputSystemLib, "GetInputSystemAPI")};
	
	if(!pfnGetInputSystemAPI)
		throw std::runtime_error("");
	
	mpInputSystem = pfnGetInputSystemAPI();
	
	if(!mpInputSystem)
		throw std::runtime_error("");
};

void SbClientApp::UnloadInputSystemModule()
{
	mpSystem->FreeLib(mnInputSystemLib);
};