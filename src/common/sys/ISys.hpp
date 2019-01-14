/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

/*
===============================================================================

	Non-portable system services.

===============================================================================
*/

enum cpuid_t
{
	CPUID_NONE							= 0x00000,
	CPUID_UNSUPPORTED					= 0x00001,	// unsupported (386/486)
	CPUID_GENERIC						= 0x00002,	// unrecognized processor
	CPUID_INTEL							= 0x00004,	// Intel
	CPUID_AMD							= 0x00008,	// AMD
	CPUID_MMX							= 0x00010,	// Multi Media Extensions
	CPUID_3DNOW							= 0x00020,	// 3DNow!
	CPUID_SSE							= 0x00040,	// Streaming SIMD Extensions
	CPUID_SSE2							= 0x00080,	// Streaming SIMD Extensions 2
	CPUID_SSE3							= 0x00100,	// Streaming SIMD Extentions 3 aka Prescott's New Instructions
	CPUID_ALTIVEC						= 0x00200,	// AltiVec
	CPUID_HTT							= 0x01000,	// Hyper-Threading Technology
	CPUID_CMOV							= 0x02000,	// Conditional Move (CMOV) and fast floating point comparison (FCOMI) instructions
	CPUID_FTZ							= 0x04000,	// Flush-To-Zero mode (denormal results are flushed to zero)
	CPUID_DAZ							= 0x08000,	// Denormals-Are-Zero mode (denormal source operands are set to zero)
	CPUID_XENON							= 0x10000,	// Xbox 360
	CPUID_CELL							= 0x20000	// PS3
};

/*
================================================
The first part of this table maps directly to Direct Input scan codes (DIK_* from dinput.h)
But they are duplicated here for console portability
================================================
*/
enum keyNum_t
{
	K_NONE,
	
	K_ESCAPE,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,
	K_0,
	K_MINUS,
	K_EQUALS,
	K_BACKSPACE,
	K_TAB,
	K_Q,
	K_W,
	K_E,
	K_R,
	K_T,
	K_Y,
	K_U,
	K_I,
	K_O,
	K_P,
	K_LBRACKET,
	K_RBRACKET,
	K_ENTER,
	K_LCTRL,
	K_A,
	K_S,
	K_D,
	K_F,
	K_G,
	K_H,
	K_J,
	K_K,
	K_L,
	K_SEMICOLON,
	K_APOSTROPHE,
	K_GRAVE,
	K_LSHIFT,
	K_BACKSLASH,
	K_Z,
	K_X,
	K_C,
	K_V,
	K_B,
	K_N,
	K_M,
	K_COMMA,
	K_PERIOD,
	K_SLASH,
	K_RSHIFT,
	K_KP_STAR,
	K_LALT,
	K_SPACE,
	K_CAPSLOCK,
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_NUMLOCK,
	K_SCROLL,
	K_KP_7,
	K_KP_8,
	K_KP_9,
	K_KP_MINUS,
	K_KP_4,
	K_KP_5,
	K_KP_6,
	K_KP_PLUS,
	K_KP_1,
	K_KP_2,
	K_KP_3,
	K_KP_0,
	K_KP_DOT,
	K_OEM_102		= 0x56, // from dinput: < > | on UK/German keyboards
	K_F11			= 0x57,
	K_F12			= 0x58,
	K_F13			= 0x64,
	K_F14			= 0x65,
	K_F15			= 0x66,
	K_KANA			= 0x70,
	K_ABNT_C1		= 0x7E, // from dinput: ? on Portugese (Brazilian) keyboards
	K_CONVERT		= 0x79,
	K_NOCONVERT		= 0x7B,
	K_YEN			= 0x7D,
	K_KP_EQUALS		= 0x8D,
	K_CIRCUMFLEX	= 0x90, // this is circumflex on japanese keyboards, ..
	K_PREVTRACK		= 0x90, // from dinput: .. but also "Previous Track"
	K_AT			= 0x91,
	K_COLON			= 0x92,
	K_UNDERLINE		= 0x93,
	K_KANJI			= 0x94,
	K_STOP			= 0x95,
	K_AX			= 0x96,
	K_UNLABELED		= 0x97,
	K_NEXTTRACK		= 0x99, // from dinput
	K_KP_ENTER		= 0x9C,
	K_RCTRL			= 0x9D,
	// some more from dinput:
	K_MUTE          = 0xA0,
	K_CALCULATOR    = 0xA1,
	K_PLAYPAUSE     = 0xA2,
	K_MEDIASTOP     = 0xA4,
	K_VOLUMEDOWN    = 0xAE,
	K_VOLUMEUP      = 0xB0,
	K_WEBHOME       = 0xB2,
	
