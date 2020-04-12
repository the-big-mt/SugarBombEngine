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

#include "precompiled.h"

#pragma hdrstop

//*****************************************************************************

/*
============
idCmdArgs::operator=
============
*/
void idCmdArgs::operator=( const idCmdArgs& args )
{
	int i;
	
	argc = args.argc;
	memcpy( tokenized, args.tokenized, MAX_COMMAND_STRING );
	for( i = 0; i < argc; i++ )
	{
		argv[ i ] = tokenized + ( args.argv[ i ] - args.tokenized );
	}
}

/*
============
idCmdArgs::Args
============
*/
const char* idCmdArgs::Args( int start, int end, bool escapeArgs ) const
{
	static char cmd_args[MAX_COMMAND_STRING];
	int		i;
	
	assert( argc < MAX_COMMAND_ARGS );
	if( end < 0 )
	{
		end = argc - 1;
	}
	else if( end >= argc )
	{
		end = argc - 1;
	}
	cmd_args[0] = '\0';
	if( escapeArgs )
	{
		strcat( cmd_args, "\"" );
	}
	for( i = start; i <= end; i++ )
	{
		if( i > start )
		{
			if( escapeArgs )
			{
				strcat( cmd_args, "\" \"" );
			}
			else
			{
				strcat( cmd_args, " " );
			}
		}
		if( escapeArgs && strchr( argv[i], '\\' ) )
		{
			char* p = argv[i];
			while( *p != '\0' )
			{
				if( *p == '\\' )
				{
					strcat( cmd_args, "\\\\" );
				}
				else
				{
					int l = strlen( cmd_args );
					cmd_args[ l ] = *p;
					cmd_args[ l + 1 ] = '\0';
				}
				p++;
			}
		}
		else
		{
			strcat( cmd_args, argv[i] );
		}
	}
	if( escapeArgs )
	{
		strcat( cmd_args, "\"" );
	}
	
	return cmd_args;
}

/*
============
idCmdArgs::TokenizeString

Parses the given string into command line tokens.
The text is copied to a separate buffer and 0 characters
are inserted in the appropriate place. The argv array
will point into this temporary buffer.
============
*/
void idCmdArgs::TokenizeString( const char* text, bool keepAsStrings )
{
	idLexer		lex;
	idToken		token, number;
	int			len, totalLen;
	
	// clear previous args
	argc = 0;
	
	if( !text )
	{
		return;
	}
	
	lex.LoadMemory( text, strlen( text ), "idCmdSystemLocal::TokenizeString" );
	lex.SetFlags( LEXFL_NOERRORS
				  | LEXFL_NOWARNINGS
				  | LEXFL_NOSTRINGCONCAT
				  | LEXFL_ALLOWPATHNAMES
				  | LEXFL_NOSTRINGESCAPECHARS
				  | LEXFL_ALLOWIPADDRESSES | ( keepAsStrings ? LEXFL_ONLYSTRINGS : 0 ) );
				  
	totalLen = 0;
	
	while( 1 )
	{
		if( argc == MAX_COMMAND_ARGS )
		{
			return;			// this is usually something malicious
		}
		
		if( !lex.ReadToken( &token ) )
		{
			return;
		}
		
		// check for negative numbers
		if( !keepAsStrings && ( token == "-" ) )
		{
			if( lex.CheckTokenType( TT_NUMBER, 0, &number ) )
			{
				token = "-" + number;
			}
		}
		
		// check for cvar expansion
		if( token == "$" )
		{
			if( !lex.ReadToken( &token ) )
			{
				return;
			}
			if( idLib::cvarSystem )
			{
				token = idLib::cvarSystem->GetCVarString( token.c_str() );
			}
			else
			{
				token = "<unknown>";
			}
		}
		
		len = token.Length();
		
		if( totalLen + len + 1 > sizeof( tokenized ) )
		{
			return;			// this is usually something malicious
		}
		
		// regular token
		argv[argc] = tokenized + totalLen;
		argc++;
		
		idStr::Copynz( tokenized + totalLen, token.c_str(), sizeof( tokenized ) - totalLen );
		
		totalLen += len + 1;
	}
}

/*
============
idCmdArgs::AppendArg
============
*/
void idCmdArgs::AppendArg( const char* text )
{
	if( argc >= MAX_COMMAND_ARGS )
	{
		return;
	}
	if( !argc )
	{
		argc = 1;
		argv[ 0 ] = tokenized;
		idStr::Copynz( tokenized, text, sizeof( tokenized ) );
	}
	else
	{
		argv[ argc ] = argv[ argc - 1 ] + strlen( argv[ argc - 1 ] ) + 1;
		idStr::Copynz( argv[ argc ], text, sizeof( tokenized ) - ( argv[ argc ] - tokenized ) );
		argc++;
	}
}

/*
============
idCmdArgs::GetArgs
============
*/
const char* const* idCmdArgs::GetArgs( int* _argc )
{
	*_argc = argc;
	return ( const char** )&argv[0];
}

