/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

class SbWindowWin : public IWindow
{
public:
	SbWindowWin(ISystem &aSystem, int anWidth, int anHeight, const char *asTitle, bool abFullScreen);
	
	void SetVisible(bool abVisible); // TODO: unused
	bool IsVisible() const; // TODO: unused
private:
private:
	ISystem &mSystem;
	
	HWND mhWnd{nullptr};
};