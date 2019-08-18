/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

#pragma hdrstop
#include "precompiled.h"
//#include <cstdio>
//#include <ctime>

//#include "../framework/CVarSystem.h"
//#include "../framework/CmdSystem.h"
//#include "../framework/FileSystem.h"
//#include "../idlib/Lib.h"
//#include "../idlib/Str.h"
#include "sys_local.h"

//namespace BFG
//{

const char* sysLanguageNames[] =
{
	ID_LANG_ENGLISH, ID_LANG_FRENCH, ID_LANG_ITALIAN, ID_LANG_GERMAN, ID_LANG_SPANISH, ID_LANG_JAPANESE, nullptr
};

const int numLanguages = sizeof( sysLanguageNames ) / sizeof sysLanguageNames[ 0 ] - 1;

idCVar sys_lang( "sys_lang", ID_LANG_ENGLISH, CVAR_SYSTEM | CVAR_INIT, "", sysLanguageNames, idCmdSystem::ArgCompletion_String<sysLanguageNames> );

static sysExport_t sysExport;

idSysLocal			sysLocal;
idSys* 				sys = &sysLocal;

// TODO
/*
#ifndef SBE_SINGLE_BINARY
idSys *sys = nullptr;
idCommon *common = nullptr;
idCmdSystem *cmdSystem = nullptr;
idCVarSystem *cvarSystem = nullptr;
idFileSystem *fileSystem = nullptr;
idCVar *idCVar::staticVars = nullptr;
#endif
*/

/*
===========
GetSysAPI
============
*/
#ifndef SBE_SINGLE_BINARY
C_EXPORT
#endif
sysExport_t *GetSysAPI(sysImport_t *import)
{
	if(import->version == SYS_API_VERSION)
	{
		// TODO
		/*
		// set interface pointers used by the module
		sys							= import->sys;
		common						= import->common;
		cmdSystem					= import->cmdSystem;
		cvarSystem					= import->cvarSystem;
		fileSystem					= import->fileSystem;
		*/
	};
	
	// TODO
	/*
	// set interface pointers used by idLib
	idLib::sys					= sys;
	idLib::common				= common;
	idLib::cvarSystem			= cvarSystem;
	idLib::fileSystem			= fileSystem;
	*/
	
	// setup export interface
	sysExport.version = SYS_API_VERSION;
	sysExport.sys = sys;
	
	return &sysExport;
};


void idSysLocal::DebugPrintf( const char* fmt, ... )
{
	va_list argptr;
	
	va_start( argptr, fmt );
	Sys_DebugVPrintf( fmt, argptr );
	va_end( argptr );
}

void idSysLocal::DebugVPrintf( const char* fmt, va_list arg )
{
	Sys_DebugVPrintf( fmt, arg );
}

double idSysLocal::GetClockTicks()
{
	return Sys_GetClockTicks();
}

double idSysLocal::ClockTicksPerSecond()
{
	return Sys_ClockTicksPerSecond();
}

cpuid_t idSysLocal::GetProcessorId()
{
	return Sys_GetProcessorId();
}

const char* idSysLocal::GetProcessorString()
{
	return Sys_GetProcessorString();
}

const char* idSysLocal::FPU_GetState()
{
	return Sys_FPU_GetState();
}

bool idSysLocal::FPU_StackIsEmpty()
{
	return Sys_FPU_StackIsEmpty();
}

void idSysLocal::FPU_SetFTZ( bool enable )
{
	Sys_FPU_SetFTZ( enable );
}

void idSysLocal::FPU_SetDAZ( bool enable )
{
	Sys_FPU_SetDAZ( enable );
}


int idSysLocal::DLL_Load( const char* dllName )
{
	return Sys_DLL_Load( dllName );
}

void* idSysLocal::DLL_GetProcAddress( int dllHandle, const char* procName )
{
	return Sys_DLL_GetProcAddress( dllHandle, procName );
}

void idSysLocal::DLL_Unload( int dllHandle )
{
	Sys_DLL_Unload( dllHandle );
}

void idSysLocal::DLL_GetFileName( const char* baseName, char* dllName, int maxLength )
{
	idStr::snPrintf( dllName, maxLength, "%s" CPUSTRING ".dll", baseName );
}

sysEvent_t idSysLocal::GenerateMouseButtonEvent( int button, bool down )
{
	sysEvent_t ev;
	ev.evType = SE_KEY;
	ev.evValue = K_MOUSE1 + button - 1;
	ev.evValue2 = down;
	ev.evPtrLength = 0;
	ev.evPtr = nullptr;
	return ev;
}

