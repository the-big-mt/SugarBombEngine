/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 
Copyright (C) 2019-2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include <stdexcept>

#include <windows.h>

#include "SbWindowWin.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe
{

constexpr auto WINDOW_STYLE{WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE | WS_THICKFRAME};

LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 1;
	};
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};

/*
=======================
GLW_CreateWindow

Responsible for creating the Win32 window.
If fullscreen, it won't have a border
=======================
*/
//ahInstance = win32.hInstance
//asWindowName = GAME_NAME
//asClassName = WIN32_WINDOW_CLASS_NAME
//abFullScreen = parms.fullScreen
SbWindowWin::SbWindowWin(LPCTSTR asWindowName, const char *asClassName, HINSTANCE ahInstance, ISystem &aSystem, int anWidth, int anHeight, bool abFullScreen)
	: mSystem(aSystem)
{
	// create our window class if we haven't already
	CreateWindowClass(ahInstance, asClassName);
	
	int stylebits{WINDOW_STYLE | WS_SYSMENU};
	int exstyle{0};
	
	if(abFullScreen)
	{
		stylebits = WS_POPUP | WS_VISIBLE | WS_SYSMENU;
		exstyle = WS_EX_TOPMOST;
	};
	
	int x = 0;
	int y = 0;
	
	mhWnd = CreateWindowEx(exstyle, asClassName, asWindowName, stylebits, x, y, anWidth, anHeight, nullptr, nullptr, ahInstance, nullptr);
	
	if(!mhWnd)
	{
		mSystem.Printf("^3GLW_CreateWindow() - Couldn't create window^0\n");
		throw std::runtime_error("Couldn't create window!");
	};
	
	::SetTimer(mhWnd, 0, 100, nullptr);
	
	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	mSystem.Printf("...created window @ %d,%d (%dx%d)\n", x, y, anWidth, anHeight);
	
	SetForegroundWindow(mhWnd);
	SetFocus(mhWnd);
};

SbWindowWin::~SbWindowWin()
{
	// destroy window
	if(mhWnd)
	{
		mSystem.Printf("...destroying window\n");
		ShowWindow(mhWnd, SW_HIDE);
		DestroyWindow(mhWnd);
		mhWnd = nullptr;
	};
};

/*
==============
Sys_ShowWindow
==============
*/
void SbWindowWin::SetVisible(bool abVisible)
{
	::ShowWindow(mhWnd, abVisible ? SW_SHOW : SW_HIDE);
};

/*
==============
Sys_IsWindowVisible
==============
*/
bool SbWindowWin::IsVisible() const
{
	return (::IsWindowVisible(mhWnd) != 0);
};

void SbWindowWin::CreateWindowClass(HINSTANCE ahInstance, const char *asClassName)
{
	WNDCLASS wc{};
	
	//
	// register the window class if necessary
	//
	//if( win32.windowClassRegistered )
		//return;
	
	memset(&wc, 0, sizeof(wc));
	
	wc.style         = 0;
	wc.lpfnWndProc   = ( WNDPROC ) MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = ahInstance;
	wc.hIcon         = LoadIcon( ahInstance, IDI_APPLICATION); // MAKEINTRESOURCE( IDI_ICON1 ) ); // TODO
	wc.hCursor       = nullptr;
	wc.hbrBackground = ( struct HBRUSH__* )COLOR_GRAYTEXT;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = asClassName;
	
	if( !RegisterClass( &wc ) )
		mSystem.FatalError( "GLW_CreateWindow: could not register window class" );

	mSystem.Printf( "...registered window class\n" );
	
	//win32.windowClassRegistered = true;
};

}; // namespace sbe