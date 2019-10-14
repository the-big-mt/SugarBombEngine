/// @file

#pragma once

#include "AppFrameworks/SbApplication/SbApplication.hpp"

class SbClientApp : public SbApplication
{
public:
	SbClientApp(int argc, char **argv);
	virtual ~SbClientApp() = default;
};