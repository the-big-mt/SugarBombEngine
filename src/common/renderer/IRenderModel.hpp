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

#pragma once

#include "idlib/sys/sys_types.h"
#include "idlib/bv/Bounds.h"
#include "idlib/containers/List.h"

#include "renderer/VertexCache.h"

/*
===============================================================================

	Render Model

===============================================================================
*/

// the init methods may be called again on an already created model when
// a reloadModels is issued

struct idFile;
struct viewDef_t;
struct silEdge_t;
class idMaterial;
class idMD5Joint;
class idJointQuat;
class idDemoFile;
class idDrawVert;
class idShadowVert;
class idRenderModelStatic;

enum dynamicModel_t
{
	DM_STATIC,		// never creates a dynamic model
	DM_CACHED,		// once created, stays constant until the entity is updated (animating characters)
	DM_CONTINUOUS	// must be recreated for every single view (time dependent things like particles)
};

enum jointHandle_t
{
	INVALID_JOINT				= -1
};

struct modelSurface_t
{
	int							id;
	const idMaterial* 			shader;
	srfTriangles_t* 			geometry;
};

class idRenderModel
{
public:
	virtual						~idRenderModel() {};
	
	// Loads static models only, dynamic models must be loaded by the modelManager
	virtual void				InitFromFile( const char* fileName ) = 0;
	
	// Supports reading/writing binary file formats
	virtual bool				LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp ) = 0;
	virtual void				WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const = 0;
	virtual bool				SupportsBinaryModel() = 0;
	
	// RB begin
	virtual void				ExportOBJ( idFile* objFile, idFile* mtlFile, ID_TIME_T* _timeStamp = NULL ) const = 0;
	// RB end
	
	// renderBump uses this to load the very high poly count models, skipping the
	// shadow and tangent generation, along with some surface cleanup to make it load faster
	virtual void				PartialInitFromFile( const char* fileName ) = 0;
	
	// this is used for dynamically created surfaces, which are assumed to not be reloadable.
	// It can be called again to clear out the surfaces of a dynamic model for regeneration.
	virtual void				InitEmpty( const char* name ) = 0;
	
	// dynamic model instantiations will be created with this
	// the geometry data will be owned by the model, and freed when it is freed
	// the geoemtry should be raw triangles, with no extra processing
	virtual void				AddSurface( modelSurface_t surface ) = 0;
	
	// cleans all the geometry and performs cross-surface processing
	// like shadow hulls
	// Creates the duplicated back side geometry for two sided, alpha tested, lit materials
	// This does not need to be called if none of the surfaces added with AddSurface require
	// light interaction, and all the triangles are already well formed.
	virtual void				FinishSurfaces() = 0;
	
	// frees all the data, but leaves the class around for dangling references,
	// which can regenerate the data with LoadModel()
	virtual void				PurgeModel() = 0;
	
	// resets any model information that needs to be reset on a same level load etc..
	// currently only implemented for liquids
	virtual void				Reset() = 0;
	
	// used for initial loads, reloadModel, and reloading the data of purged models
	// Upon exit, the model will absolutely be valid, but possibly as a default model
	virtual void				LoadModel() = 0;
	
	// internal use
	virtual bool				IsLoaded() const = 0;
	virtual void				SetLevelLoadReferenced( bool referenced ) = 0;
	virtual bool				IsLevelLoadReferenced() = 0;
	
	// models that are already loaded at level start time
	// will still touch their data to make sure they
	// are kept loaded
	virtual void				TouchData() = 0;
	
	// dump any ambient caches on the model surfaces
	virtual void				FreeVertexCache() = 0;
	
	// returns the name of the model
	virtual const char*			Name() const = 0;
	
	// prints a detailed report on the model for printModel
	virtual void				Print() const = 0;
	
	// prints a single line report for listModels
	virtual void				List() const = 0;
	
	// reports the amount of memory (roughly) consumed by the model
	virtual int					Memory() const = 0;
	
	// for reloadModels
	virtual ID_TIME_T			Timestamp() const = 0;
	
	// returns the number of surfaces
	virtual int					NumSurfaces() const = 0;
	
	// NumBaseSurfaces will not count any overlays added to dynamic models
	virtual int					NumBaseSurfaces() const = 0;
	
	// get a pointer to a surface
	virtual const modelSurface_t* Surface( int surfaceNum ) const = 0;
	
	// Allocates surface triangles.
	// Allocates memory for srfTriangles_t::verts and srfTriangles_t::indexes
	// The allocated memory is not initialized.
	// srfTriangles_t::numVerts and srfTriangles_t::numIndexes are set to zero.
	virtual srfTriangles_t* 	AllocSurfaceTriangles( int numVerts, int numIndexes ) const = 0;
	
	// Frees surfaces triangles.
	virtual void				FreeSurfaceTriangles( srfTriangles_t* tris ) const = 0;
	
	// models of the form "_area*" may have a prelight shadow model associated with it
	virtual bool				IsStaticWorldModel() const = 0;
	
	// models parsed from inside map files or dynamically created cannot be reloaded by
	// reloadmodels
	virtual bool				IsReloadable() const = 0;
	
	// md3, md5, particles, etc
	virtual dynamicModel_t		IsDynamicModel() const = 0;
	
	// if the load failed for any reason, this will return true
	virtual bool				IsDefaultModel() const = 0;
	
	// dynamic models should return a fast, conservative approximation
	// static models should usually return the exact value
	virtual idBounds			Bounds( const struct renderEntity_s* ent = NULL ) const = 0;
	
	// returns value != 0.0f if the model requires the depth hack
	virtual float				DepthHack() const = 0;
	
	// returns a static model based on the definition and view
	// currently, this will be regenerated for every view, even though
	// some models, like character meshes, could be used for multiple (mirror)
	// views in a frame, or may stay static for multiple frames (corpses)
	// The renderer will delete the returned dynamic model the next view
	// This isn't const, because it may need to reload a purged model if it
	// wasn't precached correctly.
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel ) = 0;
	
	// Returns the number of joints or 0 if the model is not an MD5
	virtual int					NumJoints() const = 0;
	
	// Returns the MD5 joints or NULL if the model is not an MD5
	virtual const idMD5Joint* 	GetJoints() const = 0;
	
	// Returns the handle for the joint with the given name.
	virtual jointHandle_t		GetJointHandle( const char* name ) const = 0;
	
	// Returns the name for the joint with the given handle.
	virtual const char* 		GetJointName( jointHandle_t handle ) const = 0;
	
	// Returns the default animation pose or NULL if the model is not an MD5.
	virtual const idJointQuat* 	GetDefaultPose() const = 0;
	
	// Returns number of the joint nearest to the given triangle.
	virtual int					NearestJoint( int surfaceNum, int a, int c, int b ) const = 0;
	
	// Writing to and reading from a demo file.
	virtual void				ReadFromDemoFile( class idDemoFile* f ) = 0;
	virtual void				WriteToDemoFile( class idDemoFile* f ) = 0;
	
	// if false, the model doesn't need to be linked into the world, because it
	// can't contribute visually -- triggers, etc
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	
	// if false, the model doesn't generate interactions with lights
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return true;
	};
	
	// if false, the model doesn't need to be added to the view unless it is
	// directly visible, because it can't cast shadows into the view
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return true;
	};
};