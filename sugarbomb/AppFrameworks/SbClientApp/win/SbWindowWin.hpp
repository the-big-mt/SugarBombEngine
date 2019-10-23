/// @file

#pragma once

class SbWindowWin : public IWindow
{
public:
	SbWindowWin(int anWidth, int anHeight, const char *asTitle, bool abFullScreen);
	
	void SetVisible(bool abVisible); // TODO: unused
	bool IsVisible() const; // TODO: unused
private:
	HWND mhWnd{nullptr};
};