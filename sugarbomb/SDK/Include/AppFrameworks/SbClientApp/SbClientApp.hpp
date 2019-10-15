/// @file

#pragma once

#include "AppFrameworks/SbApplication/SbApplication.hpp"

namespace sbe
{
struct IRenderSystem;
struct IInputSystem;
};

class SbClientApp : public SbApplication
{
public:
	SbClientApp(int argc, char **argv);
	virtual ~SbClientApp();
private:
	void LoadRenderSystemModule();
	void UnloadRenderSystemModule();
	
	void LoadInputSystemModule();
	void UnloadInputSystemModule();
	
	sbe::IRenderSystem *mpRenderSystem{nullptr};
	sbe::IInputSystem *mpInputSystem{nullptr};
	
	int mnRenderSystemLib{-1};
	int mnInputSystemLib{-1};
};