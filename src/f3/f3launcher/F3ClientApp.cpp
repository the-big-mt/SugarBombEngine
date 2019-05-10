/// @file

#include "F3ClientApp.hpp"

namespace f3bfg
{

CClientApp::~CClientApp()
{
	Shutdown();
	
	ShutdownFrameworkModule();
	ShutdownSoundSystem();
};

void CClientApp::PostInit()
{
	// start the sound system, but don't do any hardware operations yet
	InitSoundSystem();
	
	InitFrameworkModule();
	
	mpCommon->Init(0, nullptr, lpCmdLine);
	
#if TEST_FPU_EXCEPTIONS != 0
	mpCommon->Printf(Sys_FPU_GetState());
#endif
};

void CClientApp::PostFrame()
{
	// run the game
	mpCommon->Frame();
};

void CClientApp::Shutdown()
{
	// kill sound first
	printf( "soundSystem->StopAllSounds();\n" );
	soundSystem->StopAllSounds();
	
	mpCommon->Shutdown();
	
	// shut down the sound system
	printf( "soundSystem->Shutdown();\n" );
	soundSystem->Shutdown();
};

/*
=================
idCommonLocal::LoadSoundModule
=================
*/
void CClientApp::InitSoundSystem()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::soundImport_t	soundImport;
	sbe::soundExport_t	soundExport;
	sbe::GetSoundAPI_t	GetSoundAPI;
	
	mpFileSystem->FindDLL( "SbSound", dllPath, true );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find sound dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading sound DLL: '%s'\n", dllPath );
	soundDLL = mpSys->DLL_Load( dllPath );
	if( !soundDLL )
	{
		mpSys->FatalError( "couldn't load sound dynamic library" );
		return;
	};
	
	const char* functionName = "GetSoundAPI";
	GetSoundAPI = ( GetSoundAPI_t ) Sys_DLL_GetProcAddress( soundDLL, functionName );
	if( !GetSoundAPI )
	{
		Sys_DLL_Unload( soundDLL );
		soundDLL = 0;
		mpSys->FatalError( "couldn't find sound DLL API" );
		return;
	};
	
	soundImport.version					= sbe::SOUND_API_VERSION;
	soundImport.sys						= mpSys;
	soundImport.common					= ::common;
	soundImport.cmdSystem				= ::cmdSystem;
	soundImport.cvarSystem				= ::cvarSystem;
	soundImport.fileSystem				= mpFileSystem;
	soundImport.renderSystem				= renderSystem;
	soundImport.declManager				= ::declManager;
	
	soundExport							= *GetSoundAPI( &soundImport );
	
	if( soundExport.version != SOUND_API_VERSION )
	{
		Sys_DLL_Unload( soundDLL );
		soundDLL = 0;
		common->FatalError( "wrong sound DLL API version" );
		return;
	};
	
	soundSystem								= soundExport.soundSystem;
	
#endif
	
	// initialize the sound object
	if( soundSystem != nullptr )
		soundSystem->Init();
};

/*
=================
idCommonLocal::UnloadGameModule
=================
*/
void CClientApp::ShutdownSoundSystem()
{
	// shut down the sound object
	if( soundSystem != nullptr )
	{
		soundSystem->Shutdown();
		soundSystem = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( soundDLL )
	{
		Sys_DLL_Unload( soundDLL );
		soundDLL = 0;
	};
#endif
};

/*
=================
idCommonLocal::LoadSoundModule
=================
*/
void CClientApp::InitFrameworkModule()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::frameworkImport_t	frameworkImport;
	sbe::frameworkExport_t	frameworkExport;
	sbe::GetFrameworkAPI_t	GetFrameworkAPI;
	
	mpFileSystem->FindDLL( "SbFramework", dllPath/*, true*/ );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find framework dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading framework DLL: '%s'\n", dllPath );
	frameworkDLL = mpSys->DLL_Load( dllPath );
	if( !frameworkDLL )
	{
		mpSys->FatalError( "couldn't load framework dynamic library" );
		return;
	};
	
	const char* functionName = "GetFrameworkAPI";
	GetFrameworkAPI = ( GetFrameworkAPI_t ) mpSys->DLL_GetProcAddress( frameworkDLL, functionName );
	if( !GetFrameworkAPI )
	{
		mpSys->DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
		mpSys->FatalError( "couldn't find framework DLL API" );
		return;
	};
	
	frameworkImport.version					= sbe::FRAMEWORK_API_VERSION;
	frameworkImport.sys						= mpSys;
	frameworkImport.cmdSystem				= mpCmdSystem.get();
	frameworkImport.cvarSystem				= mpCVarSystem.get();
	frameworkImport.fileSystem				= mpFileSystem;
	frameworkImport.renderSystem				= renderSystem;
	frameworkImport.declManager				= ::declManager;
	
	frameworkExport							= *GetFrameworkAPI( &frameworkImport );
	
	if( frameworkExport.version != sbe::FRAMEWORK_API_VERSION )
	{
		mpSys->DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
		mpSys->FatalError( "wrong framework DLL API version" );
		return;
	};
	
	mpCommon								= frameworkExport.common;
	
#endif
	
	// initialize the sound object
	if( mpCommon != nullptr )
		mpCommon->Init();
};

/*
=================
idCommonLocal::UnloadFrameworkModule
=================
*/
void CClientApp::ShutdownFrameworkModule()
{
	// shut down the framework object
	if( mpCommon != nullptr )
	{
		mpCommon->Shutdown();
		mpCommon = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( frameworkDLL )
	{
		Sys_DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
	};
#endif
};

}; // namespace f3bfg