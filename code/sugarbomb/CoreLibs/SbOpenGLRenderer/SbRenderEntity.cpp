/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

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

#pragma hdrstop
//#include <cstring>
#include "precompiled.h"

#include "idlib/bv/Bounds.h"
#include "idlib/geometry/RenderMatrix.h"
#include "idlib/math/Plane.h"
#include "idlib/math/Vector.h"

#include "RenderCommon.h"
#include "RenderWorld.h"

//namespace BFG
//{

//class idMaterial;

idRenderEntityLocal::idRenderEntityLocal()
{
	memset(&parms, 0, sizeof(parms));
	memset(modelMatrix, 0, sizeof(modelMatrix));

	world = nullptr;
	index = 0;
	lastModifiedFrameNum = 0;
	archived = false;
	dynamicModel = nullptr;
	dynamicModelFrameCount = 0;
	cachedDynamicModel = nullptr;
	localReferenceBounds = bounds_zero;
	globalReferenceBounds = bounds_zero;
	viewCount = 0;
	viewEntity = nullptr;
	decals = nullptr;
	overlays = nullptr;
	entityRefs = nullptr;
	firstInteraction = nullptr;
	lastInteraction = nullptr;
	needsPortalSky = false;
}

void idRenderEntityLocal::FreeRenderEntity()
{
}

void idRenderEntityLocal::UpdateRenderEntity(const renderEntity_t *re, bool forceUpdate)
{
}

void idRenderEntityLocal::GetRenderEntity(renderEntity_t *re)
{
}

void idRenderEntityLocal::ForceUpdate()
{
}

int idRenderEntityLocal::GetIndex()
{
	return index;
}

void idRenderEntityLocal::ProjectOverlay(const idPlane localTextureAxis[2], const idMaterial *material)
{
}
void idRenderEntityLocal::RemoveDecals()
{
}

//======================================================================

idRenderLightLocal::idRenderLightLocal()
{
	memset(&parms, 0, sizeof(parms));
	memset(lightProject, 0, sizeof(lightProject));

	lightHasMoved = false;
	world = nullptr;
	index = 0;
	areaNum = 0;
	lastModifiedFrameNum = 0;
	archived = false;
	lightShader = nullptr;
	falloffImage = nullptr;
	globalLightOrigin = vec3_zero;
	viewCount = 0;
	viewLight = nullptr;
	references = nullptr;
	foggedPortals = nullptr;
	firstInteraction = nullptr;
	lastInteraction = nullptr;

	baseLightProject.Zero();
	inverseBaseLightProject.Zero();
}

void idRenderLightLocal::FreeRenderLight()
{
}
void idRenderLightLocal::UpdateRenderLight(const renderLight_t *re, bool forceUpdate)
{
}
void idRenderLightLocal::GetRenderLight(renderLight_t *re)
{
}
void idRenderLightLocal::ForceUpdate()
{
}
int idRenderLightLocal::GetIndex()
{
	return index;
}

