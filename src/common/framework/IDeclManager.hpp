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

/// @file

#pragma once

/*
===============================================================================

	Declaration Manager

	All "small text" data types, like materials, sound shaders, fx files,
	entity defs, etc. are managed uniformly, allowing reloading, purging,
	listing, printing, etc. All "large text" data types that never have more
	than one declaration in a given file, like maps, models, AAS files, etc.
	are not handled here.

	A decl will never, ever go away once it is created. The manager is
	garranteed to always return the same decl pointer for a decl type/name
	combination. The index of a decl in the per type list also stays the
	same throughout the lifetime of the engine. Although the pointer to
	a decl always stays the same, one should never maintain pointers to
	data inside decls. The data stored in a decl is not garranteed to stay
	the same for more than one engine frame.

	The decl indexes of explicitely defined decls are garrenteed to be
	consistent based on the parsed decl files. However, the indexes of
	implicit decls may be different based on the order in which levels
	are loaded.

	The decl namespaces are separate for each type. Comments for decls go
	above the text definition to keep them associated with the proper decl.

	During decl parsing, errors should never be issued, only warnings
	followed by a call to MakeDefault().

===============================================================================
*/

class idMaterial;
class idDeclSkin;
class idSoundShader;

typedef enum
{
	DECL_TABLE				= 0,
	DECL_MATERIAL,
	DECL_SKIN,
	DECL_SOUND,
	DECL_ENTITYDEF,
	DECL_MODELDEF,
	DECL_FX,
	DECL_PARTICLE,
	DECL_AF,
	DECL_PDA,
	DECL_VIDEO,
	DECL_AUDIO,
	DECL_EMAIL,
	DECL_MODELEXPORT,
	DECL_MAPDEF,
	
	// new decl types can be added here
	
	DECL_MAX_TYPES			= 32
} declType_t;

class idDeclManager
{
public:
	virtual					~idDeclManager() {}
	
	virtual void			Init() = 0;
	virtual void			Init2() = 0;
	virtual void			Shutdown() = 0;
	virtual void			Reload( bool force ) = 0;
	
	virtual void			BeginLevelLoad() = 0;
	virtual void			EndLevelLoad() = 0;
	
	// Registers a new decl type.
	virtual void			RegisterDeclType( const char* typeName, declType_t type, idDecl * ( *allocator )() ) = 0;
	
	// Registers a new folder with decl files.
	virtual void			RegisterDeclFolder( const char* folder, const char* extension, declType_t defaultType ) = 0;
	
	// Returns a checksum for all loaded decl text.
	virtual int				GetChecksum() const = 0;
	
	// Returns the number of decl types.
	virtual int				GetNumDeclTypes() const = 0;
	
	// Returns the type name for a decl type.
	virtual const char* 	GetDeclNameFromType( declType_t type ) const = 0;
	
	// Returns the decl type for a type name.
	virtual declType_t		GetDeclTypeFromName( const char* typeName ) const = 0;
	
	// If makeDefault is true, a default decl of appropriate type will be created
	// if an explicit one isn't found. If makeDefault is false, NULL will be returned
	// if the decl wasn't explcitly defined.
	virtual const idDecl* 	FindType( declType_t type, const char* name, bool makeDefault = true ) = 0;
	
	virtual const idDecl*	FindDeclWithoutParsing( declType_t type, const char* name, bool makeDefault = true ) = 0;
	
	virtual void			ReloadFile( const char* filename, bool force ) = 0;
	
	// Returns the number of decls of the given type.
	virtual int				GetNumDecls( declType_t type ) = 0;
	
	// The complete lists of decls can be walked to populate editor browsers.
	// If forceParse is set false, you can get the decl to check name / filename / etc.
	// without causing it to parse the source and load media.
	virtual const idDecl* 	DeclByIndex( declType_t type, int index, bool forceParse = true ) = 0;
	
	// List and print decls.
	virtual void			ListType( const idCmdArgs& args, declType_t type ) = 0;
	virtual void			PrintType( const idCmdArgs& args, declType_t type ) = 0;
	
	// Creates a new default decl of the given type with the given name in
	// the given file used by editors to create a new decls.
	virtual idDecl* 		CreateNewDecl( declType_t type, const char* name, const char* fileName ) = 0;
	
	// BSM - Added for the material editors rename capabilities
	virtual bool			RenameDecl( declType_t type, const char* oldName, const char* newName ) = 0;
	
	// When media files are loaded, a reference line can be printed at a
	// proper indentation if decl_show is set
	virtual void			MediaPrint( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	virtual void			WritePrecacheCommands( idFile* f ) = 0;
	
	// Convenience functions for specific types.
	virtual	const idMaterial* 		FindMaterial( const char* name, bool makeDefault = true ) = 0;
	virtual const idDeclSkin* 		FindSkin( const char* name, bool makeDefault = true ) = 0;
	virtual const idSoundShader* 	FindSound( const char* name, bool makeDefault = true ) = 0;
	
	virtual const idMaterial* 		MaterialByIndex( int index, bool forceParse = true ) = 0;
	virtual const idDeclSkin* 		SkinByIndex( int index, bool forceParse = true ) = 0;
	virtual const idSoundShader* 	SoundByIndex( int index, bool forceParse = true ) = 0;
	
	virtual void					Touch( const idDecl* decl ) = 0;
};