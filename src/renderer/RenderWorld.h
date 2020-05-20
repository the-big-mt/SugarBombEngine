/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#pragma once

#include "renderer/IRenderWorld.hpp"
//#include "renderer/Model.h" // for idRenderModel
#include "Material.h"

#include "idlib/sys/sys_defines.h"
#include "idlib/bv/Bounds.h"
#include "idlib/math/Vector.h"
#include "idlib/math/Matrix.h"

//namespace sbe
//{

struct idRenderModel;
struct idSoundEmitter;
struct idUserInterface;
class idJointMat;
class idDeclSkin;

/*
===============================================================================

	Render World

===============================================================================
*/

// shader parms
const int SHADERPARM_RED			= 0;
const int SHADERPARM_GREEN			= 1;
const int SHADERPARM_BLUE			= 2;
const int SHADERPARM_ALPHA			= 3;
const int SHADERPARM_TIMESCALE		= 3;
const int SHADERPARM_TIMEOFFSET		= 4;
const int SHADERPARM_DIVERSITY		= 5;	// random between 0.0 and 1.0 for some effects (muzzle flashes, etc)
const int SHADERPARM_MODE			= 7;	// for selecting which shader passes to enable
const int SHADERPARM_TIME_OF_DEATH	= 7;	// for the monster skin-burn-away effect enable and time offset

// model parms
const int SHADERPARM_MD3_FRAME		= 8;
const int SHADERPARM_MD3_LASTFRAME	= 9;
const int SHADERPARM_MD3_BACKLERP	= 10;

const int SHADERPARM_BEAM_END_X		= 8;	// for _beam models
const int SHADERPARM_BEAM_END_Y		= 9;
const int SHADERPARM_BEAM_END_Z		= 10;
const int SHADERPARM_BEAM_WIDTH		= 11;

const int SHADERPARM_SPRITE_WIDTH		= 8;
const int SHADERPARM_SPRITE_HEIGHT		= 9;

const int SHADERPARM_PARTICLE_STOPTIME = 8;	// don't spawn any more particles after this time

// guis
const int MAX_RENDERENTITY_GUI		= 3;

// the renderEntity_s::joints array needs to point at enough memory to store the number of joints rounded up to two for SIMD
ID_INLINE int SIMD_ROUND_JOINTS( int numJoints )
{
	return ( ( numJoints + 1 ) & ~1 );
}
ID_INLINE void SIMD_INIT_LAST_JOINT( idJointMat* joints, int numJoints )
{
	if( numJoints & 1 )
	{
		joints[numJoints] = joints[numJoints - 1];
	}
}

typedef bool( *deferredEntityCallback_t )( renderEntity_s*, const renderView_s* );

typedef struct renderEntity_s
{
	idRenderModel* 			hModel;				// this can only be null if callback is set
	
	int						entityNum;
	int						bodyId;
	
	// Entities that are expensive to generate, like skeletal models, can be
	// deferred until their bounds are found to be in view, in the frustum
	// of a shadowing light that is in view, or contacted by a trace / overlay test.
	// This is also used to do visual cueing on items in the view
	// The renderView may be NULL if the callback is being issued for a non-view related
	// source.
	// The callback function should clear renderEntity->callback if it doesn't
	// want to be called again next time the entity is referenced (ie, if the
	// callback has now made the entity valid until the next updateEntity)
	idBounds				bounds;					// only needs to be set for deferred models and md5s
	deferredEntityCallback_t	callback;
	
	void* 					callbackData;			// used for whatever the callback wants
	
	// player bodies and possibly player shadows should be suppressed in views from
	// that player's eyes, but will show up in mirrors and other subviews
	// security cameras could suppress their model in their subviews if we add a way
	// of specifying a view number for a remoteRenderMap view
	int						suppressSurfaceInViewID;
	int						suppressShadowInViewID;
	
	// world models for the player and weapons will not cast shadows from view weapon
	// muzzle flashes
	int						suppressShadowInLightID;
	
	// if non-zero, the surface and shadow (if it casts one)
	// will only show up in the specific view, ie: player weapons
	int						allowSurfaceInViewID;
	
	// positioning
	// axis rotation vectors must be unit length for many
	// R_LocalToGlobal functions to work, so don't scale models!
	// axis vectors are [0] = forward, [1] = left, [2] = up
	idVec3					origin;
	idMat3					axis;
	
	// texturing
	const idMaterial* 		customShader;			// if non-0, all surfaces will use this
	const idMaterial* 		referenceShader;		// used so flares can reference the proper light shader
	const idDeclSkin* 		customSkin;				// 0 for no remappings
	class idSoundEmitter* 	referenceSound;			// for shader sound tables, allowing effects to vary with sounds
	float					shaderParms[ MAX_ENTITY_SHADER_PARMS ];	// can be used in any way by shader or model generation
	
	// networking: see WriteGUIToSnapshot / ReadGUIFromSnapshot
	class idUserInterface* gui[ MAX_RENDERENTITY_GUI ];
	
	struct renderView_s*		remoteRenderView;		// any remote camera surfaces will use this
	
	int						numJoints;
	idJointMat* 			joints;					// array of joints that will modify vertices.
	// NULL if non-deformable model.  NOT freed by renderer
	
	float					modelDepthHack;			// squash depth range so particle effects don't clip into walls
	
	// options to override surface shader flags (replace with material parameters?)
	bool					noSelfShadow;			// cast shadows onto other objects,but not self
	bool					noShadow;				// no shadow at all
	
	bool					noDynamicInteractions;	// don't create any light / shadow interactions after
	// the level load is completed.  This is a performance hack
	// for the gigantic outdoor meshes in the monorail map, so
	// all the lights in the moving monorail don't touch the meshes
	
	bool					weaponDepthHack;		// squash depth range so view weapons don't poke into walls
	// this automatically implies noShadow
	bool					noOverlays;				// force no overlays on this model
	bool					skipMotionBlur;			// Mask out this object during motion blur
	int						forceUpdate;			// force an update (NOTE: not a bool to keep this struct a multiple of 4 bytes)
	int						timeGroup;
	int						xrayIndex;
} renderEntity_t;

