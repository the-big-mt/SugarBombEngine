/// @file

#pragma once

#include <memory>
#include <cstdint>

namespace google_breakpad
{
class ExceptionHandler;
};

namespace sbe
{
struct ISys;
struct IFileSystem;
};

class idCmdSystemLocal;
class idCVarSystemLocal;

class SbApplication
{
public:
	SbApplication(const char *asCmdLine);
	SbApplication(int argc, char **argv);
	virtual ~SbApplication() = default;
	
	virtual bool Run();
	
	void Stop();
protected:
	virtual bool Init();
	virtual void PostInit(){}
	
	//virtual void Shutdown(){}
	
	virtual bool PreFrame(){return true;}
	virtual void Frame(){}
	virtual void PostFrame(){}
	
	std::unique_ptr<idCmdSystemLocal> mpCmdSystem;
	std::unique_ptr<idCVarSystemLocal> mpCVarSystem;
	
	sbe::ISys *mpSys{nullptr};
	sbe::IFileSystem *mpFileSystem{nullptr};
	
	bool mbRunning{false};
private:
	void ParseCommandLine(int argc, const char* const* argv);
	
	void InitSystemModule();
	void ShutdownSystemModule();
	
	intptr_t DLL_Load(const char *asName);
	void *DLL_GetProcAddress(intptr_t ahDLLHandle, const char *asProcName);
	void DLL_Unload(intptr_t ahDLLHandle);
	
#ifdef USE_BREAKPAD
#	ifdef _WIN32
	google_breakpad::ExceptionHandler *mpHandler{nullptr};
#	endif
#endif

#ifndef SBE_SINGLE_BINARY
	int sysDLL{0};
#endif
};