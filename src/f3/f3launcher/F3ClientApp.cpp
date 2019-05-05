/// @file

#include "F3ClientApp.hpp"

namespace f3bfg
{

CClientApp::~CClientApp()
{
	Shutdown();
	
	ShutdownSoundSystem();
};

void CClientApp::PostInit()
{
	// start the sound system, but don't do any hardware operations yet
	InitSoundSystem();
	
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

}; // namespace f3bfg