	K_KP_COMMA		= 0xB3,
	K_KP_SLASH		= 0xB5,
	K_PRINTSCREEN	= 0xB7, // aka SysRq
	K_RALT			= 0xB8,
	K_PAUSE			= 0xC5,
	K_HOME			= 0xC7,
	K_UPARROW		= 0xC8,
	K_PGUP			= 0xC9,
	K_LEFTARROW		= 0xCB,
	K_RIGHTARROW	= 0xCD,
	K_END			= 0xCF,
	K_DOWNARROW		= 0xD0,
	K_PGDN			= 0xD1,
	K_INS			= 0xD2,
	K_DEL			= 0xD3,
	K_LWIN			= 0xDB,
	K_RWIN			= 0xDC,
	K_APPS			= 0xDD,
	K_POWER			= 0xDE,
	K_SLEEP			= 0xDF,
	
	// DG: dinput has some more buttons, let's support them as well
	K_WAKE			= 0xE3,
	K_WEBSEARCH		= 0xE5,
	K_WEBFAVORITES	= 0xE6,
	K_WEBREFRESH	= 0xE7,
	K_WEBSTOP		= 0xE8,
	K_WEBFORWARD	= 0xE9,
	K_WEBBACK		= 0xEA,
	K_MYCOMPUTER	= 0xEB,
	K_MAIL			= 0xEC,
	K_MEDIASELECT	= 0xED,
	
	//------------------------
	// K_JOY codes must be contiguous, too
	//------------------------
	
	K_JOY1 = 256,
	K_JOY2,
	K_JOY3,
	K_JOY4,
	K_JOY5,
	K_JOY6,
	K_JOY7,
	K_JOY8,
	K_JOY9,
	K_JOY10,
	K_JOY11,
	K_JOY12,
	K_JOY13,
	K_JOY14,
	K_JOY15,
	K_JOY16,
	
	K_JOY_STICK1_UP,
	K_JOY_STICK1_DOWN,
	K_JOY_STICK1_LEFT,
	K_JOY_STICK1_RIGHT,
	
	K_JOY_STICK2_UP,
	K_JOY_STICK2_DOWN,
	K_JOY_STICK2_LEFT,
	K_JOY_STICK2_RIGHT,
	
	K_JOY_TRIGGER1,
	K_JOY_TRIGGER2,
	
	K_JOY_DPAD_UP,
	K_JOY_DPAD_DOWN,
	K_JOY_DPAD_LEFT,
	K_JOY_DPAD_RIGHT,
	
	//------------------------
	// K_MOUSE enums must be contiguous (no char codes in the middle)
	//------------------------
	
	K_MOUSE1, // left
	K_MOUSE2, // right
	K_MOUSE3, // middle
	K_MOUSE4,
	K_MOUSE5,
	K_MOUSE6,
	K_MOUSE7,
	K_MOUSE8,
	
	// DG: add some more mouse buttons
	K_MOUSE9,
	K_MOUSE10,
	K_MOUSE11,
	K_MOUSE12,
	K_MOUSE13,
	K_MOUSE14,
	K_MOUSE15,
	K_MOUSE16,
	// DG end
	
	K_MWHEELDOWN,
	K_MWHEELUP,
	
	K_LAST_KEY
};

