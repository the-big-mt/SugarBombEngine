/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
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

#pragma once

/*
================================================================================================

	Minimum set of headers needed to compile the code for a job.

================================================================================================
*/

#include "sys/sys_defines.h"

#include <cstddef>					// for offsetof
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

// RB: added <stdint.h> for missing uintptr_t
#include <stdint.h>

// RB begin
#if defined(__MINGW32__)
//#include <sal.h> 	// RB: missing __analysis_assume
#include <malloc.h> // DG: _alloca16 needs that

#ifndef __analysis_assume
#define __analysis_assume( x )
#endif

#elif defined(__linux__)
#include <malloc.h> // DG: _alloca16 needs that
#include <csignal>
// RB end
// Yamagi begin
#elif defined(__FreeBSD__)
#include <csignal>
#endif
// Yamagi end

#ifdef _MSC_VER
#include <intrin.h>
#pragma warning( disable : 4100 )	// unreferenced formal parameter
#pragma warning( disable : 4127 )	// conditional expression is constant
#endif

#include "sys/sys_assert.h"
#include "sys/sys_types.h"
#include "sys/sys_intrinsics.h"
#include "math/Math.h"
#include "ParallelJobList.h"

//*****************************************************************************