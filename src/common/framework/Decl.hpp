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

#pragma once

#include <cstddef>

typedef enum
{
	DS_UNPARSED,
	DS_DEFAULTED,			// set if a parse failed due to an error, or the lack of any source
	DS_PARSED
} declState_t;

class idDeclBase
{
public:
	virtual 				~idDeclBase() {};
	virtual const char* 	GetName() const = 0;
	virtual declType_t		GetType() const = 0;
	virtual declState_t		GetState() const = 0;
	virtual bool			IsImplicit() const = 0;
	virtual bool			IsValid() const = 0;
	virtual void			Invalidate() = 0;
	virtual void			Reload() = 0;
	virtual void			EnsureNotPurged() = 0;
	virtual int				Index() const = 0;
	virtual int				GetLineNum() const = 0;
	virtual const char* 	GetFileName() const = 0;
	virtual void			GetText( char* text ) const = 0;
	virtual int				GetTextLength() const = 0;
	virtual void			SetText( const char* text ) = 0;
	virtual bool			ReplaceSourceFileText() = 0;
	virtual bool			SourceFileChanged() const = 0;
	virtual void			MakeDefault() = 0;
	virtual bool			EverReferenced() const = 0;
	virtual bool			SetDefaultText() = 0;
	virtual const char* 	DefaultDefinition() const = 0;
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion ) = 0;
	virtual void			FreeData() = 0;
	virtual size_t			Size() const = 0;
	virtual void			List() const = 0;
	virtual void			Print() const = 0;
};


class idDecl
{
public:
	// The constructor should initialize variables such that
	// an immediate call to FreeData() does no harm.
	idDecl()
	{
		base = NULL;
	}
	virtual 				~idDecl() {};
	
	// Returns the name of the decl.
	const char* 			GetName() const
	{
		return base->GetName();
	}
	
	// Returns the decl type.
	declType_t				GetType() const
	{
		return base->GetType();
	}
	
	// Returns the decl state which is usefull for finding out if a decl defaulted.
	declState_t				GetState() const
	{
		return base->GetState();
	}
	
	// Returns true if the decl was defaulted or the text was created with a call to SetDefaultText.
	bool					IsImplicit() const
	{
		return base->IsImplicit();
	}
	
	// The only way non-manager code can have an invalid decl is if the *ByIndex()
	// call was used with forceParse = false to walk the lists to look at names
	// without touching the media.
	bool					IsValid() const
	{
		return base->IsValid();
	}
	
	// Sets state back to unparsed.
	// Used by decl editors to undo any changes to the decl.
	void					Invalidate()
	{
		base->Invalidate();
	}
	
	// if a pointer might possible be stale from a previous level,
	// call this to have it re-parsed
	void					EnsureNotPurged()
	{
		base->EnsureNotPurged();
	}
	
	// Returns the index in the per-type list.
	int						Index() const
	{
		return base->Index();
	}
	
	// Returns the line number the decl starts.
	int						GetLineNum() const
	{
		return base->GetLineNum();
	}
	
	// Returns the name of the file in which the decl is defined.
	const char* 			GetFileName() const
	{
		return base->GetFileName();
	}
	
	// Returns the decl text.
	void					GetText( char* text ) const
	{
		base->GetText( text );
	}
	
	// Returns the length of the decl text.
	int						GetTextLength() const
	{
		return base->GetTextLength();
	}
	
	// Sets new decl text.
	void					SetText( const char* text )
	{
		base->SetText( text );
	}
	
	// Saves out new text for the decl.
	// Used by decl editors to replace the decl text in the source file.
	bool					ReplaceSourceFileText()
	{
		return base->ReplaceSourceFileText();
	}
	
	// Returns true if the source file changed since it was loaded and parsed.
	bool					SourceFileChanged() const
	{
		return base->SourceFileChanged();
	}
	
	// Frees data and makes the decl a default.
	void					MakeDefault()
	{
		base->MakeDefault();
	}
	
	// Returns true if the decl was ever referenced.
	bool					EverReferenced() const
	{
		return base->EverReferenced();
	}
	
public:
	// Sets textSource to a default text if necessary.
	// This may be overridden to provide a default definition based on the
	// decl name. For instance materials may default to an implicit definition
	// using a texture with the same name as the decl.
	virtual bool			SetDefaultText()
	{
		return base->SetDefaultText();
	}
	
	// Each declaration type must have a default string that it is guaranteed
	// to parse acceptably. When a decl is not explicitly found, is purged, or
	// has an error while parsing, MakeDefault() will do a FreeData(), then a
	// Parse() with DefaultDefinition(). The defaultDefintion should start with
	// an open brace and end with a close brace.
	virtual const char* 	DefaultDefinition() const
	{
		return base->DefaultDefinition();
	}
	
	// The manager will have already parsed past the type, name and opening brace.
	// All necessary media will be touched before return.
	// The manager will have called FreeData() before issuing a Parse().
	// The subclass can call MakeDefault() internally at any point if
	// there are parse errors.
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion = false )
	{
		return base->Parse( text, textLength, allowBinaryVersion );
	}
	
	// Frees any pointers held by the subclass. This may be called before
	// any Parse(), so the constructor must have set sane values. The decl will be
	// invalid after issuing this call, but it will always be immediately followed
	// by a Parse()
	virtual void			FreeData()
	{
		base->FreeData();
	}
	
	// Returns the size of the decl in memory.
	virtual size_t			Size() const
	{
		return base->Size();
	}
	
	// If this isn't overridden, it will just print the decl name.
	// The manager will have printed 7 characters on the line already,
	// containing the reference state and index number.
	virtual void			List() const
	{
		base->List();
	}
	
	// The print function will already have dumped the text source
	// and common data, subclasses can override this to dump more
	// explicit data.
	virtual void			Print() const
	{
		base->Print();
	}
	
public:
	idDeclBase* 			base;
};