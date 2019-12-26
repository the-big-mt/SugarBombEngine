/// @file

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

#include "SbWindowWin.hpp"

sbe::IWindow *SbClientApp::CreateMainWindow(const std::string &asTitle, int anWidth, int anHeight, bool abFullScreen)
{
	return new sbe::SbWindowWin(asTitle.c_str(), "SugarBombEngine", 0, *mpSystem, anWidth, anHeight, abFullScreen); // TODO: de-hardcode
};

bool SbClientApp::PreFrame()
{
	static MSG Msg{};
	
	if(PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
	{
		if(Msg.message == WM_QUIT)
			Stop();
		
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		return false;
	};
	
	return true;
};