sysEvent_t idSysLocal::GenerateMouseMoveEvent( int deltax, int deltay )
{
	sysEvent_t ev;
	ev.evType = SE_MOUSE;
	ev.evValue = deltax;
	ev.evValue2 = deltay;
	ev.evPtrLength = 0;
	ev.evPtr = nullptr;
	return ev;
}

void idSysLocal::FPU_EnableExceptions( int exceptions )
{
	Sys_FPU_EnableExceptions( exceptions );
}

int idSysLocal::GetLangsNum() const
{
	return Sys_NumLangs();
}

const char *idSysLocal::GetLangName(int anIndex) const
{
	return Sys_Lang(anIndex);
}

/*
==================
idCommonLocal::VPrintf

A raw string should NEVER be passed as fmt, because of "%f" type crashes.
==================
*/
void idSysLocal::VPrintf( const char* fmt, va_list args )
{
	static bool	logFileFailed = false;
	
	// if the cvar system is not initialized
	if( !cvarSystem->IsInitialized() )
	{
		return;
	}
	// optionally put a timestamp at the beginning of each print,
	// so we can see how long different init sections are taking
	int timeLength = 0;
	char msg[MAX_PRINT_MSG_SIZE];
	msg[ 0 ] = '\0';
	if( com_timestampPrints.GetInteger() )
	{
		int	t = Sys_Milliseconds();
		if( com_timestampPrints.GetInteger() == 1 )
		{
			std::sprintf( msg, "[%5.2f]", t * 0.001f );
		}
		else
		{
			std::sprintf( msg, "[%i]", t );
		}
	}
	timeLength = strlen( msg );
	// don't overflow
	if( idStr::vsnPrintf( msg + timeLength, MAX_PRINT_MSG_SIZE - timeLength - 1, fmt, args ) < 0 )
	{
		msg[sizeof( msg ) - 2] = '\n';
		msg[sizeof( msg ) - 1] = '\0'; // avoid output garbling
		Sys_Printf( "idCommon::VPrintf: truncated to %d characters\n", strlen( msg ) - 1 );
	}
	
	if( rd_buffer )
	{
		if( ( int )( strlen( msg ) + strlen( rd_buffer ) ) > ( rd_buffersize - 1 ) )
		{
			rd_flush( rd_buffer );
			*rd_buffer = 0;
		}
		strcat( rd_buffer, msg );
		return;
	}
#ifndef ID_RETAIL
	if( com_printFilter.GetString() != nullptr && com_printFilter.GetString()[ 0 ] != '\0' )
	{
		idStrStatic< 4096 > filterBuf = com_printFilter.GetString();
		idStrStatic< 4096 > msgBuf = msg;
		filterBuf.ToLower();
		msgBuf.ToLower();
		char* sp = strtok( &filterBuf[ 0 ], ";" );
		bool p = false;
		for( ; sp != nullptr ; )
		{
			if( strstr( msgBuf, sp ) != nullptr )
			{
				p = true;
				break;
			}
			sp = strtok( nullptr, ";" );
		}
		if( !p )
		{
			return;
		}
	}
#endif
	
	
	if( !idLib::IsMainThread() )
	{
		// RB: printf should be thread-safe on Linux
#if defined(_WIN32)
		OutputDebugString( msg );
#else
		printf( "%s", msg );
#endif
		// RB end
		return;
	}
	
	// echo to console buffer
	console->Print( msg );
	
	// remove any color codes
	idStr::RemoveColors( msg );
	
	// echo to dedicated console and early console
	Sys_Printf( "%s", msg );
	
	// print to script debugger server
	// DebuggerServerPrint( msg );
	
#if 0	// !@#
#if defined(_DEBUG) && defined(_WIN32)
	if( strlen( msg ) < 512 )
	{
		TRACE( msg );
	}
#endif
#endif
	
	// logFile
	if( com_logFile.GetInteger() && !logFileFailed && fileSystem->IsInitialized() )
	{
		static bool recursing;
		
		if( !logFile && !recursing )
		{
			const char* fileName = com_logFileName.GetString()[0] ? com_logFileName.GetString() : "qconsole.log";
			
			// fileSystem->OpenFileWrite can cause recursive prints into here
			recursing = true;
			
			logFile = fileSystem->OpenFileWrite( fileName );
			if( !logFile )
			{
				logFileFailed = true;
				FatalError( "failed to open log file '%s'\n", fileName );
			}
			
			recursing = false;
			
			if( com_logFile.GetInteger() > 1 )
			{
				// force it to not buffer so we get valid
				// data even if we are crashing
				logFile->ForceFlush();
			}
			
			time_t aclock;
			time( &aclock );
			struct tm* newtime = localtime( &aclock );
			Printf( "log file '%s' opened on %s\n", fileName, asctime( newtime ) );
		}
		if( logFile )
		{
			logFile->Write( msg, strlen( msg ) );
			logFile->Flush();	// ForceFlush doesn't help a whole lot
		}
	}
	
	// don't trigger any updates if we are in the process of doing a fatal error
	if( com_errorEntered != ERP_FATAL )
	{
		// update the console if we are in a long-running command, like dmap
		if( com_refreshOnPrint )
		{
			const bool captureToImage = false;
			UpdateScreen( captureToImage );
		}
	}
}

