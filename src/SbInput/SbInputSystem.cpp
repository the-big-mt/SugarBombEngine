/// @file

#include "SbInputSystem.hpp"

static inputExport_t inputExport;

#ifndef SBE_SINGLE_BINARY
idSys *sys{nullptr};
idCommon *common{nullptr};
idCmdSystem *cmdSystem{nullptr};
idCVarSystem *cvarSystem{nullptr};
//idFileSystem *fileSystem{nullptr};
idCVar *idCVar::staticVars{nullptr};
#endif

/*
===========
GetInputAPI
============
*/
#ifndef SBE_SINGLE_BINARY
C_EXPORT
#endif
inputExport_t *GetInputAPI(inputImport_t *import)
{
	if(import->version == INPUT_API_VERSION)
	{
		// set interface pointers used by the module
		sys = import->sys;
		common = import->common;
		cmdSystem = import->cmdSystem;
		cvarSystem = import->cvarSystem;
		//fileSystem = import->fileSystem;
	};
	
	// set interface pointers used by idLib
	idLib::sys = sys;
	idLib::common = common;
	idLib::cvarSystem = cvarSystem;
	//idLib::fileSystem = fileSystem;
	
	// setup export interface
	inputExport.version = INPUT_API_VERSION;
	inputExport.inputSystem = CreateInputSystem(common);
	
	return &inputExport;
};