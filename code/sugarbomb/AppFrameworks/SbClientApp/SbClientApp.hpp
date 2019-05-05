/// @file

#pragma once

#include <memory>

#include "SbApplication/SbApplication.hpp"

#ifdef _WIN32
using HWND = void*;
#endif

namespace google_breakpad
{
class ExceptionHandler;
};

namespace sbe
{
struct ISys;
struct IFileSystem;
struct IRenderSystem;
struct IInputSystem;
};

class idCmdSystemLocal;
class idCVarSystemLocal;

class SbClientApp : public SbApplication
{
public:
	SbClientApp();
	virtual ~SbClientApp();
private:
	static constexpr auto sDefaultAppTitle{"SugarBombEngine Client Application"};
	static constexpr auto nDefaultWindowSizeW{1280};
	static constexpr auto nDefaultWindowSizeH{600};
	
	bool Init() override;
	
	void Frame() override;
	
	void CreateMainWindow(int anWidth = nDefaultWindowSizeW, int anHeight = nDefaultWindowSizeH, const char *asTitle = sDefaultAppTitle, bool abFullScreen = false);
	
	void InitSystemModule();
	void ShutdownSystemModule();
	
	void InitRenderSystem();
	void ShutdownRenderSystem();
	
	void InitInputSystem();
	void ShutdownInputSystem();
	
	std::unique_ptr<idCmdSystemLocal> mpCmdSystem;
	std::unique_ptr<idCVarSystemLocal> mpCVarSystem;
	
	sbe::ISys *mpSys{nullptr};
	sbe::IFileSystem *mpFileSystem{nullptr};
	sbe::IRenderSystem *renderSystem{nullptr};
	sbe::IInputSystem *inputSystem{nullptr};
	
#ifdef USE_BREAKPAD
#	ifdef _WIN32
	google_breakpad::ExceptionHandler *mpHandler{nullptr};
#	endif
#endif

	int sysDLL{0};
	int renderDLL{0};
	int inputDLL{0};
	
#ifdef _WIN32
	HWND mhWnd{nullptr};
#else
	void *mpWindow{nullptr};
#endif
protected:
	virtual bool PreInputUpdate(){return true;}
	virtual void InputUpdate();
	virtual void PostInputUpdate(){}
	
	virtual bool PreLogicUpdate(){return true;}
	virtual void LogicUpdate(){}
	virtual void PostLogicUpdate(){}
	
	virtual bool PreRender(){return true;}
	virtual void Render();
	virtual void PostRender(){}
};