/*
==================
idCommonLocal::Printf

Both client and server can use this, and it will output to the appropriate place.

A raw string should NEVER be passed as fmt, because of "%f" type crashers.
==================
*/
void idSysLocal::Printf( const char* fmt, ... )
{
	va_list argptr;
	va_start( argptr, fmt );
	VPrintf( fmt, argptr );
	va_end( argptr );
}

/*
==================
idCommonLocal::DPrintf

prints message that only shows up if the "developer" cvar is set
==================
*/
void idSysLocal::DPrintf( const char* fmt, ... )
{
	va_list		argptr;
	char		msg[MAX_PRINT_MSG_SIZE];
	
	if( !cvarSystem->IsInitialized() || !com_developer.GetBool() )
	{
		return;			// don't confuse non-developers with techie stuff...
	}
	
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, sizeof( msg ), fmt, argptr );
	va_end( argptr );
	msg[sizeof( msg ) - 1] = '\0';
	
	// never refresh the screen, which could cause reentrency problems
	bool temp = com_refreshOnPrint;
	com_refreshOnPrint = false;
	
	Printf( S_COLOR_RED"%s", msg );
	
	com_refreshOnPrint = temp;
}

/*
==================
idCommonLocal::DWarning

prints warning message in yellow that only shows up if the "developer" cvar is set
==================
*/
void idSysLocal::DWarning( const char* fmt, ... )
{
	va_list		argptr;
	char		msg[MAX_PRINT_MSG_SIZE];
	
	if( !com_developer.GetBool() )
	{
		return;			// don't confuse non-developers with techie stuff...
	}
	
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, sizeof( msg ), fmt, argptr );
	va_end( argptr );
	msg[sizeof( msg ) - 1] = '\0';
	
	Printf( S_COLOR_YELLOW"WARNING: %s\n", msg );
}

/*
==================
idCommonLocal::Warning

prints WARNING %s and adds the warning message to a queue to be printed later on
==================
*/
void idSysLocal::Warning( const char* fmt, ... )
{
	va_list		argptr;
	char		msg[MAX_PRINT_MSG_SIZE];
	
	if( !idLib::IsMainThread() )
		return;	// not thread safe!
	
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, sizeof( msg ), fmt, argptr );
	va_end( argptr );
	msg[sizeof( msg ) - 1] = 0;
	
	Printf( S_COLOR_YELLOW "WARNING: " S_COLOR_RED "%s\n", msg );
	
	if( warningList.Num() < MAX_WARNING_LIST )
		warningList.AddUnique( msg );
};

