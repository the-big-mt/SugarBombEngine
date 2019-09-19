/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 BlackPhrase

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

#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

// RB: required for SWF extensions
#include "rapidjson/document.h"

#include "sys/sys_defines.h"
#include "sys/sys_builddefines.h"
#include <algorithm>	// for min / max / swap
#include "sys/sys_includes.h" // NOTE: this header will include the windows header on Windows platform which will define __in and __out as blank but they're required by cstdlib++
#include "sys/sys_assert.h"
#include "sys/sys_types.h"
#include "sys/sys_intrinsics.h"
#include "sys/sys_threading.h"

//-----------------------------------------------------

// non-portable system services
#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbNetwork/NetworkTypes.hpp"

// id lib
#include "Lib.h"

#include "sys/sys_filesystem.h"

// framework
#include "framework/ICmdSystem.hpp"
#include "framework/ICVarSystem.hpp"
#include "framework/ICommon.hpp"
#include "framework/Serializer.h"

#include "CoreLibs/SbSystem/IFile.hpp"
#include "CoreLibs/SbSystem/IFileSystem.hpp"

// renderer

// RB: replaced QGL with GLEW
#include <GL/glew.h>
// RB end

// tools
//#include "../tools/compilers/compiler_public.h"

//-----------------------------------------------------

#undef min
#undef max

#endif /* !__PRECOMPILED_H__ */
