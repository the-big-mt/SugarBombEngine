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
	virtual bool Init(){}
	//virtual void Shutdown(){}
	
	virtual bool PreFrame(){return true;}
	virtual void Frame()
	virtual void PostFrame(){}
	
	virtual bool PreInputUpdate(){return false;}
	virtual void InputUpdate(){}
	virtual void PostInputUpdate(){}
	
	virtual bool PreLogicUpdate(){return false;}
	virtual void LogicUpdate(){}
	virtual void PostLogicUpdate(){}
	
	virtual bool PreRender(){return false;}
	virtual void Render(){}
	virtual void PostRender(){}
	
	bool mbRunning{false};
};