/*
==================
idCommonLocal::Error
==================
*/
void idSysLocal::Error( const char* fmt, ... )
{
	va_list		argptr;
	static int	lastErrorTime;
	static int	errorCount;
	int			currentTime;
	
	errorParm_t code = ERP_DROP;
	
	// always turn this off after an error
	com_refreshOnPrint = false;
	
	if( com_productionMode.GetInteger() == 3 )
	{
		Quit();
	}
	
	// when we are running automated scripts, make sure we
	// know if anything failed
	if( cvarSystem->GetCVarInteger( "fs_copyfiles" ) )
	{
		code = ERP_FATAL;
	}
	
	// if we don't have GL running, make it a fatal error
	if( !renderSystem->IsOpenGLRunning() )
	{
		code = ERP_FATAL;
	}
	
	// if we got a recursive error, make it fatal
	if( com_errorEntered )
	{
		// if we are recursively erroring while exiting
		// from a fatal error, just kill the entire
		// process immediately, which will prevent a
		// full screen rendering window covering the
		// error dialog
		if( com_errorEntered == ERP_FATAL )
		{
			Quit();
		}
		code = ERP_FATAL;
	}
	
	// if we are getting a solid stream of ERP_DROP, do an ERP_FATAL
	currentTime = Sys_Milliseconds();
	if( currentTime - lastErrorTime < 100 )
	{
		if( ++errorCount > 3 )
		{
			code = ERP_FATAL;
		}
	}
	else
	{
		errorCount = 0;
	}
	lastErrorTime = currentTime;
	
	com_errorEntered = code;
	
	va_start( argptr, fmt );
	idStr::vsnPrintf( errorMessage, sizeof( errorMessage ), fmt, argptr );
	va_end( argptr );
	errorMessage[sizeof( errorMessage ) - 1] = '\0';
	
	
	// copy the error message to the clip board
	Sys_SetClipboardData( errorMessage );
	
	// add the message to the error list
	errorList.AddUnique( errorMessage );
	
	Stop();
	
	if( code == ERP_DISCONNECT )
	{
		com_errorEntered = ERP_NONE;
		throw idException( errorMessage );
	}
	else if( code == ERP_DROP )
	{
		Printf( "********************\nERROR: %s\n********************\n", errorMessage );
		com_errorEntered = ERP_NONE;
		throw idException( errorMessage );
	}
	else
	{
		Printf( "********************\nERROR: %s\n********************\n", errorMessage );
	}
	
	if( cvarSystem->GetCVarBool( "r_fullscreen" ) )
	{
		cmdSystem->BufferCommandText( CMD_EXEC_NOW, "vid_restart partial windowed\n" );
	}
	
	Sys_Error( "%s", errorMessage );
	
}

/*
==================
idCommonLocal::FatalError

Dump out of the game to a system dialog
==================
*/
void idSysLocal::FatalError( const char* fmt, ... )
{
	va_list		argptr;
	
	if( com_productionMode.GetInteger() == 3 )
	{
		Quit();
	}
	// if we got a recursive error, make it fatal
	if( com_errorEntered )
	{
		// if we are recursively erroring while exiting
		// from a fatal error, just kill the entire
		// process immediately, which will prevent a
		// full screen rendering window covering the
		// error dialog
		
		Printf( "FATAL: recursed fatal error:\n%s\n", errorMessage );
		
		va_start( argptr, fmt );
		idStr::vsnPrintf( errorMessage, sizeof( errorMessage ), fmt, argptr );
		va_end( argptr );
		errorMessage[sizeof( errorMessage ) - 1] = '\0';
		
		Printf( "%s\n", errorMessage );
		
		// write the console to a log file?
		Quit();
	}
	com_errorEntered = ERP_FATAL;
	
	va_start( argptr, fmt );
	idStr::vsnPrintf( errorMessage, sizeof( errorMessage ), fmt, argptr );
	va_end( argptr );
	errorMessage[sizeof( errorMessage ) - 1] = '\0';
	
	if( cvarSystem->GetCVarBool( "r_fullscreen" ) )
	{
		cmdSystem->BufferCommandText( CMD_EXEC_NOW, "vid_restart partial windowed\n" );
	}
	
	Sys_SetFatalError( errorMessage );
	
	Error( "%s", errorMessage );
	
}

bool idSysLocal::AlreadyRunning() const
{
	return Sys_AlreadyRunning();
};

/*
=================
Sys_TimeStampToStr
=================
*/
const char* Sys_TimeStampToStr( ID_TIME_T timeStamp )
{
	static char timeString[MAX_STRING_CHARS];
	timeString[0] = '\0';
	
	time_t ts = ( time_t )timeStamp;
	tm*	time = localtime( &ts );
	if( time == nullptr )
	{
		// String separated to prevent detection of trigraphs
		return "??" "/" "??" "/" "???? ??:??";
	}
	
	idStr out;
	
	idStr lang = cvarSystem->GetCVarString( "sys_lang" );
	if( lang.Icmp( ID_LANG_ENGLISH ) == 0 )
	{
		// english gets "month/day/year  hour:min" + "am" or "pm"
		out = va( "%02d", time->tm_mon + 1 );
		out += "/";
		out += va( "%02d", time->tm_mday );
		out += "/";
		out += va( "%d", time->tm_year + 1900 );
		out += " ";	// changed to spaces since flash doesn't recognize \t
		if( time->tm_hour > 12 )
		{
			out += va( "%02d", time->tm_hour - 12 );
		}
		else if( time->tm_hour == 0 )
		{
			out += "12";
		}
		else
		{
			out += va( "%02d", time->tm_hour );
		}
		out += ":";
		out += va( "%02d", time->tm_min );
		if( time->tm_hour >= 12 )
		{
			out += "pm";
		}
		else
		{
			out += "am";
		}
	}
	else
	{
		// europeans get "day/month/year  24hour:min"
		out = va( "%02d", time->tm_mday );
		out += "/";
		out += va( "%02d", time->tm_mon + 1 );
		out += "/";
		out += va( "%d", time->tm_year + 1900 );
		out += " ";	// changed to spaces since flash doesn't recognize \t
		out += va( "%02d", time->tm_hour );
		out += ":";
		out += va( "%02d", time->tm_min );
	}
	idStr::Copynz( timeString, out, sizeof( timeString ) );
	
	return timeString;
}

