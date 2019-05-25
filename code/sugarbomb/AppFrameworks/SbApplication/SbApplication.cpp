/// @file

#include "SbApplication.hpp"

#include "framework/ICmdSystem.hpp"
#include "framework/ICVarSystem.hpp"
#include "idlib/CmdArgs.h"
extern sbe::idCmdSystem *cmdSystem;
extern sbe::idCVarSystem *cvarSystem;


SbApplication::SbApplication(const char *asCmdLine)
{
	int argc;
	const char * const *argv;
	
	//idLib::Printf( "Command line: %s\n", asCmdLine );
	//::MessageBox( nullptr, asCmdLine, "blah", MB_OK );
	
	// parse command line options
	idCmdArgs args;
	if(asCmdLine)
	{
		// tokenize if the OS doesn't do it for us
		args.TokenizeString(asCmdLine, true);
		argv = args.GetArgs(&argc);
	};
	ParseCommandLine(argc, argv);
};

SbApplication::SbApplication(int argc, char **argv)
{
	ParseCommandLine(argc, argv);
};

SbApplication::~SbApplication()
{
	// only shut down the log file after all output is done
	printf( "CloseLogFile();\n" );
	CloseLogFile();
	
	// shut down the file system
	printf( "fileSystem->Shutdown( false );\n" );
	mpFileSystem->Shutdown( false );
	
	ShutdownSystemModule();
	
	// shut down the cvar system
	printf( "cvarSystem->Shutdown();\n" );
	mpCVarSystem->Shutdown();
	
	// shut down the console command system
	printf( "cmdSystem->Shutdown();\n" );
	mpCmdSystem->Shutdown();
};

bool SbApplication::Run()
{
	if(!Init())
		return false;
	
	PostInit();
	
	mbRunning = true;
	
	// app's main loop
	while(mbRunning)
	{
		if(PreFrame())
		{
			Frame();
			PostFrame();
		};
	};
	
	return true;
};

void SbApplication::Stop()
{
	mbRunning = false;
};

void SbApplication::Init()
{
	//mpCmdSystem = std::make_unique<idCmdSystemLocal>();
	//mpCVarSystem = std::make_unique<idCVarSystemLocal>();
	
	mpCmdSystem = cmdSystem;
	mpCVarSystem = cvarSystem;
	
	
	// init console command system
	mpCmdSystem->Init();
	
	// init CVar system
	mpCVarSystem->Init();
	
	InitSystemModule();
	
	if( Sys_AlreadyRunning() )
		mpSys->Quit();
	
	// initialize processor specific SIMD implementation
	InitSIMD();
	
	// initialize the file system
	mpFileSystem->Init();
	return true;
};

/*
============================================================================

COMMAND LINE FUNCTIONS

+ characters separate the commandLine string into multiple console
command lines.

All of these are valid:

doom +set test blah +map test
doom set test blah+map test
doom set test blah + map test

============================================================================
*/

#define		MAX_CONSOLE_LINES	32
int			com_numConsoleLines;
idCmdArgs	com_consoleLines[MAX_CONSOLE_LINES];

/*
==================
idCommonLocal::ParseCommandLine
==================
*/
void SbApplication::ParseCommandLine( int argc, const char* const* argv )
{
	int i, current_count;
	
	com_numConsoleLines = 0;
	current_count = 0;
	// API says no program path
	for( i = 0; i < argc; i++ )
	{
		if( argv[ i ][ 0 ] == '+' )
		{
			com_numConsoleLines++;
			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] + 1 );
		}
		else
		{
			if( !com_numConsoleLines )
				com_numConsoleLines++;

			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] );
		};
	};
};

/*
=================
idCommonLocal::LoadSystemDLL
=================
*/
void SbApplication::InitSystemModule()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::sysImport_t	sysImport;
	sbe::sysExport_t	sysExport;
	sbe::GetSysAPI_t	GetSysAPI;
	
	strncpy(dllPath, "./SbSystem", sbe::MAX_OSPATH);
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find system dynamic library" );
		return;
	}
	mpSys->DPrintf( "Loading system DLL: '%s'\n", dllPath );
	sysDLL = Sys_DLL_Load( dllPath );
	if( !sysDLL )
	{
		mpSys->FatalError( "couldn't load system dynamic library" );
		return;
	}
	
	const char* functionName = "GetSysAPI";
	GetSysAPI = ( sbe::GetSysAPI_t ) Sys_DLL_GetProcAddress( sysDLL, functionName );
	if( !GetSysAPI )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
		mpSys->FatalError( "couldn't find system DLL API" );
		return;
	}
	
	sysImport.version					= sbe::SYS_API_VERSION;
	//sysImport.common					= ::common;
	sysImport.cmdSystem				= mpCmdSystem.get();
	sysImport.cvarSystem				= mpCVarSystem.get();
	
	sysExport = *GetSysAPI( &sysImport );
	
	if( sysExport.version != sbe::SYS_API_VERSION )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
		mpSys->FatalError( "wrong system DLL API version" );
		return;
	};
	
	mpSys = sysExport.sys;
	mpFileSystem = sysExport.fileSystem;
	
#endif
	
	// initialize the system object
	// get architecture info
	if( mpSys != nullptr )
		mpSys->Init();
};

/*
=================
idCommonLocal::UnloadSystemDLL
=================
*/
void SbApplication::ShutdownSystemModule()
{
	// shut down the system object
	if( mpSys != nullptr )
	{
		// shut down non-portable system services
		printf( "Sys_Shutdown();\n" );
		mpSys->Shutdown();
		mpSys = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( sysDLL )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
	};
#endif
};

/*
========================================================================

DLL Loading

========================================================================
*/

/*
=====================
Sys_DLL_Load
=====================
*/
// RB: 64 bit fixes, changed int to intptr_t
intptr_t SbApplication::DLL_Load( const char *dllName )
{
#ifdef _WIN32
	HINSTANCE libHandle = LoadLibrary( dllName );
	return (int)libHandle;
#else
	auto handle = dlopen( path, RTLD_NOW );
	if( !handle )
		printf( "dlopen '%s' failed: %s\n", path, dlerror() );
	
	return ( intptr_t )handle;
#endif
};

/*
=====================
Sys_DLL_GetProcAddress
=====================
*/
void *SbApplication::DLL_GetProcAddress( intptr_t dllHandle, const char *procName )
{
#ifdef _WIN32
	// RB: added missing cast
	return ( void* ) GetProcAddress( (HINSTANCE)dllHandle, procName );
#else
	const char* error;
	void* ret = dlsym( ( void* )dllHandle, procName );
	if( ( error = dlerror() ) != nullptr )
		printf( "dlsym '%s' failed: %s\n", sym, error );
	return ret;
#endif
};

/*
=====================
Sys_DLL_Unload
=====================
*/
void SbApplication::DLL_Unload( intptr_t dllHandle )
{
#ifdef _WIN32
	if( !dllHandle )
		return;
	
	if( FreeLibrary( (HINSTANCE)dllHandle ) == 0 )
	{
		int lastError = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
		    nullptr,
			lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			nullptr 
		);

		//Sys_Error( "Sys_DLL_Unload: FreeLibrary failed - %s (%d)", lpMsgBuf, lastError );
	};
#else // if not _WIN32
	dlclose( ( void* )handle );
#endif // _WIN32
};