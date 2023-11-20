/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans
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

//#include "idlib/geometry/Surface_Patch.h"
//#include "idlib/math/Plane.h"
//#include "idlib/Dict.h"

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Reads or writes the contents of .map files into a standard internal
	format, which can then be moved into private formats for collision
	detection, map processing, or editor use.

	No validation (duplicate planes, null area brushes, etc) is performed.
	There are no limits to the number of any of the elements in maps.
	The order of entities, brushes, and sides is maintained.

===============================================================================
*/

const int OLD_MAP_VERSION					= 1;
// RB: added new map format
const int DOOM3_MAP_VERSION					= 2;
const int CURRENT_MAP_VERSION				= 3;
// RB end
const int DEFAULT_CURVE_SUBDIVISION			= 4;
const float DEFAULT_CURVE_MAX_ERROR			= 4.0f;
const float DEFAULT_CURVE_MAX_ERROR_CD		= 24.0f;
const float DEFAULT_CURVE_MAX_LENGTH		= -1.0f;
const float DEFAULT_CURVE_MAX_LENGTH_CD		= -1.0f;

struct idDeclManager;

class idMapPrimitive
{
public:
	// RB: new mesh primitive to work with Blender Ngons
	enum { TYPE_INVALID = -1, TYPE_BRUSH, TYPE_PATCH, TYPE_MESH };
	// RB end
	
	idDict					epairs;
	
	idMapPrimitive()
	{
		type = TYPE_INVALID;
	}
	virtual					~idMapPrimitive() { }
	int						GetType() const
	{
		return type;
	}
	
protected:
	int						type;
};


class idMapBrushSide
{
	friend class idMapBrush;
	
public:
	idMapBrushSide();
	~idMapBrushSide() { }
	const char* 			GetMaterial() const
	{
		return material;
	}
	void					SetMaterial( const char* p )
	{
		material = p;
	}
	const idPlane& 			GetPlane() const
	{
		return plane;
	}
	void					SetPlane( const idPlane& p )
	{
		plane = p;
	}
	void					SetTextureMatrix( const idVec3 mat[2] )
	{
		texMat[0] = mat[0];
		texMat[1] = mat[1];
	}
	void					GetTextureMatrix( idVec3& mat1, idVec3& mat2 )
	{
		mat1 = texMat[0];
		mat2 = texMat[1];
	}
	void					GetTextureVectors( idVec4 v[2] ) const;
	
protected:
	idStr					material;
	idPlane					plane;
	idVec3					texMat[2];
	idVec3					origin;
};

ID_INLINE idMapBrushSide::idMapBrushSide()
{
	plane.Zero();
	texMat[0].Zero();
	texMat[1].Zero();
	origin.Zero();
}


class idMapBrush : public idMapPrimitive
{
public:
	idMapBrush()
	{
		type = TYPE_BRUSH;
		sides.Resize( 8, 4 );
	}
	~idMapBrush()
	{
		sides.DeleteContents( true );
	}
	static idMapBrush* 		Parse( idLexer& src, const idVec3& origin, bool newFormat = true, float version = CURRENT_MAP_VERSION );
	static idMapBrush* 		ParseQ3( idLexer& src, const idVec3& origin );
	bool					Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;
	int						GetNumSides() const
	{
		return sides.Num();
	}
	int						AddSide( idMapBrushSide* side )
	{
		return sides.Append( side );
	}
	idMapBrushSide* 		GetSide( int i ) const
	{
		return sides[i];
	}
	unsigned int			GetGeometryCRC() const;
	
protected:
	int						numSides;
	idList<idMapBrushSide*, TAG_IDLIB_LIST_MAP> sides;
};


