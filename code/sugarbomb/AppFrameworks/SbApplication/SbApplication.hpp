/// @file

#pragma once

class SbApplication
{
public:
	SbApplication(const char *asCmdLine);
	SbApplication(int argc, char **argv);
	virtual ~SbApplication() = default;
	
	virtual bool Run();
	
	void Stop();
protected:
	virtual bool Init(){return true;}
	virtual void PostInit(){}
	
	//virtual void Shutdown(){}
	
	virtual bool PreFrame(){return true;}
	virtual void Frame(){}
	virtual void PostFrame(){}
	
	bool mbRunning{false};
};