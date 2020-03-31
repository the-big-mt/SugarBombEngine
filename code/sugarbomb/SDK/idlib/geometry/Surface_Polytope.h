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

#pragma once

//#include "../idlib/bv/Bounds.h"         // for idBounds
//#include "../idlib/geometry/Surface.h"

//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Polytope surface.

	NOTE: vertexes are not duplicated for texture coordinates.

===============================================================================
*/

class idSurface_Polytope : public idSurface
{
public:
	idSurface_Polytope();
	explicit idSurface_Polytope( const idSurface& surface ) : idSurface( surface ) {}
	
	void				FromPlanes( const idPlane* planes, const int numPlanes );
	
	void				SetupTetrahedron( const idBounds& bounds );
	void				SetupHexahedron( const idBounds& bounds );
	void				SetupOctahedron( const idBounds& bounds );
	void				SetupDodecahedron( const idBounds& bounds );
	void				SetupIcosahedron( const idBounds& bounds );
	void				SetupCylinder( const idBounds& bounds, const int numSides );
	void				SetupCone( const idBounds& bounds, const int numSides );
	
	int					SplitPolytope( const idPlane& plane, const float epsilon, idSurface_Polytope** front, idSurface_Polytope** back ) const;
	
protected:

};

/*
====================
idSurface_Polytope::idSurface_Polytope
====================
*/
ID_INLINE idSurface_Polytope::idSurface_Polytope()
{
}

//} // namespace BFG