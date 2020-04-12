/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

//*****************************************************************************

#pragma hdrstop

#include "precompiled.h"

#include "idlib/geometry/DrawVert.h"
//#include "idlib/math/Vector.h"

//*****************************************************************************

//namespace sbe
//{

/*
============
idShadowVert::CreateShadowCache
============
*/
int idShadowVert::CreateShadowCache( idShadowVert* vertexCache, const idDrawVert* verts, const int numVerts )
{
	for( int i = 0; i < numVerts; i++ )
	{
		vertexCache[i * 2 + 0].xyzw[0] = verts[i].xyz[0];
		vertexCache[i * 2 + 0].xyzw[1] = verts[i].xyz[1];
		vertexCache[i * 2 + 0].xyzw[2] = verts[i].xyz[2];
		vertexCache[i * 2 + 0].xyzw[3] = 1.0f;
		
		vertexCache[i * 2 + 1].xyzw[0] = verts[i].xyz[0];
		vertexCache[i * 2 + 1].xyzw[1] = verts[i].xyz[1];
		vertexCache[i * 2 + 1].xyzw[2] = verts[i].xyz[2];
		vertexCache[i * 2 + 1].xyzw[3] = 0.0f;
	}
	return numVerts * 2;
}

/*
===================
idShadowVertSkinned::CreateShadowCache
===================
*/
int idShadowVertSkinned::CreateShadowCache( idShadowVertSkinned* vertexCache, const idDrawVert* verts, const int numVerts )
{
	for( int i = 0; i < numVerts; i++ )
	{
		vertexCache[0].xyzw[0] = verts[i].xyz[0];
		vertexCache[0].xyzw[1] = verts[i].xyz[1];
		vertexCache[0].xyzw[2] = verts[i].xyz[2];
		vertexCache[0].xyzw[3] = 1.0f;
		*( unsigned int* )vertexCache[0].color = *( unsigned int* )verts[i].color;
		*( unsigned int* )vertexCache[0].color2 = *( unsigned int* )verts[i].color2;
		
		vertexCache[1].xyzw[0] = verts[i].xyz[0];
		vertexCache[1].xyzw[1] = verts[i].xyz[1];
		vertexCache[1].xyzw[2] = verts[i].xyz[2];
		vertexCache[1].xyzw[3] = 0.0f;
		*( unsigned int* )vertexCache[1].color = *( unsigned int* )verts[i].color;
		*( unsigned int* )vertexCache[1].color2 = *( unsigned int* )verts[i].color2;
		
		vertexCache += 2;
	}
	return numVerts * 2;
}

//} // namespace sbe