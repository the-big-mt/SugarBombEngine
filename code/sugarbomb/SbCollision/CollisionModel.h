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

/// @file

#pragma once

//#include "../idlib/math/Vector.h"
//#include "../idlib/MapFile.h"
//#include "../renderer/Material.h"       // for idMaterial, etc

//namespace BFG
//{

class idMaterial;

// contact type
typedef enum
{
	CONTACT_NONE,							// no contact
	CONTACT_EDGE,							// trace model edge hits model edge
	CONTACT_MODELVERTEX,					// model vertex hits trace model polygon
	CONTACT_TRMVERTEX						// trace model vertex hits model polygon
} contactType_t;

// contact info
typedef struct
{
	contactType_t			type;			// contact type
	idVec3					point;			// point of contact
	idVec3					normal;			// contact plane normal
	float					dist;			// contact plane distance
	int						contents;		// contents at other side of surface
	const idMaterial* 		material;		// surface material
	int						modelFeature;	// contact feature on model
	int						trmFeature;		// contact feature on trace model
	int						entityNum;		// entity the contact surface is a part of
	int						id;				// id of clip model the contact surface is part of
} contactInfo_t;

// trace result
typedef struct trace_s
{
	float					fraction;		// fraction of movement completed, 1.0 = didn't hit anything
	idVec3					endpos;			// final position of trace model
	idMat3					endAxis;		// final axis of trace model
	contactInfo_t			c;				// contact information, only valid if fraction < 1.0
} trace_t;

#define CM_CLIP_EPSILON		0.25f			// always stay this distance away from any model
#define CM_BOX_EPSILON		1.0f			// should always be larger than clip epsilon
#define CM_MAX_TRACE_DIST	4096.0f			// maximum distance a trace model may be traced, point traces are unlimited

#include "SbCollision/ICollisionModelManager.hpp"

extern idCollisionModelManager* 		collisionModelManager;

//} // namespace BFG