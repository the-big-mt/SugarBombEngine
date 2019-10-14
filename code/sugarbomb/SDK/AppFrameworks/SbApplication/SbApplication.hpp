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
struct idFile;
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
	
	sbe::idFile *logFile{nullptr};
	
#ifdef USE_BREAKPAD
#	ifdef _WIN32
	google_breakpad::ExceptionHandler *mpHandler{nullptr};
#	endif
#endif

#ifndef SBE_SINGLE_BINARY
	int sysDLL{0};
#endif
};