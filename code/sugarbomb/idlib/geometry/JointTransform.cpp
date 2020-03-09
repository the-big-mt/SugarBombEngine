/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

//*****************************************************************************

#pragma hdrstop

#include "precompiled.h"

#include "idlib/geometry/JointTransform.h"
//#include "idlib/math/Math.h"
//#include "idlib/math/Quat.h"
//#include "idlib/math/Vector.h"

//*****************************************************************************

//namespace BFG
//{

/*
=============
idJointMat::ToJointQuat
=============
*/
idJointQuat idJointMat::ToJointQuat() const
{
	idJointQuat	jq;
	float		trace;
	float		s;
	float		t;
	int     	i;
	int			j;
	int			k;
	
	static int 	next[3] = { 1, 2, 0 };
	
	trace = mat[0 * 4 + 0] + mat[1 * 4 + 1] + mat[2 * 4 + 2];
	
	if( trace > 0.0f )
	{
	
		t = trace + 1.0f;
		s = idMath::InvSqrt( t ) * 0.5f;
		
		jq.q[3] = s * t;
		jq.q[0] = ( mat[1 * 4 + 2] - mat[2 * 4 + 1] ) * s;
		jq.q[1] = ( mat[2 * 4 + 0] - mat[0 * 4 + 2] ) * s;
		jq.q[2] = ( mat[0 * 4 + 1] - mat[1 * 4 + 0] ) * s;
		
	}
	else
	{
	
		i = 0;
		if( mat[1 * 4 + 1] > mat[0 * 4 + 0] )
		{
			i = 1;
		}
		if( mat[2 * 4 + 2] > mat[i * 4 + i] )
		{
			i = 2;
		}
		j = next[i];
		k = next[j];
		
		t = ( mat[i * 4 + i] - ( mat[j * 4 + j] + mat[k * 4 + k] ) ) + 1.0f;
		s = idMath::InvSqrt( t ) * 0.5f;
		
		jq.q[i] = s * t;
		jq.q[3] = ( mat[j * 4 + k] - mat[k * 4 + j] ) * s;
		jq.q[j] = ( mat[i * 4 + j] + mat[j * 4 + i] ) * s;
		jq.q[k] = ( mat[i * 4 + k] + mat[k * 4 + i] ) * s;
	}
	
	jq.t[0] = mat[0 * 4 + 3];
	jq.t[1] = mat[1 * 4 + 3];
	jq.t[2] = mat[2 * 4 + 3];
	jq.w = 0.0f;
	
	return jq;
}

//} // namespace BFG
