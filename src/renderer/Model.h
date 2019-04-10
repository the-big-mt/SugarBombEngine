/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2019 BlackPhrase

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

#ifndef __MODEL_H__
#define __MODEL_H__

#include "renderer/IRenderModel.hpp"

//#include "Material.h" // for idMaterial, etc
#include "VertexCache.h"

//#include "jobs/ShadowShared.h"
//#include "jobs/prelightshadowvolume/PreLightShadowVolume.h"
//#include "jobs/staticshadowvolume/StaticShadowVolume.h"
//#include "jobs/dynamicshadowvolume/DynamicShadowVolume.h"

#include "idlib/sys/sys_types.h"
#include "idlib/sys/sys_defines.h"
#include "idlib/Str.h"
#include "idlib/containers/List.h"

//namespace BFG
//{

// shared between the renderer, game, and Maya export DLL
#define MD5_VERSION_STRING		"MD5Version"
#define MD5_MESH_EXT			"md5mesh"
#define MD5_ANIM_EXT			"md5anim"
#define MD5_CAMERA_EXT			"md5camera"
#define MD5_VERSION				10

// this is used for calculating unsmoothed normals and tangents for deformed models
struct dominantTri_t
{
	triIndex_t					v2, v3;
	float						normalizationScale[3];
};

const int SHADOW_CAP_INFINITE	= 64;

struct silEdge_t;
class idRenderModelStatic;
class idDrawVert;
class idShadowVert;

// our only drawing geometry type
struct srfTriangles_t
{
	srfTriangles_t() {}
	
	idBounds					bounds;					// for culling
	
	bool						generateNormals;		// create normals from geometry, instead of using explicit ones
	bool						tangentsCalculated;		// set when the vertex tangents have been calculated
	bool						perfectHull;			// true if there aren't any dangling edges
	bool						referencedVerts;		// if true the 'verts' are referenced and should not be freed
	bool						referencedIndexes;		// if true, indexes, silIndexes, mirrorVerts, and silEdges are
	// pointers into the original surface, and should not be freed
	
	int							numVerts;				// number of vertices
	idDrawVert* 				verts;					// vertices, allocated with special allocator
	
	int							numIndexes;				// for shadows, this has both front and rear end caps and silhouette planes
	triIndex_t* 				indexes;				// indexes, allocated with special allocator
	
	triIndex_t* 				silIndexes;				// indexes changed to be the first vertex with same XYZ, ignoring normal and texcoords
	
	int							numMirroredVerts;		// this many verts at the end of the vert list are tangent mirrors
	int* 						mirroredVerts;			// tri->mirroredVerts[0] is the mirror of tri->numVerts - tri->numMirroredVerts + 0
	
	int							numDupVerts;			// number of duplicate vertexes
	int* 						dupVerts;				// pairs of the number of the first vertex and the number of the duplicate vertex
	
	int							numSilEdges;			// number of silhouette edges
	silEdge_t* 					silEdges;				// silhouette edges
	
	dominantTri_t* 				dominantTris;			// [numVerts] for deformed surface fast tangent calculation
	
	int							numShadowIndexesNoFrontCaps;	// shadow volumes with front caps omitted
	int							numShadowIndexesNoCaps;			// shadow volumes with the front and rear caps omitted
	
	int							shadowCapPlaneBits;		// bits 0-5 are set when that plane of the interacting light has triangles
	// projected on it, which means that if the view is on the outside of that
	// plane, we need to draw the rear caps of the shadow volume
	// dynamic shadows will have SHADOW_CAP_INFINITE
	
	idShadowVert* 				preLightShadowVertexes;	// shadow vertices in CPU memory for pre-light shadow volumes
	idShadowVert* 				staticShadowVertexes;	// shadow vertices in CPU memory for static shadow volumes
	
	srfTriangles_t* 			ambientSurface;			// for light interactions, point back at the original surface that generated
	// the interaction, which we will get the ambientCache from
	
	srfTriangles_t* 			nextDeferredFree;		// chain of tris to free next frame
	
	// for deferred normal / tangent transformations by joints
	// the jointsInverted list / buffer object on md5WithJoints may be
	// shared by multiple srfTriangles_t
	idRenderModelStatic* 		staticModelWithJoints;
	
	// data in vertex object space, not directly readable by the CPU
	vertCacheHandle_t			indexCache;				// GL_INDEX_TYPE
	vertCacheHandle_t			ambientCache;			// idDrawVert
	vertCacheHandle_t			shadowCache;			// idVec4
	
	DISALLOW_COPY_AND_ASSIGN( srfTriangles_t );
};

typedef idList<srfTriangles_t*, TAG_IDLIB_LIST_TRIANGLES> idTriList;

class idMD5Joint
{
public:
	idMD5Joint()
	{
		parent = NULL;
	}
	idStr						name;
	const idMD5Joint* 			parent;
};

//} // namespace BFG

#endif /* !__MODEL_H__ */