class idMapPatch : public idMapPrimitive, public idSurface_Patch
{
public:
	idMapPatch();
	idMapPatch( int maxPatchWidth, int maxPatchHeight );
	~idMapPatch() { }
	static idMapPatch* 		Parse( idLexer& src, const idVec3& origin, bool patchDef3 = true, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;
	const char* 			GetMaterial() const
	{
		return material;
	}
	void					SetMaterial( const char* p )
	{
		material = p;
	}
	int						GetHorzSubdivisions() const
	{
		return horzSubdivisions;
	}
	int						GetVertSubdivisions() const
	{
		return vertSubdivisions;
	}
	bool					GetExplicitlySubdivided() const
	{
		return explicitSubdivisions;
	}
	void					SetHorzSubdivisions( int n )
	{
		horzSubdivisions = n;
	}
	void					SetVertSubdivisions( int n )
	{
		vertSubdivisions = n;
	}
	void					SetExplicitlySubdivided( bool b )
	{
		explicitSubdivisions = b;
	}
	unsigned int			GetGeometryCRC() const;
	
protected:
	idStr					material;
	int						horzSubdivisions;
	int						vertSubdivisions;
	bool					explicitSubdivisions;
};

ID_INLINE idMapPatch::idMapPatch()
{
	type = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions = false;
	width = height = 0;
	maxWidth = maxHeight = 0;
	expanded = false;
}

ID_INLINE idMapPatch::idMapPatch( int maxPatchWidth, int maxPatchHeight )
{
	type = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions = false;
	width = height = 0;
	maxWidth = maxPatchWidth;
	maxHeight = maxPatchHeight;
	verts.SetNum( maxWidth * maxHeight );
	expanded = false;
}


// RB begin
class MapPolygon
{
	friend class MapPolygonMesh;
	
public:
	MapPolygon();
	MapPolygon( int numIndexes );
	~MapPolygon() { }
	
	const char* 			GetMaterial() const
	{
		return material;
	}
	
	void					SetMaterial( const char* p )
	{
		material = p;
	}
	
	void					AddIndex( int index )
	{
		indexes.Append( index );
	}
	
	void					SetIndexes( const idTempArray<int>& _indexes )
	{
		indexes.Resize( _indexes.Num() );
		
		for( unsigned int i = 0; i < _indexes.Num(); i++ )
		{
			indexes[i] = _indexes[i];
		}
	}
	
	const idList<int>&		GetIndexes() const
	{
		return indexes;
	}
	
	
protected:
	idStr					material;
	idList<int>				indexes;		// [3..n] references to vertices for each face
};

ID_INLINE MapPolygon::MapPolygon()
{
}

ID_INLINE MapPolygon::MapPolygon( int numIndexes )
{
	//indexes.AssureSize( 3 );
}


class MapPolygonMesh : public idMapPrimitive
{
public:
	MapPolygonMesh(idDeclManager *apDeclManager);
	~MapPolygonMesh()
	{
		//verts.DeleteContents();
		//polygons.DeleteContents( true );
	}
	
	void					ConvertFromBrush( const idMapBrush* brush, int entityNum, int primitiveNum );
	void					ConvertFromPatch( const idMapPatch* patch, int entityNum, int primitiveNum );
	
