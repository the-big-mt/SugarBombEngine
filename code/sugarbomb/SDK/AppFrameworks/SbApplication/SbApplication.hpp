/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 BlackPhrase

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

/// @file

#pragma once

//#include <memory>
#include <cstdint>

namespace google_breakpad
{
class ExceptionHandler;
};

namespace sbe
{
struct ISystem;
struct IFileSystem;
struct idCmdSystem;
struct idCVarSystem;
struct IFile;
};

//class idCmdSystemLocal;
//class idCVarSystemLocal;

class SbApplication
{
public:
	SbApplication(const char *asCmdLine);
	SbApplication(int argc, const char **argv);
	virtual ~SbApplication();
	
	virtual bool Run();
	
	void Stop();
protected:
	virtual bool Init();
	virtual void PostInit(){}
	
	//virtual void Shutdown(){}
	
	virtual bool PreFrame(){return true;}
	virtual void Frame(){}
	virtual void PostFrame(){}
	
	//std::unique_ptr<idCmdSystemLocal> mpCmdSystem;
	//std::unique_ptr<idCVarSystemLocal> mpCVarSystem;
	
	sbe::idCmdSystem *mpCmdSystem{nullptr};
	sbe::idCVarSystem *mpCVarSystem{nullptr};
	
	sbe::ISystem *mpSys{nullptr};
	sbe::IFileSystem *mpFileSystem{nullptr};
	
	bool mbRunning{false};
private:
	void ParseCommandLine(int argc, const char* const* argv);
	bool SafeMode();
	void StartupVariable(const char *asMatch);
	void AddStartupCommands();
	
	void InitSystemModule();
	void ShutdownSystemModule();
	
	void InitSIMD();
	
	void CloseLogFile();
	
	intptr_t DLL_Load(const char *asName);
	void *DLL_GetProcAddress(intptr_t ahDLLHandle, const char *asProcName);
	void DLL_Unload(intptr_t ahDLLHandle);
	
	sbe::IFile *logFile{nullptr};
	
#ifdef USE_BREAKPAD
#	ifdef _WIN32
	google_breakpad::ExceptionHandler *mpHandler{nullptr};
#	endif
#endif

#ifndef SBE_SINGLE_BINARY
	int sysDLL{0};
#endif
};