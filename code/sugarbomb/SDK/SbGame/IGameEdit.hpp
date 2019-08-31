/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
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

#pragma once

/*
===============================================================================

	Public game interface with methods for in-game editing.

===============================================================================
*/

class idEntity;
class idMD5Anim;

struct refSound_t
{
	idSoundEmitter* 			referenceSound;	// this is the interface to the sound system, created
	// with idSoundWorld::AllocSoundEmitter() when needed
	idVec3						origin;
	int							listenerId;		// SSF_PRIVATE_SOUND only plays if == listenerId from PlaceListener
	// no spatialization will be performed if == listenerID
	const idSoundShader* 		shader;			// this really shouldn't be here, it is a holdover from single channel behavior
	float						diversity;		// 0.0 to 1.0 value used to select which
	// samples in a multi-sample list from the shader are used
	bool						waitfortrigger;	// don't start it at spawn time
	soundShaderParms_t			parms;			// override volume, flags, etc
};

// FIXME: this interface needs to be reworked but it properly separates code for the time being
class idGameEdit
{
public:
	virtual						~idGameEdit() {}
	
	// These are the canonical idDict to parameter parsing routines used by both the game and tools.
	virtual void				ParseSpawnArgsToRenderLight( const idDict* args, renderLight_t* renderLight );
	virtual void				ParseSpawnArgsToRenderEntity( const idDict* args, renderEntity_t* renderEntity );
	virtual void				ParseSpawnArgsToRefSound( const idDict* args, refSound_t* refSound );
	
	// Animation system calls for non-game based skeletal rendering.
	virtual idRenderModel* 		ANIM_GetModelFromEntityDef( const char* classname );
	virtual const idVec3&		 ANIM_GetModelOffsetFromEntityDef( const char* classname );
	virtual idRenderModel* 		ANIM_GetModelFromEntityDef( const idDict* args );
	virtual idRenderModel* 		ANIM_GetModelFromName( const char* modelName );
	virtual const idMD5Anim* 	ANIM_GetAnimFromEntityDef( const char* classname, const char* animname );
	virtual int					ANIM_GetNumAnimsFromEntityDef( const idDict* args );
	virtual const char* 		ANIM_GetAnimNameFromEntityDef( const idDict* args, int animNum );
	virtual const idMD5Anim* 	ANIM_GetAnim( const char* fileName );
	virtual int					ANIM_GetLength( const idMD5Anim* anim );
	virtual int					ANIM_GetNumFrames( const idMD5Anim* anim );
	virtual void				ANIM_CreateAnimFrame( const idRenderModel* model, const idMD5Anim* anim, int numJoints, idJointMat* frame, int time, const idVec3& offset, bool remove_origin_offset );
	virtual idRenderModel* 		ANIM_CreateMeshForAnim( idRenderModel* model, const char* classname, const char* animname, int frame, bool remove_origin_offset );
	
	// Articulated Figure calls for AF editor and Radiant.
	virtual bool				AF_SpawnEntity( const char* fileName );
	virtual void				AF_UpdateEntities( const char* fileName );
	virtual void				AF_UndoChanges();
	virtual idRenderModel* 		AF_CreateMesh( const idDict& args, idVec3& meshOrigin, idMat3& meshAxis, bool& poseIsSet );
	
	
	// Entity selection.
	virtual void				ClearEntitySelection();
	virtual int					GetSelectedEntities( idEntity* list[], int max );
	virtual void				AddSelectedEntity( idEntity* ent );
	
	// Selection methods
	virtual void				TriggerSelected();
	
	// Entity defs and spawning.
	virtual const idDict* 		FindEntityDefDict( const char* name, bool makeDefault = true ) const;
	virtual void				SpawnEntityDef( const idDict& args, idEntity** ent );
	virtual idEntity* 			FindEntity( const char* name ) const;
	virtual const char* 		GetUniqueEntityName( const char* classname ) const;
	
	// Entity methods.
	virtual void				EntityGetOrigin( idEntity* ent, idVec3& org ) const;
	virtual void				EntityGetAxis( idEntity* ent, idMat3& axis ) const;
	virtual void				EntitySetOrigin( idEntity* ent, const idVec3& org );
	virtual void				EntitySetAxis( idEntity* ent, const idMat3& axis );
	virtual void				EntityTranslate( idEntity* ent, const idVec3& org );
	virtual const idDict* 		EntityGetSpawnArgs( idEntity* ent ) const;
	virtual void				EntityUpdateChangeableSpawnArgs( idEntity* ent, const idDict* dict );
	virtual void				EntityChangeSpawnArgs( idEntity* ent, const idDict* newArgs );
	virtual void				EntityUpdateVisuals( idEntity* ent );
	virtual void				EntitySetModel( idEntity* ent, const char* val );
	virtual void				EntityStopSound( idEntity* ent );
	virtual void				EntityDelete( idEntity* ent );
	virtual void				EntitySetColor( idEntity* ent, const idVec3 color );
	
	// Player methods.
	virtual bool				PlayerIsValid() const;
	virtual void				PlayerGetOrigin( idVec3& org ) const;
	virtual void				PlayerGetAxis( idMat3& axis ) const;
	virtual void				PlayerGetViewAngles( idAngles& angles ) const;
	virtual void				PlayerGetEyePosition( idVec3& org ) const;
	
	// In game map editing support.
	virtual const idDict* 		MapGetEntityDict( const char* name ) const;
	virtual void				MapSave( const char* path = nullptr ) const;
	virtual void				MapSetEntityKeyVal( const char* name, const char* key, const char* val ) const ;
	virtual void				MapCopyDictToEntity( const char* name, const idDict* dict ) const;
	virtual int					MapGetUniqueMatchingKeyVals( const char* key, const char* list[], const int max ) const;
	virtual void				MapAddEntity( const idDict* dict ) const;
	virtual int					MapGetEntitiesMatchingClassWithString( const char* classname, const char* match, const char* list[], const int max ) const;
	virtual void				MapRemoveEntity( const char* name ) const;
	virtual void				MapEntityTranslate( const char* name, const idVec3& v ) const;
	
};