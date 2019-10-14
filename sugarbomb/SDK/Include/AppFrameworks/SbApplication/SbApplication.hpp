/// @file

#pragma once

class SbApplication
{
public:
	SbApplication(int argc, char **argv);
	virtual ~SbApplication() = default;
	
	void Run();
private:
	void Init();
	
	void RunFrame();
	
	bool mbInitialized{false};
};