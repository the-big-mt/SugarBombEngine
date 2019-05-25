/// @file

#include "SbApplication.hpp"

#include "framework/ICmdSystem.hpp"
#include "framework/ICVarSystem.hpp"
#include "framework/CVar.hpp"

#include "sys/IFileSystem.hpp"

#include "idlib/CmdArgs.h"
extern sbe::idCmdSystem *cmdSystem;
extern sbe::idCVarSystem *cvarSystem;

sbe::idCVar com_logFile( "logFile", "0", sbe::CVAR_SYSTEM | sbe::CVAR_NOCHEAT, "1 = buffer log, 2 = flush after each print", 0, 2, sbe::idCmdSystem::ArgCompletion_Integer<0, 2> );
sbe::idCVar com_logFileName( "logFileName", "sbeconsole.log", sbe::CVAR_SYSTEM | sbe::CVAR_NOCHEAT, "name of log file, if empty, sbeconsole.log will be used" );


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

SbApplication::SbApplication(int argc, const char **argv)
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
==================
idCommonLocal::SafeMode

Check for "safe" on the command line, which will
skip loading of config file (DoomConfig.cfg)
==================
*/
bool SbApplication::SafeMode()
{
	int			i;
	
	for( i = 0 ; i < com_numConsoleLines ; i++ )
	{
		if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "safe" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "cvar_restart" ) )
		{
			com_consoleLines[ i ].Clear();
			return true;
		};
	};
	return false;
};

/*
==================
idCommonLocal::StartupVariable

Searches for command line parameters that are set commands.
If match is not nullptr, only that cvar will be looked for.
That is necessary because cddir and basedir need to be set
before the filesystem is started, but all other sets should
be after execing the config and default.
==================
*/
void SbApplication::StartupVariable( const char* match )
{
	int i = 0;
	while(	i < com_numConsoleLines )
	{
		if( strcmp( com_consoleLines[ i ].Argv( 0 ), "set" ) != 0 )
		{
			i++;
			continue;
		};
		const char* s = com_consoleLines[ i ].Argv( 1 );
		
		if( !match || !idStr::Icmp( s, match ) )
		{
			mpCVarSystem->SetCVarString( s, com_consoleLines[ i ].Argv( 2 ) );
		}
		i++;
	};
};

/*
==================
idCommonLocal::AddStartupCommands

Adds command line parameters as script statements
Commands are separated by + signs

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
==================
*/
void SbApplication::AddStartupCommands()
{
	// quote every token, so args with semicolons can work
	for( int i = 0; i < com_numConsoleLines; i++ )
	{
		if( !com_consoleLines[i].Argc() )
		{
			continue;
		}
		// directly as tokenized so nothing gets screwed
		mpCmdSystem->BufferCommandArgs( sbe::CMD_EXEC_APPEND, com_consoleLines[i] );
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
==================
idCommonLocal::CloseLogFile
==================
*/
void SbApplication::CloseLogFile()
{
	if( logFile )
	{
		com_logFile.SetBool( false ); // make sure no further VPrintf attempts to open the log file again
		mpFileSystem->CloseFile( logFile );
		logFile = nullptr;
	};
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