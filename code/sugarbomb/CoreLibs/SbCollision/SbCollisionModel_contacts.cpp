/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

/*
===============================================================================

	Trace model vs. polygonal model collision detection.

===============================================================================
*/

#pragma hdrstop
#include "precompiled.h"

//#include "../cm/CollisionModel.h"
//#include "../idlib/math/Vector.h"
#include "CollisionModel_local.h"

//namespace BFG
//{

//class idMat3;
//class idTraceModel;

/*
===============================================================================

Retrieving contacts

===============================================================================
*/

/*
==================
idCollisionModelManagerLocal::Contacts
==================
*/
int idCollisionModelManagerLocal::Contacts(contactInfo_t *contacts, const int maxContacts, const idVec3 &start, const idVec6 &dir, const float depth,
                                           const idTraceModel *trm, const idMat3 &trmAxis, int contentMask,
                                           cmHandle_t model, const idVec3 &origin, const idMat3 &modelAxis)
{
	trace_t results;
	idVec3 end;

	// same as Translation but instead of storing the first collision we store all collisions as contacts
	idCollisionModelManagerLocal::getContacts = true;
	idCollisionModelManagerLocal::contacts = contacts;
	idCollisionModelManagerLocal::maxContacts = maxContacts;
	idCollisionModelManagerLocal::numContacts = 0;
	end = start + dir.SubVec3(0) * depth;
	idCollisionModelManagerLocal::Translation(&results, start, end, trm, trmAxis, contentMask, model, origin, modelAxis);
	if(dir.SubVec3(1).LengthSqr() != 0.0f)
	{
		// FIXME: rotational contacts
	}
	idCollisionModelManagerLocal::getContacts = false;
	idCollisionModelManagerLocal::maxContacts = 0;

	return idCollisionModelManagerLocal::numContacts;
}

//} // namespace BFG
