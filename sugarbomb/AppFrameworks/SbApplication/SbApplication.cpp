/// @file

#include <cassert>
#include <stdexcept>

#include "AppFrameworks/SbApplication/SbApplication.hpp"
#include "SbLibraryLoader.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

SbApplication::SbApplication(int argc, char **argv)
{
	Init();
};

SbApplication::~SbApplication()
{
	mpSystem->Shutdown();
	
	UnloadSystemModule();
};

void SbApplication::Run()
{
	assert(mbInitialized);

	while(true)
	{
		RunFrame();
	};
};

void SbApplication::Init()
{
	LoadSystemModule();
	
	mpSystem->Init();
	
	mbInitialized = true;
};

void SbApplication::LoadSystemModule()
{
	mnSystemLib = sbe::SbLibraryLoader::Load("SbSystem");
	
	if(!mnSystemLib)
		throw std::runtime_error("Failed to load the system module!");
	
	using fnGetSystemAPI = sbe::ISystem *(*)();
	fnGetSystemAPI pfnGetSystemAPI{sbe::SbLibraryLoader::GetSymbol<fnGetSystemAPI>(mnSystemLib, "GetSystemAPI")};
	
	if(!pfnGetSystemAPI)
		throw std::runtime_error("");
	
	mpSystem = pfnGetSystemAPI();
	
	if(!mpSystem)
		throw std::runtime_error("");
};

void SbApplication::UnloadSystemModule()
{
	sbe::SbLibraryLoader::Unload(mnSystemLib);
};

void SbApplication::RunFrame()
{
};