typedef struct renderLight_s
{
	idMat3					axis;				// rotation vectors, must be unit length
	idVec3					origin;
	
	// if non-zero, the light will not show up in the specific view,
	// which may be used if we want to have slightly different muzzle
	// flash lights for the player and other views
	int						suppressLightInViewID;
	
	// if non-zero, the light will only show up in the specific view
	// which can allow player gun gui lights and such to not effect everyone
	int						allowLightInViewID;
	
	// I am sticking the four bools together so there are no unused gaps in
	// the padded structure, which could confuse the memcmp that checks for redundant
	// updates
	bool					forceShadows;		// Used to override the material parameters
	bool					noShadows;			// (should we replace this with material parameters on the shader?)
	bool					noSpecular;			// (should we replace this with material parameters on the shader?)
	
	bool					pointLight;			// otherwise a projection light (should probably invert the sense of this, because points are way more common)
	bool					parallel;			// lightCenter gives the direction to the light at infinity
	idVec3					lightRadius;		// xyz radius for point lights
	idVec3					lightCenter;		// offset the lighting direction for shading and
	// shadows, relative to origin
	
	// frustum definition for projected lights, all reletive to origin
	// FIXME: we should probably have real plane equations here, and offer
	// a helper function for conversion from this format
	idVec3					target;
	idVec3					right;
	idVec3					up;
	idVec3					start;
	idVec3					end;
	
	// Dmap will generate an optimized shadow volume named _prelight_<lightName>
	// for the light against all the _area* models in the map.  The renderer will
	// ignore this value if the light has been moved after initial creation
	idRenderModel* 			prelightModel;
	
	// muzzle flash lights will not cast shadows from player and weapon world models
	int						lightId;
	
	
	const idMaterial* 		shader;				// NULL = either lights/defaultPointLight or lights/defaultProjectedLight
	float					shaderParms[MAX_ENTITY_SHADER_PARMS];		// can be used in any way by shader
	idSoundEmitter* 		referenceSound;		// for shader sound tables, allowing effects to vary with sounds
} renderLight_t;

typedef struct renderView_s
{
	// player views will set this to a non-zero integer for model suppress / allow
	// subviews (mirrors, cameras, etc) will always clear it to zero
	int						viewID;
	
	float					fov_x, fov_y;		// in degrees
	idVec3					vieworg;			// has already been adjusted for stereo world seperation
	idVec3					vieworg_weapon;		// has already been adjusted for stereo world seperation
	idMat3					viewaxis;			// transformation matrix, view looks down the positive X axis
	
	bool					cramZNear;			// for cinematics, we want to set ZNear much lower
	bool					flipProjection;
	bool					forceUpdate;		// for an update
	
	// time in milliseconds for shader effects and other time dependent rendering issues
	int						time[2];
	float					shaderParms[MAX_GLOBAL_SHADER_PARMS];		// can be used in any way by shader
	const idMaterial*		globalMaterial;							// used to override everything draw
	
	// the viewEyeBuffer may be of a different polarity than stereoScreenSeparation if the eyes have been swapped
	int						viewEyeBuffer;				// -1 = left eye, 1 = right eye, 0 = monoscopic view or GUI
	float					stereoScreenSeparation;		// projection matrix horizontal offset, positive or negative based on camera eye
} renderView_t;

// modelTrace_t is for tracing vs. visual geometry
typedef struct modelTrace_s
{
	float					fraction;			// fraction of trace completed
	idVec3					point;				// end point of trace in global space
	idVec3					normal;				// hit triangle normal vector in global space
	const idMaterial* 		material;			// material of hit surface
	const renderEntity_t* 	entity;				// render entity that was hit
	int						jointNumber;		// md5 joint nearest to the hit triangle
} modelTrace_t;

//} // namespace sbe