	static MapPolygonMesh*	Parse( idDeclManager *apDeclManager, idLexer& src, const idVec3& origin, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;
	
	static MapPolygonMesh*	ParseJSON( idDeclManager *apDeclManager, idLexer& src );
	bool					WriteJSON( idFile* fp, int primitiveNum, const idVec3& origin ) const;
	
	
	
	int						GetNumVertices() const
	{
		return verts.Num();
	}
	
	int						AddVertex( const idDrawVert& v )
	{
		return verts.Append( v );
	}
	
	
	int						GetNumPolygons() const
	{
		return polygons.Num();
	}
	
	//int						AddPolygon( MapPolygon* face )
	//{
	//	return polygons.Append( face );
	//}
	
	const MapPolygon& 			GetFace( int i ) const
	{
		return polygons[i];
	}
	
	unsigned int			GetGeometryCRC() const;
	
	const idList<idDrawVert>&	GetDrawVerts() const
	{
		return verts;
	}
	
	bool					IsOpaque() const
	{
		return opaque;
	}
	
	bool					IsAreaportal() const;
	
	void					GetBounds( idBounds& bounds ) const;
	
private:
	void					SetContents();
	
	int						originalType;
	
protected:

	idList<idDrawVert>		verts;			// vertices can be shared between polygons
	idList<MapPolygon>		polygons;
	
	// derived data after parsing
	
	// material surface flags
	int						contents;
	bool					opaque;
private:
	idDeclManager *declManager{nullptr};
};
// RB end




class idMapEntity
{
	friend class			idMapFile;
	
public:
	idDict					epairs;
	
public:
	idMapEntity()
	{
		epairs.SetHashSize( 64 );
	}
	~idMapEntity()
	{
		primitives.DeleteContents( true );
	}
	static idMapEntity* 	Parse( idDeclManager *apDeclManager, idLexer& src, bool worldSpawn = false, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile* fp, int entityNum ) const;
	// RB begin
	static idMapEntity* 	ParseJSON( idDeclManager *apDeclManager, idLexer& src );
	bool					WriteJSON( idFile* fp, int entityNum, int numEntities ) const;
	// RB end
	int						GetNumPrimitives() const
	{
		return primitives.Num();
	}
	idMapPrimitive* 		GetPrimitive( int i ) const
	{
		return primitives[i];
	}
	void					AddPrimitive( idMapPrimitive* p )
	{
		primitives.Append( p );
	}
	unsigned int			GetGeometryCRC() const;
	void					RemovePrimitiveData();
	
protected:
	idList<idMapPrimitive*, TAG_IDLIB_LIST_MAP>	primitives;
};


class idMapFile
{
public:
	idMapFile();
	~idMapFile()
	{
		entities.DeleteContents( true );
	}
	
	// filename does not require an extension
	// normally this will use a .reg file instead of a .map file if it exists,
	// which is what the game and dmap want, but the editor will want to always
	// load a .map file
	bool					Parse( idDeclManager *apDeclManager, const char* filename, bool ignoreRegion = false, bool osPath = false );
	bool					Write( const char* fileName, const char* ext, bool fromBasePath = true );
	
	// RB begin
	bool					WriteJSON( const char* fileName, const char* ext, bool fromBasePath = true );
	bool					ConvertToPolygonMeshFormat(idDeclManager *apDeclManager);
	// RB end
	
	// get the number of entities in the map
	int						GetNumEntities() const
	{
		return entities.Num();
	}
	// get the specified entity
	idMapEntity* 			GetEntity( int i ) const
	{
		return entities[i];
	}
	// get the name without file extension
	const char* 			GetName() const
	{
		return name;
	}
	// get the file time
	ID_TIME_T					GetFileTime() const
	{
		return fileTime;
	}
	// get CRC for the map geometry
	// texture coordinates and entity key/value pairs are not taken into account
	unsigned int			GetGeometryCRC() const
	{
		return geometryCRC;
	}
	// returns true if the file on disk changed
	bool					NeedsReload();
	
	int						AddEntity( idMapEntity* mapentity );
	idMapEntity* 			FindEntity( const char* name );
	void					RemoveEntity( idMapEntity* mapEnt );
	void					RemoveEntities( const char* classname );
	void					RemoveAllEntities();
	void					RemovePrimitiveData();
	bool					HasPrimitiveData()
	{
		return hasPrimitiveData;
	}
	
protected:
	float					version;
	ID_TIME_T					fileTime;
	unsigned int			geometryCRC;
	idList<idMapEntity*, TAG_IDLIB_LIST_MAP>	entities;
	idStr					name;
	bool					hasPrimitiveData;
	
private:
	void					SetGeometryCRC();
};

ID_INLINE idMapFile::idMapFile()
{
	version = CURRENT_MAP_VERSION;
	fileTime = 0;
	geometryCRC = 0;
	entities.Resize( 1024, 256 );
	hasPrimitiveData = false;
}

//} // namespace BFG