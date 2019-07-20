/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 BlackPhrase

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

#ifndef __DECLMANAGER_H__
#define __DECLMANAGER_H__

typedef enum
{
	DS_UNPARSED,
	DS_DEFAULTED,			// set if a parse failed due to an error, or the lack of any source
	DS_PARSED
} declState_t;

const int DECL_LEXER_FLAGS	=	LEXFL_NOSTRINGCONCAT |				// multiple strings separated by whitespaces are not concatenated
								LEXFL_NOSTRINGESCAPECHARS |			// no escape characters inside strings
								LEXFL_ALLOWPATHNAMES |				// allow path seperators in names
								LEXFL_ALLOWMULTICHARLITERALS |		// allow multi character literals
								LEXFL_ALLOWBACKSLASHSTRINGCONCAT |	// allow multiple strings separated by '\' to be concatenated
								LEXFL_NOFATALERRORS;				// just set a flag instead of fatal erroring




#include "framework/Decl.hpp"

template< class type >
ID_INLINE idDecl* idDeclAllocator()
{
	return new( TAG_DECL ) type;
}

#include "framework/IDeclManager.hpp"

extern idDeclManager* 		declManager;


template< declType_t type >
ID_INLINE void idListDecls_f( const idCmdArgs& args )
{
	declManager->ListType( args, type );
}

template< declType_t type >
ID_INLINE void idPrintDecls_f( const idCmdArgs& args )
{
	declManager->PrintType( args, type );
}

#endif /* !__DECLMANAGER_H__ */
