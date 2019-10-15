/// @file

#pragma once

namespace sbe
{
struct ISystem;
};

class SbApplication
{
public:
	SbApplication(int argc, char **argv);
	virtual ~SbApplication();
	
	void Run();
protected:
	sbe::ISystem *mpSystem{nullptr};
private:
	void Init();
	
	void LoadSystemModule();
	void UnloadSystemModule();
	
	void RunFrame();
	
	bool mbInitialized{false};
	
	int mnSystemLib{-1};
};