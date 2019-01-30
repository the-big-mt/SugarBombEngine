/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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
#pragma hdrstop
#include "precompiled.h"
#include "../Game_local.h"

void idMenuWidget_PDA_VideoInfo::Update()
{

	if( GetSWFObject() == nullptr )
	{
		return;
	}
	
	idSWFScriptObject& root = GetSWFObject()->GetRootObject();
	if( !BindSprite( root ) || GetSprite() == nullptr )
	{
		return;
	}
	
	idPlayer* player = gameLocal.GetLocalPlayer();
	if( player == nullptr )
	{
		return;
	}
	
	idSWFTextInstance* txtHeading = GetSprite()->GetScriptObject()->GetNestedText( "txtName" );
	idSWFTextInstance* txtInfo = GetSprite()->GetScriptObject()->GetNestedText( "txtInfo" );
	
	int numVideos = player->GetInventory().videos.Num();
	if( numVideos != 0 )
	{
		const idDeclVideo* video = player->GetVideo( videoIndex );
		if( video != nullptr )
		{
			if( txtHeading != nullptr )
			{
				txtHeading->SetText( video->GetVideoName() );
			}
			
			if( txtInfo != nullptr )
			{
				txtInfo->SetText( video->GetInfo() );
			}
		}
	}
	else
	{
		if( txtHeading != nullptr )
		{
			txtHeading->SetText( "" );
		}
		
		if( txtInfo != nullptr )
		{
			txtInfo->SetText( "" );
		}
	}
}

void idMenuWidget_PDA_VideoInfo::ObserveEvent( const idMenuWidget& widget, const idWidgetEvent& event )
{
	const idMenuWidget_Button* const button = dynamic_cast< const idMenuWidget_Button* >( &widget );
	if( button == nullptr )
	{
		return;
	}
	
	const idMenuWidget* const listWidget = button->GetParent();
	
	if( listWidget == nullptr )
	{
		return;
	}
	
	switch( event.type )
	{
		case WIDGET_EVENT_FOCUS_ON:
		{
			const idMenuWidget_DynamicList* const list = dynamic_cast< const idMenuWidget_DynamicList* const >( listWidget );
			videoIndex = list->GetViewIndex();
			
			idPlayer* player = gameLocal.GetLocalPlayer();
			if( player != nullptr )
			{
				player->EndVideoDisk();
				
				const idDeclVideo* video = player->GetVideo( videoIndex );
				
				if( video != nullptr )
				{
					idSWFSpriteInstance* videoSprite = GetSprite()->GetScriptObject()->GetNestedSprite( "video", "img" );
					if( videoSprite != nullptr )
					{
						videoSprite->SetMaterial( video->GetPreview() );
					}
				}
			}
			
			if( GetParent() != nullptr )
			{
				idMenuScreen_PDA_VideoDisks* screen = dynamic_cast< idMenuScreen_PDA_VideoDisks* const >( GetParent() );
				if( screen != nullptr )
				{
					screen->Update();
				}
			}
			
			break;
		}
	}
}

