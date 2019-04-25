/// @file

#include "SbApplication.hpp"

SbApplication::SbApplication(const char *asCmdLine)
{
};

SbApplication::SbApplication(int argc, char **argv)
{
};

bool SbApplication::Run()
{
	if(!Init())
		return false;
	
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

void SbApplication::Frame()
{
	if(PreInputUpdate())
	{
		InputUpdate();
		PostInputUpdate();
	};
	
	if(PreLogicUpdate())
	{
		LogicUpdate();
		PostLogicUpdate();
	};
	
	if(PreRender())
	{
		Render();
		PostRender();
	};
};