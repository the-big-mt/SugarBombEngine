/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 SugarBombEngine Developers

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

#ifndef __WIN_LOCAL_H__
#define __WIN_LOCAL_H__

#include <windows.h>

// RB: replaced QGL with GLEW
#include "../../libs/glew/include/GL/wglew.h" // windows OpenGL extensions
// RB end

//#include "framework/ICVarSystem.hpp"
#include "win_input.h"

//#include <dinput.h>

//namespace sbe
//{

#define	WINDOW_STYLE	(WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE | WS_THICKFRAME)

void	Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void* ptr, int inputDeviceNum );

void	Sys_CreateConsole();
void	Sys_DestroyConsole();

char*	Sys_ConsoleInput();
char*	Sys_GetCurrentUser();

void	Win_SetErrorText( const char* text );

cpuid_t	Sys_GetCPUId();

// Input subsystem

void	IN_Init();
void	IN_Shutdown();
// add additional non keyboard / non mouse movement on top of the keyboard move cmd

void	IN_DeactivateMouseIfWindowed();
void	IN_DeactivateMouse();
void	IN_ActivateMouse();

void	IN_Frame();

void	DisableTaskKeys( BOOL bDisable, BOOL bBeep, BOOL bTaskMgr );

uint64 Sys_Microseconds();

// window procedure
LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void Conbuf_AppendText( const char* msg );

struct Win32Vars_t
{
	HWND			hWnd;
	HINSTANCE		hInstance;
	
	bool			activeApp;			// changed with WM_ACTIVATE messages
	bool			mouseReleased;		// when the game has the console down or is doing a long operation
	bool			movingWindow;		// inhibit mouse grab when dragging the window
	bool			mouseGrabbed;		// current state of grab and hide
	
	OSVERSIONINFOEX	osversion;
	
	cpuid_t			cpuid;
	
	// when we get a windows message, we store the time off so keyboard processing
	// can know the exact time of an event (not really needed now that we use async direct input)
	int				sysMsgTime;
	
	bool			windowClassRegistered;
	
	WNDPROC			wndproc;
	
	HDC				hDC;							// handle to device context
	HGLRC			hGLRC;						// handle to GL rendering context
	PIXELFORMATDESCRIPTOR pfd;
	int				pixelformat;
	
	HINSTANCE		hinstOpenGL;	// HINSTANCE for the OpenGL library
	
	int				desktopBitsPixel;
	int				desktopWidth, desktopHeight;
	
	int				cdsFullscreen;	// 0 = not fullscreen, otherwise monitor number
	
	idFileHandle	log_fp;
	
	unsigned short	oldHardwareGamma[3][256];
	// desktop gamma is saved here for restoration at exit
	
	static idCVar	sys_arch;
	static idCVar	sys_cpustring;
	static idCVar	in_mouse;
	static idCVar	win_allowAltTab;
	static idCVar	win_notaskkeys;
	static idCVar	win_username;
	static idCVar	win_outputEditString;
	static idCVar	win_viewlog;
	static idCVar	win_timerUpdate;
	static idCVar	win_allowMultipleInstances;
	
	CRITICAL_SECTION criticalSections[MAX_CRITICAL_SECTIONS];
};

extern Win32Vars_t	win32;

//} // namespace sbe

#endif /* !__WIN_LOCAL_H__ */