/*
========================
Sys_SecToStr
========================
*/
const char* Sys_SecToStr( int sec )
{
	static char timeString[MAX_STRING_CHARS];
	
	int weeks = sec / ( 3600 * 24 * 7 );
	sec -= weeks * ( 3600 * 24 * 7 );
	
	int days = sec / ( 3600 * 24 );
	sec -= days * ( 3600 * 24 );
	
	int hours = sec / 3600;
	sec -= hours * 3600;
	
	int min = sec / 60;
	sec -= min * 60;
	
	if( weeks > 0 )
		std::sprintf( timeString, "%dw, %dd, %d:%02d:%02d", weeks, days, hours, min, sec );
	else if( days > 0 )
		std::sprintf( timeString, "%dd, %d:%02d:%02d", days, hours, min, sec );
	else
		std::sprintf( timeString, "%d:%02d:%02d", hours, min, sec );
	
	return timeString;
}

// return number of supported languages
int Sys_NumLangs()
{
	return numLanguages;
}

// get language name by index
const char* Sys_Lang( int idx )
{
	if( idx >= 0 && idx < numLanguages )
	{
		return sysLanguageNames[ idx ];
	}
	return "";
}

const char* Sys_DefaultLanguage()
{
	// sku breakdowns are as follows
	//  EFIGS	Digital
	//  EF  S	North America
	//   FIGS	EU
	//  E		UK
	// JE    	Japan
	
	// If japanese exists, default to japanese
	// else if english exists, defaults to english
	// otherwise, french
	
	if( !fileSystem->UsingResourceFiles() )
	{
		return ID_LANG_ENGLISH;
	}
	
	idStr fileName;
	
	//D3XP: Instead of just loading a single lang file for each language
	//we are going to load all files that begin with the language name
	//similar to the way pak files work. So you can place english001.lang
	//to add new strings to the english language dictionary
	idFileList* langFiles;
	langFiles = fileSystem->ListFilesTree( "strings", ".lang", true );
	
	idStrList langList = langFiles->GetList();
	
	// Loop through the list and filter
	idStrList currentLangList = langList;
	
	idStr temp;
	for( int i = 0; i < currentLangList.Num(); i++ )
	{
		temp = currentLangList[i];
		temp = temp.Right( temp.Length() - strlen( "strings/" ) );
		temp = temp.Left( temp.Length() - strlen( ".lang" ) );
		currentLangList[i] = temp;
	}
	
	if( currentLangList.Num() <= 0 )
	{
		// call it English if no lang files exist
		sys_lang.SetString( ID_LANG_ENGLISH );
	}
	else if( currentLangList.Num() == 1 )
	{
		sys_lang.SetString( currentLangList[0] );
	}
	else
	{
		if( currentLangList.Find( ID_LANG_JAPANESE ) )
		{
			sys_lang.SetString( ID_LANG_JAPANESE );
		}
		else if( currentLangList.Find( ID_LANG_ENGLISH ) )
		{
			sys_lang.SetString( ID_LANG_ENGLISH );
		}
		else if( currentLangList.Find( ID_LANG_FRENCH ) )
		{
			sys_lang.SetString( ID_LANG_FRENCH );
		}
		else if( currentLangList.Find( ID_LANG_GERMAN ) )
		{
			sys_lang.SetString( ID_LANG_GERMAN );
		}
		else if( currentLangList.Find( ID_LANG_ITALIAN ) )
		{
			sys_lang.SetString( ID_LANG_GERMAN );
		}
		else if( currentLangList.Find( ID_LANG_SPANISH ) )
		{
			sys_lang.SetString( ID_LANG_GERMAN );
		}
		else
		{
			sys_lang.SetString( currentLangList[0] );
		}
	}
	
	fileSystem->FreeFileList( langFiles );
	
	return sys_lang.GetString();// ID_LANG_ENGLISH;
	
	
}

//} // namespace BFG