enum sysEventType_t
{
	SE_NONE,				// evTime is still valid
	SE_KEY,					// evValue is a key code, evValue2 is the down flag
	SE_CHAR,				// evValue is an Unicode UTF-32 char (or non-surrogate UTF-16)
	SE_MOUSE,				// evValue and evValue2 are relative signed x / y moves
	SE_MOUSE_ABSOLUTE,		// evValue and evValue2 are absolute coordinates in the window's client area.
	SE_MOUSE_LEAVE,			// evValue and evValue2 are meaninless, this indicates the mouse has left the client area.
	SE_JOYSTICK,			// evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE				// evPtr is a char*, from typing something at a non-game console
};

struct sysEvent_t
{
	sysEventType_t	evType;
	int				evValue;
	int				evValue2;
	int				evPtrLength;		// bytes of data pointed to by evPtr, for journaling
	void* 			evPtr;				// this must be manually freed if not NULL
	
	int				inputDevice;
	bool			IsKeyEvent() const
	{
		return evType == SE_KEY;
	}
	bool			IsMouseEvent() const
	{
		return evType == SE_MOUSE;
	}
	bool			IsCharEvent() const
	{
		return evType == SE_CHAR;
	}
	bool			IsJoystickEvent() const
	{
		return evType == SE_JOYSTICK;
	}
	bool			IsKeyDown() const
	{
		return evValue2 != 0;
	}
	keyNum_t		GetKey() const
	{
		return static_cast< keyNum_t >( evValue );
	}
	int				GetXCoord() const
	{
		return evValue;
	}
	int				GetYCoord() const
	{
		return evValue2;
	}
};

/*
==============================================================

	idSys

==============================================================
*/

struct idSys
{
	virtual void			DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	virtual void			DebugVPrintf( const char* fmt, va_list arg ) = 0;
	
	virtual double			GetClockTicks() = 0;
	virtual double			ClockTicksPerSecond() = 0;
	virtual cpuid_t			GetProcessorId() = 0;
	virtual const char* 	GetProcessorString() = 0;
	virtual const char* 	FPU_GetState() = 0;
	virtual bool			FPU_StackIsEmpty() = 0;
	virtual void			FPU_SetFTZ( bool enable ) = 0;
	virtual void			FPU_SetDAZ( bool enable ) = 0;
	
	virtual void			FPU_EnableExceptions( int exceptions ) = 0;
	
	virtual bool			LockMemory( void* ptr, int bytes ) = 0;
	virtual bool			UnlockMemory( void* ptr, int bytes ) = 0;
	
	virtual int				DLL_Load( const char* dllName ) = 0;
	virtual void* 			DLL_GetProcAddress( int dllHandle, const char* procName ) = 0;
	virtual void			DLL_Unload( int dllHandle ) = 0;
	virtual void			DLL_GetFileName( const char* baseName, char* dllName, int maxLength ) = 0;
	
	virtual sysEvent_t		GenerateMouseButtonEvent( int button, bool down ) = 0;
	virtual sysEvent_t		GenerateMouseMoveEvent( int deltax, int deltay ) = 0;
	
	virtual void			OpenURL( const char* url, bool quit ) = 0;
	virtual void			StartProcess( const char* exePath, bool quit ) = 0;
};

/*
===============================================================================

	System API.

===============================================================================
*/

const int SYS_API_VERSION		= 1;

struct idCommon;
struct idCmdSystem;
struct idCVarSystem;
struct idFileSystem;

struct sysImport_t
{
	int							version;				// API version
	idCommon* 					common;					// common
	idCmdSystem* 				cmdSystem;				// console command system
	idCVarSystem* 				cvarSystem;				// console variable system
	idFileSystem* 				fileSystem;				// file system
	idDeclManager* 				declManager;			// declaration manager
};

struct sysExport_t
{
	int							version;				// API version
	idSys* 						sys;					// non-portable system services
};

extern "C" using GetSysAPI_t = sysExport_t *(*)( sysImport_t* import );