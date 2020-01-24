/*
*******************************************************************************

Copyright (C) 2018-2019 SugarBombEngine Developers

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

*******************************************************************************
*/

/// @file

#pragma once

#include "Model.h"

class SbRenderModelNIF : public idRenderModel
{
public:
	SbRenderModelNIF();
	~SbRenderModelNIF();

	virtual void InitFromFile(const char *asFileName) override;

	virtual bool LoadBinaryModel(sbe::IFile *apFile, const ID_TIME_T sourceTimeStamp) override;
	virtual void WriteBinaryModel(sbe::IFile *apFile, ID_TIME_T *_timeStamp = nullptr) const override;
	virtual bool SupportsBinaryModel() override;

	virtual void ExportOBJ(sbe::IFile *apObjFile, sbe::IFile *apMtlFile, ID_TIME_T *_timeStamp = nullptr) const override;

	virtual void PartialInitFromFile(const char *asFileName) override;

	virtual void InitEmpty(const char *asName) override;

	virtual void AddSurface(modelSurface_t surface) override;

	virtual void FinishSurfaces() override;

	virtual void PurgeModel() override;

	virtual void Reset() override;

	virtual void LoadModel() override;
	virtual bool IsLoaded() override;

	virtual void SetLevelLoadReferenced(bool abReferenced) override;
	virtual bool IsLevelLoadReferenced() override;

	virtual void TouchData() override;

	virtual void FreeVertexCache() override;

	virtual const char *GetName() const override;

	virtual void Print() const override;

	virtual void List() const override;

	virtual int GetMemory() const override;

	virtual ID_TIME_T GetTimestamp() const override;

	virtual int GetNumSurfaces() const override;
	virtual int GetNumBaseSurfaces() const override;

	virtual const modelSurface_t *GetSurface(int anSurface) const override;

	virtual srfTriangles_t *AllocSurfaceTriangles(int anVerts, int anIndexes) const override;
	virtual void FreeSurfaceTriangles(srfTriangles_t *apTris) const override;

	virtual bool IsStaticWorldModel() const override;
	virtual bool IsReloadable() const override;
	virtual dynamicModel_t IsDynamicModel() const override;
	virtual bool IsDefaultModel() const override;

	virtual idBounds GetBounds(const struct renderEntity_s *apEnt = nullptr) const override;

	virtual float GetDepthHack() const override;

	virtual IRenderModel *InstantiateDynamicModel(const struct renderEntity_s *apEnt, const viewDef_t *view, IRenderModel *apCachedModel) override;

	virtual int GetNumJoints() const override;

	virtual const idMD5Joint *GetJoints() const override;

	virtual jointHandle_t GetJointHandle(const char *asName) const override;

	virtual const char *GetJointName(jointHandle_t handle) const override;

	virtual const idJointQuat *GetDefaultPose() const override;

	virtual int GetNearestJoint(int anSurface, int a, int c, int b) const override;

	virtual void ReadFromDemoFile(idDemoFile *apFile) override;
	virtual void WriteToDemoFile(idDemoFile *apFile) override;

	virtual bool ModelHasDrawingSurfaces() const
	{
		return true;
	};

	virtual bool ModelHasInteractingSurfaces() const
	{
		return true;
	};

	virtual bool ModelHasShadowCastingSurfaces() const
	{
		return true;
	};
};