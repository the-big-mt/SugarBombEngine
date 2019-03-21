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

#pragma hdrstop
#include "precompiled.h"
#include "../Game_local.h"

/*
========================
idMenuScreen_Shell_DisplayOptions::Initialize
========================
*/
void idMenuScreen_Shell_DisplayOptions::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
	
	if( data != nullptr )
	{
		menuGUI = data->GetGUI();
	}
	
	SetSpritePath( "menuDisplayOptions" );
	
	options = new( TAG_SWF ) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_GAME_OPTIONS_OPTIONS );
	options->SetSpritePath( GetSpritePath(), "info", "options" );
	options->SetWrappingAllowed( true );
	options->SetControlList( true );
	options->Initialize( data );
	AddChild( options );
	
	btnBack = new( TAG_SWF ) idMenuWidget_Button();
	btnBack->Initialize( data );
	btnBack->SetLabel( "#str_swf_settings" );
	btnBack->SetSpritePath( GetSpritePath(), "info", "btnBack" );
	btnBack->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_GO_BACK );
	AddChild( btnBack );
	
	idMenuWidget_ControlButton* control;
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TOGGLE );
	control->SetLabel( "#str_02135" );	// Auto Weapon Switch
	control->SetDataSource( &systemData, idMenuDataSource_GameSettings::GAME_FIELD_AUTO_SWITCH );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_PRESS_FOCUSED, options->GetChildren().Num() );
	options->AddChild( control );
	
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ) );
};

/*
========================
idMenuScreen_Shell_DisplayOptions::Update
========================
*/
void idMenuScreen_Shell_DisplayOptions::Update()
{
};

/*
========================
idMenuScreen_Shell_DisplayOptions::ShowScreen
========================
*/
void idMenuScreen_Shell_DisplayOptions::ShowScreen( const mainMenuTransition_t transitionType )
{
	systemData.LoadData();
	idMenuScreen::ShowScreen( transitionType );
};

/*
========================
idMenuScreen_Shell_DisplayOptions::HideScreen
========================
*/
void idMenuScreen_Shell_DisplayOptions::HideScreen( const mainMenuTransition_t transitionType )
{
	if( systemData.IsDataChanged() )
	{
		systemData.CommitData();
	}
	idMenuScreen::HideScreen( transitionType );
};

/*
========================
idMenuScreen_Shell_DisplayOptions::HandleAction
========================
*/
bool idMenuScreen_Shell_DisplayOptions::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{
	return idMenuWidget::HandleAction( action, event, widget, forceHandled );
};

/*
========================
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::idMenuDataSource_DisplaySettings
========================
*/
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::idMenuDataSource_DisplaySettings()
{
	fields.SetNum( MAX_GAME_FIELDS );
	originalFields.SetNum( MAX_GAME_FIELDS );
};

/*
========================
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::LoadData
========================
*/
void idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::LoadData()
{
	fields[ GAME_FIELD_FOV ].SetInteger( g_fov.GetFloat() );
	fields[ GAME_FIELD_CHECKPOINTS ].SetBool( g_checkpoints.GetBool() );
	fields[ GAME_FIELD_AUTO_SWITCH ].SetBool( ui_autoSwitch.GetBool() );
	fields[ GAME_FIELD_AUTO_RELOAD ].SetBool( ui_autoReload.GetBool() );
	fields[ GAME_FIELD_AIM_ASSIST ].SetBool( aa_targetAimAssistEnable.GetBool() );
	fields[ GAME_FIELD_ALWAYS_SPRINT ].SetBool( in_alwaysRun.GetBool() );
	fields[ GAME_FIELD_FLASHLIGHT_SHADOWS ].SetBool( g_weaponShadows.GetBool() );
	fields[ GAME_FIELD_MUZZLE_FLASHES ].SetBool( g_muzzleFlash.GetBool() );
	
	originalFields = fields;
};

/*
========================
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::CommitData
========================
*/
void idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::CommitData()
{
	g_fov.SetFloat( fields[ GAME_FIELD_FOV ].ToFloat() );
	g_gun_x.SetFloat( Lerp( MIN_FOV_GUN, MAX_FOV_GUN, ( fields[ GAME_FIELD_FOV ].ToFloat() - MIN_FOV ) / ( MAX_FOV - MIN_FOV ) ) );
	
	g_checkpoints.SetBool( fields[ GAME_FIELD_CHECKPOINTS ].ToBool() );
	ui_autoSwitch.SetBool( fields[ GAME_FIELD_AUTO_SWITCH ].ToBool() );
	ui_autoReload.SetBool( fields[ GAME_FIELD_AUTO_RELOAD ].ToBool() );
	aa_targetAimAssistEnable.SetBool( fields[ GAME_FIELD_AIM_ASSIST ].ToBool() );
	in_alwaysRun.SetBool( fields[ GAME_FIELD_ALWAYS_SPRINT ].ToBool() );
	g_weaponShadows.SetBool( fields[ GAME_FIELD_FLASHLIGHT_SHADOWS ].ToBool() );
	g_muzzleFlash.SetBool( fields[ GAME_FIELD_MUZZLE_FLASHES ].ToBool() );
	
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
	
	// make the committed fields into the backup fields
	originalFields = fields;
};

/*
========================
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::AdjustField
========================
*/
void idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::AdjustField( const int fieldIndex, const int adjustAmount )
{
	if( fieldIndex == GAME_FIELD_FOV )
	{
		fields[ fieldIndex ].SetInteger( idMath::ClampInt( MIN_FOV, MAX_FOV, fields[ fieldIndex ].ToInteger() + adjustAmount * 5 ) );
	}
	else
	{
		fields[ fieldIndex ].SetBool( !fields[ fieldIndex ].ToBool() );
	}
};

/*
========================
idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_DisplayOptions::idMenuDataSource_DisplaySettings::IsDataChanged() const
{

	if( fields[ GAME_FIELD_FOV ].ToInteger() != originalFields[ GAME_FIELD_FOV ].ToInteger() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_CHECKPOINTS ].ToBool() != originalFields[ GAME_FIELD_CHECKPOINTS ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_AUTO_SWITCH ].ToBool() != originalFields[ GAME_FIELD_AUTO_SWITCH ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_AUTO_RELOAD ].ToBool() != originalFields[ GAME_FIELD_AUTO_RELOAD ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_AIM_ASSIST ].ToBool() != originalFields[ GAME_FIELD_AIM_ASSIST ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_ALWAYS_SPRINT ].ToBool() != originalFields[ GAME_FIELD_ALWAYS_SPRINT ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_FLASHLIGHT_SHADOWS ].ToBool() != originalFields[ GAME_FIELD_FLASHLIGHT_SHADOWS ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_MUZZLE_FLASHES ].ToBool() != originalFields[ GAME_FIELD_MUZZLE_FLASHES ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_DIFFICULTY ].ToInteger() != originalFields[ GAME_FIELD_DIFFICULTY ].ToInteger() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_SAVE_ON_REST ].ToBool() != originalFields[ GAME_FIELD_SAVE_ON_REST ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_SAVE_ON_WAIT ].ToBool() != originalFields[ GAME_FIELD_SAVE_ON_WAIT ].ToBool() )
	{
		return true;
	}
	
	if( fields[ GAME_FIELD_SAVE_ON_TRAVEL ].ToBool() != originalFields[ GAME_FIELD_SAVE_ON_TRAVEL ].ToBool() )
	{
		return true;
	}
	return false;
};

// Brightness
// HUD Opacity
// HUD Color
// Pip-boy Color
// Crosshair (toggle)
// Dialog Subtitles (toggle)
// General Subtitles (toggle)