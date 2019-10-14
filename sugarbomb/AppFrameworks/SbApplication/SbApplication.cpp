/// @file

#include <cassert>

#include "AppFrameworks/SbApplication/SbApplication.hpp"

SbApplication::SbApplication(int argc, char **argv)
{
	Init();
};

void SbApplication::Run()
{
	assert(mbInitialized);

	while(true)
	{
		RunFrame();
	};
};

void SbApplication::Init()
{
	mbInitialized = true;
};

void SbApplication::RunFrame()
{
};