void idRenderEntityLocal::ReadFromDemoFile(class idDemoFile *f)
{
	int i;
	renderEntity_t ent;
	/* Initialize Pointers */
	decals = nullptr;
	overlays = nullptr;
	dynamicModel = nullptr;
	ent.referenceShader = nullptr;
	ent.referenceSound = nullptr;
	ent.hModel = nullptr;
	ent.customSkin = nullptr;
	ent.customShader = nullptr;
	ent.joints = nullptr;
	ent.callback = nullptr;
	ent.callbackData = nullptr;
	ent.remoteRenderView = nullptr;

	f->ReadInt(index);
	f->ReadInt(dynamicModelFrameCount);
	f->ReadInt(ent.entityNum);
	f->ReadInt(ent.bodyId);
	f->ReadVec3(ent.bounds[0]);
	f->ReadVec3(ent.bounds[1]);
	f->ReadInt(ent.suppressSurfaceInViewID);
	f->ReadInt(ent.suppressShadowInViewID);
	f->ReadInt(ent.suppressShadowInLightID);
	f->ReadInt(ent.allowSurfaceInViewID);
	f->ReadVec3(ent.origin);
	f->ReadMat3(ent.axis);
	for(i = 0; i < MAX_ENTITY_SHADER_PARMS; i++)
	{
		f->ReadFloat(ent.shaderParms[i]);
	}
	for(i = 0; i < MAX_RENDERENTITY_GUI; i++)
	{
		f->ReadInt((int &)ent.gui[i]);
		ent.gui[i] = nullptr;
	}
	f->ReadInt(i); //( int& )ent.remoteRenderView
	f->ReadInt(ent.numJoints);
	f->ReadFloat(ent.modelDepthHack);
	f->ReadBool(ent.noSelfShadow);
	f->ReadBool(ent.noShadow);
	f->ReadBool(ent.noOverlays);
	f->ReadBool(ent.noDynamicInteractions);
	f->ReadBool(ent.weaponDepthHack);
	f->ReadInt(ent.forceUpdate);

	const char *declName = nullptr;

	declName = f->ReadHashString();
	ent.customShader = (declName[0] != 0) ? declManager->FindMaterial(declName) : nullptr;

	declName = f->ReadHashString();
	ent.referenceShader = (declName[0] != 0) ? declManager->FindMaterial(declName) : nullptr;

	declName = f->ReadHashString();
	ent.customSkin = (declName[0] != 0) ? declManager->FindSkin(declName) : nullptr;

	int soundIndex = -1;
	f->ReadInt(soundIndex);
	ent.referenceSound = soundIndex != -1 ? common->SW()->EmitterForIndex(soundIndex) : nullptr;

	const char *mdlName = f->ReadHashString();
	ent.hModel = (mdlName[0] != 0) ? renderModelManager->FindModel(mdlName) : nullptr;

	/*if( ent.hModel != nullptr )
	{
		bool dynamicModel = false;
		f->ReadBool( dynamicModel );
		if( dynamicModel )
		{
			ent.hModel->ReadFromDemoFile( f );
		}
	}*/

	if(ent.numJoints > 0)
	{
		ent.joints = (idJointMat *)Mem_Alloc16(SIMD_ROUND_JOINTS(ent.numJoints) * sizeof(ent.joints[0]), TAG_JOINTMAT);
		for(int i = 0; i < ent.numJoints; i++)
		{
			float *data = ent.joints[i].ToFloatPtr();
			for(int j = 0; j < 12; ++j)
			{
				f->ReadFloat(data[j]);
			}
		}
		SIMD_INIT_LAST_JOINT(ent.joints, ent.numJoints);
	}

	f->ReadInt(ent.timeGroup);
	f->ReadInt(ent.xrayIndex);

	/*
	f->ReadInt( i );
	if( i ) {
	ent.overlays = idRenderModelOverlay::Alloc();
	ent.overlays->ReadFromDemoFile( f->Read );
	}
	*/

	world->UpdateEntityDef(index, &ent);
	for(i = 0; i < MAX_RENDERENTITY_GUI; i++)
	{
		if(parms.gui[i])
		{
			parms.gui[i] = uiManager->Alloc();
#ifdef WRITE_GUIS
			parms.gui[i]->ReadFromDemoFile(f->Read);
#endif
		}
	}
	if(r_showDemo.GetBool())
	{
		mpSystem->Printf("DC_UPDATE_ENTITYDEF: %i = %s\n", index, parms.hModel ? parms.hModel->Name() : "nullptr"); // TODO: WHAT?
	}
}
void idRenderEntityLocal::WriteToDemoFile(class idDemoFile *f) const
{
	f->WriteInt(index);
	f->WriteInt(dynamicModelFrameCount);
	f->WriteInt(parms.entityNum);
	f->WriteInt(parms.bodyId);
	f->WriteVec3(parms.bounds[0]);
	f->WriteVec3(parms.bounds[1]);
	f->WriteInt(parms.suppressSurfaceInViewID);
	f->WriteInt(parms.suppressShadowInViewID);
	f->WriteInt(parms.suppressShadowInLightID);
	f->WriteInt(parms.allowSurfaceInViewID);
	f->WriteVec3(parms.origin);
	f->WriteMat3(parms.axis);
	for(int i = 0; i < MAX_ENTITY_SHADER_PARMS; i++)
		f->WriteFloat(parms.shaderParms[i]);
	for(int i = 0; i < MAX_RENDERENTITY_GUI; i++)
		f->WriteInt((int &)parms.gui[i]);
	f->WriteInt((int &)parms.remoteRenderView);
	f->WriteInt(parms.numJoints);
	f->WriteFloat(parms.modelDepthHack);
	f->WriteBool(parms.noSelfShadow);
	f->WriteBool(parms.noShadow);
	f->WriteBool(parms.noOverlays);
	f->WriteBool(parms.noDynamicInteractions);
	f->WriteBool(parms.weaponDepthHack);
	f->WriteInt(parms.forceUpdate);

	f->WriteHashString(parms.customShader ? parms.customShader->GetName() : "");
	f->WriteHashString(parms.referenceShader ? parms.referenceShader->GetName() : "");
	f->WriteHashString(parms.customSkin ? parms.customSkin->GetName() : "");
	f->WriteInt(parms.referenceSound ? parms.referenceSound->Index() : -1);

	f->WriteHashString(parms.hModel ? parms.hModel->Name() : "");
	/*if( parms.hModel )
	{
		f->WriteBool( parms.hModel->ModelDynamicallyGenerated() ? true : false );
		if( parms.hModel->ModelDynamicallyGenerated() )
		{
			parms.hModel->WriteToDemoFile( f );
		}
	}*/

	for(int i = 0; i < parms.numJoints; i++)
	{
		float *data = parms.joints[i].ToFloatPtr();
		for(int j = 0; j < 12; ++j)
			f->WriteFloat(data[j]);
	}

	// RENDERDEMO_VERSION >= 2 ( Doom3 1.2 )
	f->WriteInt(parms.timeGroup);
	f->WriteInt(parms.xrayIndex);

#ifdef WRITE_GUIS
	for(i = 0; i < MAX_RENDERENTITY_GUI; i++)
	{
		if(parms.gui[i])
		{
			f->WriteInt(1);
			parms.gui[i]->WriteToDemoFile(f);
		}
		else
		{
			f->WriteInt(0);
		}
	}
#endif

	if(r_showDemo.GetBool())
	{
		mpSystem->Printf("write DC_UPDATE_ENTITYDEF: %i = %s\n", index, parms.hModel ? parms.hModel->Name() : "nullptr"); // TODO: WHAT?!
	}
}
