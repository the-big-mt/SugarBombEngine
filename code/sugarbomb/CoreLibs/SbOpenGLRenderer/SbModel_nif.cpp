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

#include "precompiled.h"

#include "SbModel_nif.hpp"

SbRenderModelNIF::SbRenderModelNIF() = default;
SbRenderModelNIF::~SbRenderModelNIF() = default;

void SbRenderModelNIF::InitFromFile(const char *fileName){};

bool SbRenderModelNIF::LoadBinaryModel(idFile *file, const ID_TIME_T sourceTimeStamp)
{
	return false;
};

void SbRenderModelNIF::WriteBinaryModel(idFile *file, ID_TIME_T *_timeStamp) const {};

bool SbRenderModelNIF::SupportsBinaryModel()
{
	return false;
};

void SbRenderModelNIF::ExportOBJ(idFile *objFile, idFile *mtlFile, ID_TIME_T *_timeStamp) const {};

void SbRenderModelNIF::PartialInitFromFile(const char *fileName){};

void SbRenderModelNIF::InitEmpty(const char *name){};

void SbRenderModelNIF::AddSurface(modelSurface_t surface){};

void SbRenderModelNIF::FinishSurfaces(){};

void SbRenderModelNIF::PurgeModel(){};

void SbRenderModelNIF::Reset(){};

void SbRenderModelNIF::LoadModel(){};

bool SbRenderModelNIF::IsLoaded()
{
	return false;
};

void SbRenderModelNIF::SetLevelLoadReferenced(bool referenced){};

bool SbRenderModelNIF::IsLevelLoadReferenced()
{
	return false;
};

void SbRenderModelNIF::TouchData(){};

void SbRenderModelNIF::FreeVertexCache(){};

const char *SbRenderModelNIF::Name() const
{
	return "";
};

void SbRenderModelNIF::Print() const {};

void SbRenderModelNIF::List() const {};

int SbRenderModelNIF::Memory() const
{
	return 0;
};

ID_TIME_T SbRenderModelNIF::Timestamp() const
{
	return {};
};

int SbRenderModelNIF::NumSurfaces() const
{
	return 0;
};

int SbRenderModelNIF::NumBaseSurfaces() const
{
	return 0;
};

const modelSurface_t *SbRenderModelNIF::Surface(int surfaceNum) const
{
	return nullptr;
};

srfTriangles_t *SbRenderModelNIF::AllocSurfaceTriangles(int numVerts, int numIndexes) const
{
	return nullptr;
};

void SbRenderModelNIF::FreeSurfaceTriangles(srfTriangles_t *tris) const {};

bool SbRenderModelNIF::IsStaticWorldModel() const
{
	return false;
};

bool SbRenderModelNIF::IsReloadable() const
{
	return false;
};

dynamicModel_t SbRenderModelNIF::IsDynamicModel() const
{
	return 0;
};

bool SbRenderModelNIF::IsDefaultModel() const
{
	return false;
};

idBounds SbRenderModelNIF::Bounds(const struct renderEntity_s *ent) const
{
	return {};
};

float SbRenderModelNIF::DepthHack() const
{
	return 0.0f;
};

idRenderModel *SbRenderModelNIF::InstantiateDynamicModel(const struct renderEntity_s *ent, const viewDef_t *view, idRenderModel *cachedModel)
{
	return nullptr;
};

int SbRenderModelNIF::NumJoints() const
{
	return 0;
};

const idMD5Joint *SbRenderModelNIF::GetJoints() const
{
	return nullptr;
};

jointHandle_t SbRenderModelNIF::GetJointHandle(const char *name) const
{
	return 0;
};

const char *SbRenderModelNIF::GetJointName(jointHandle_t handle) const
{
	return "";
};

const idJointQuat *SbRenderModelNIF::GetDefaultPose() const
{
	return nullptr;
};

int SbRenderModelNIF::NearestJoint(int surfaceNum, int a, int c, int b) const
{
	return 0;
};

void SbRenderModelNIF::ReadFromDemoFile(class idDemoFile *f){};

void SbRenderModelNIF::WriteToDemoFile(class idDemoFile *f){};