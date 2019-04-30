/// @file

#pragma once

#include "SbApplication/SbApplication.hpp"

class SbClientApp : public SbApplication
{
public:
	SbClientApp();
	virtual ~SbClientApp();
private:
	constexpr auto sDefaultAppTitle{"SugarBombEngine Client Application"};
	constexpr auto nDefaultWindowSizeW{1280};
	constexpr auto nDefaultWindowSizeH{600};
	
	bool Init() override;
	
	void CreateMainWindow(int anWidth = nDefaultWindowSizeW, int anHeight = nDefaultWindowSizeH, const char *asTitle = sDefaultAppTitle, bool abFullScreen = false);
	
	void InitRenderSystem();
	void ShutdownRenderSystem();
	
	void InitInputSystem();
	void ShutdownInputSystem();
	
	sbe::IRenderSystem *renderSystem{nullptr};
	sbe::IInputSystem *inputSystem{nullptr};
	
	int renderDLL{-1};
	int inputDLL{-1};
protected:
	virtual bool PreInputUpdate(){return true;}
	virtual void InputUpdate(){}
	virtual void PostInputUpdate(){}
	
	virtual bool PreLogicUpdate(){return true;}
	virtual void LogicUpdate(){}
	virtual void PostLogicUpdate(){}
	
	virtual bool PreRender(){return true;}
	virtual void Render(){}
	virtual void PostRender(){}
};