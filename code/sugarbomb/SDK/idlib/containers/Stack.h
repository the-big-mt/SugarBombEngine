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

#ifndef __STACK_H__
#define __STACK_H__
//*****************************************************************************


//*****************************************************************************

//namespace BFG
//{

/*
===============================================================================

	Stack template

===============================================================================
*/

#define idStack( type, next )		idStackTemplate<type, (int)&(((type*)NULL)->next)>

template< class type, int nextOffset >
class idStackTemplate
{
public:
	idStackTemplate();
	
	void					Add( type* element );
	type* 					Get();
	
private:
	type* 					top;
	type* 					bottom;
};

#define STACK_NEXT_PTR( element )		(*(type**)(((byte*)element)+nextOffset))

template< class type, int nextOffset >
idStackTemplate<type, nextOffset>::idStackTemplate()
{
	top = bottom = NULL;
}

template< class type, int nextOffset >
void idStackTemplate<type, nextOffset>::Add( type* element )
{
	STACK_NEXT_PTR( element ) = top;
	top = element;
	if( !bottom )
	{
		bottom = element;
	}
}

template< class type, int nextOffset >
type* idStackTemplate<type, nextOffset>::Get()
{
	type* element;
	
	element = top;
	if( element )
	{
		top = STACK_NEXT_PTR( top );
		if( bottom == element )
		{
			bottom = NULL;
		}
		STACK_NEXT_PTR( element ) = NULL;
	}
	return element;
}

//} // namespace BFG

#endif /* !__STACK_H__ */
