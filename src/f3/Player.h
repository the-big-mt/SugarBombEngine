/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2020 SugarBombEngine Developers

This file is part of F3GOATY

F3GOATY is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

F3GOATY is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with F3GOATY. If not, see <http://www.gnu.org/licenses/>.

In addition, F3GOATY is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
F3GOATY source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

/*
#include <cstddef>

#include "../cm/CollisionModel.h"
#include "../d3xp/Actor.h"
#include "../d3xp/AimAssist.h"
#include "../d3xp/Entity.h"
#include "../d3xp/GameEdit.h"
#include "../d3xp/PlayerIcon.h"
#include "../d3xp/PlayerView.h"
#include "../d3xp/gamesys/Class.h"
#include "../d3xp/gamesys/Event.h"
#include "../d3xp/physics/Physics_Player.h"
#include "../d3xp/script/Script_Program.h"
#include "../framework/CVarSystem.h"
#include "../framework/UsercmdGen.h"
#include "../idlib/Heap.h"
#include "../idlib/Lib.h"
#include "../idlib/Str.h"
#include "../idlib/containers/Array.h"
#include "../idlib/containers/HashTable.h"
#include "../idlib/containers/List.h"
#include "../idlib/math/Angles.h"
#include "../idlib/math/Interpolate.h"
#include "../idlib/math/Math.h"
#include "../idlib/math/Matrix.h"
#include "../idlib/math/Quat.h"
#include "../idlib/math/Vector.h"
#include "../idlib/sys/sys_defines.h"
#include "../renderer/Model.h"
#include "../renderer/RenderWorld.h"
#include "../sound/sound.h"
*/
#include "PredictedValue.h"

/*
#include "../d3xp/Weapon.h"             // for idWeapon, ammo_t
#include "../d3xp/Achievements.h"       // for idAchievementManager

#include "../idlib/containers/HashTable.h"       // for idHashTable
*/

#include Inventory.hpp"

//*****************************************************************************

/*
namespace F3GOATY
{

class idAAS;
class idAFEntity_Vehicle;
class idAI;
class idBitMsg;
class idCamera;
class idDeclEmail;
class idDeclPDA;
class idDeclSkin;
class idDeclVideo;
class idDict;
class idFuncMountedObject;
class idItem;
class idLexer;
class idLight;
class idMaterial;
class idPhysics;
class idPlayer;
class idProjectile;
class idRestoreGame;
class idSWF;
class idSaveGame;
class idUserInterface;
class idWeapon;

struct sysEvent_t;

template <class type_> class idPredictedValue;
*/

/*
===============================================================================

	Player entity.

===============================================================================
*/

class idMenuHandler_PDA;
class idMenuHandler_HUD;
class idMenuScreen_HUD;
class idTarget_SetPrimaryObjective;

extern const idEventDef EV_Player_GetButtons;
extern const idEventDef EV_Player_GetMove;
extern const idEventDef EV_Player_GetViewAngles;
extern const idEventDef EV_Player_EnableWeapon;
extern const idEventDef EV_Player_DisableWeapon;
extern const idEventDef EV_Player_ExitTeleporter;
extern const idEventDef EV_Player_SelectWeapon;
extern const idEventDef EV_SpectatorTouch;

const float THIRD_PERSON_FOCUS_DISTANCE	= 512.0f;
const int	LAND_DEFLECT_TIME = 150;
const int	LAND_RETURN_TIME = 300;
const int	FOCUS_TIME = 300;
const int	FOCUS_GUI_TIME = 500;
const int	NUM_QUICK_SLOTS = 4;

const int DEAD_HEARTRATE = 0;			// fall to as you die
const int LOWHEALTH_HEARTRATE_ADJ = 20; //
const int DYING_HEARTRATE = 30;			// used for volumen calc when dying/dead
const int BASE_HEARTRATE = 70;			// default
const int ZEROSTAMINA_HEARTRATE = 115;  // no stamina
const int MAX_HEARTRATE = 130;			// maximum
const int ZERO_VOLUME = -40;			// volume at zero
const int DMG_VOLUME = 5;				// volume when taking damage
const int DEATH_VOLUME = 15;			// volume at death

const int SAVING_THROW_TIME = 5000;		// maximum one "saving throw" every five seconds

const int ASYNC_PLAYER_INV_AMMO_BITS = idMath::BitsForInteger( 3000 );
const int ASYNC_PLAYER_INV_CLIP_BITS = -7;								// -7 bits to cover the range [-1, 60]

enum gameExpansionType_t
{
	GAME_BASE,
	GAME_D3XP,
	GAME_D3LE,
	GAME_UNKNOWN
};

struct idObjectiveInfo
{
	idStr title;
	idStr text;
	const idMaterial* screenshot;
};

struct idLevelTriggerInfo
{
	SbStr levelName;
	SbStr triggerName;
};

// powerup modifiers
enum
{
	SPEED = 0,
	PROJECTILE_DAMAGE,
	MELEE_DAMAGE,
	MELEE_DISTANCE
};

// influence levels
enum
{
	INFLUENCE_NONE = 0,			// none
	INFLUENCE_LEVEL1,			// no gun or hud
	INFLUENCE_LEVEL2,			// no gun, hud, movement
	INFLUENCE_LEVEL3,			// slow player movement
};

struct WeaponToggle_t
{
	char		name[64];
	SbList<int, TAG_IDLIB_LIST_PLAYER>	toggleList;
	int			lastUsed;
};

struct loggedAccel_t
{
	int		time;
	SbVec3	dir;		// scaled larger for running
};

struct aasLocation_t
{
	int		areaNum;
	SbVec3	pos;
};

class idPlayer : public idActor
{
public:
	enum
	{
		EVENT_IMPULSE = idEntity::EVENT_MAXEVENTS,
		EVENT_EXIT_TELEPORTER,
		EVENT_ABORT_TELEPORTER,
		EVENT_POWERUP,
		EVENT_SPECTATE,
		EVENT_PICKUPNAME,
		EVENT_FORCE_ORIGIN,
		EVENT_KNOCKBACK,
		EVENT_MAXEVENTS
	};
	
	static const int MAX_PLAYER_PDA = 100;
	//static const int MAX_PLAYER_VIDEO = 100;
	static const int MAX_PLAYER_AUDIO = 100;
	static const int MAX_PLAYER_AUDIO_ENTRIES = 2;
	static const int MAX_PLAYER_NOTE = 100;
	
	usercmd_t				oldCmd;
	usercmd_t				usercmd;
	
	class idPlayerView		playerView;			// handles damage kicks and effects
	
	renderEntity_t			laserSightRenderEntity;	// replace crosshair for 3DTV
	qhandle_t				laserSightHandle;
	
	bool					noclip;
	bool					godmode;
	
	bool					spawnAnglesSet;		// on first usercmd, we must set deltaAngles
	idAngles				spawnAngles;
	idAngles				viewAngles;			// player view angles
	idAngles				cmdAngles;			// player cmd angles
	float					independentWeaponPitchAngle;	// viewAngles[PITCH} when head tracking is active
	
	// For interpolating angles between snapshots
	SbQuat					previousViewQuat;
	SbQuat					nextViewQuat;
	
	int						buttonMask;
	int						oldButtons;
	int						oldImpulseSequence;
	
	int						lastHitTime;			// last time projectile fired by player hit target
	int						lastSndHitTime;			// MP hit sound - != lastHitTime because we throttle
	int						lastSavingThrowTime;	// for the "free miss" effect
	
	bool					pdaHasBeenRead[ MAX_PLAYER_PDA ];
	//bool					videoHasBeenViewed[ MAX_PLAYER_VIDEO ];
	bool					audioHasBeenHeard[ MAX_PLAYER_AUDIO ][ MAX_PLAYER_AUDIO_ENTRIES ];
	
	idScriptBool			AI_FORWARD;
	idScriptBool			AI_BACKWARD;
	idScriptBool			AI_STRAFE_LEFT;
	idScriptBool			AI_STRAFE_RIGHT;
	idScriptBool			AI_ATTACK_HELD;
	idScriptBool			AI_WEAPON_FIRED;
	idScriptBool			AI_JUMP;
	idScriptBool			AI_CROUCH;
	idScriptBool			AI_ONGROUND;
	idScriptBool			AI_ONLADDER;
	idScriptBool			AI_DEAD;
	idScriptBool			AI_RUN;
	idScriptBool			AI_PAIN;
	idScriptBool			AI_HARDLANDING;
	idScriptBool			AI_SOFTLANDING;
	idScriptBool			AI_RELOAD;
	idScriptBool			AI_TELEPORT;
	idScriptBool			AI_TURN_LEFT;
	idScriptBool			AI_TURN_RIGHT;
	
	// inventory
	idInventory				inventory;
	idTarget_SetPrimaryObjective* primaryObjective;
	
	int						flashlightBattery;
	idEntityPtr<idWeapon>	flashlight;
	
	idEntityPtr<idWeapon>	weapon;
	idMenuHandler_HUD* 		hudManager;
	idMenuScreen_HUD* 		hud;
	idMenuHandler_PDA* 		pdaMenu;
	idSWF* 					mpMessages;
	bool					objectiveSystemOpen;
	int						quickSlot[ NUM_QUICK_SLOTS ];
	
	int						weapon_pipboy;
	int						weapon_fists;
	int						weapon_flashlight;
	bool					harvest_lock;
	
	int						heartRate;
	idInterpolate<float>	heartInfo;
	int						lastHeartAdjust;
	int						lastHeartBeat;
	int						lastDmgTime;
	int						deathClearContentsTime;
	bool					doingDeathSkin;
	int						lastArmorPulse;		// lastDmgTime if we had armor at time of hit
	float					stamina;
	float					healthPool;			// amount of health to give over time
	int						nextHealthPulse;
	bool					healthPulse;
	bool					healthTake;
	int						nextHealthTake;
	
	//-----------------------------------------------------------------
	// controller shake parms
	//-----------------------------------------------------------------
	
	const static int		MAX_SHAKE_BUFFER = 3;
	float					controllerShakeHighMag[ MAX_SHAKE_BUFFER ];		// magnitude of the high frequency controller shake
	float					controllerShakeLowMag[ MAX_SHAKE_BUFFER ];		// magnitude of the low frequency controller shake
	int						controllerShakeHighTime[ MAX_SHAKE_BUFFER ];	// time the controller shake ends for high frequency.
	int						controllerShakeLowTime[ MAX_SHAKE_BUFFER ];		// time the controller shake ends for low frequency.
	int						controllerShakeTimeGroup;
	
	bool					hiddenWeapon;		// if the weapon is hidden ( in noWeapons maps )
	
	idAimAssist				aimAssist;
	
	int						spectator;
	bool					forceScoreBoard;
	bool					forceRespawn;
	bool					spectating;
	int						lastSpectateTeleport;
	bool					lastHitToggle;
	bool					wantSpectate;		// from userInfo
	bool					weaponGone;			// force stop firing
	bool					useInitialSpawns;	// toggled by a map restart to be active for the first game spawn
	int						tourneyRank;		// for tourney cycling - the higher, the more likely to play next - server
	int						tourneyLine;		// client side - our spot in the wait line. 0 means no info.
	int						spawnedTime;		// when client first enters the game
	
	bool					carryingFlag;		// is the player carrying the flag?
	
	idEntityPtr<idEntity>	teleportEntity;		// while being teleported, this is set to the entity we'll use for exit
	int						teleportKiller;		// entity number of an entity killing us at teleporter exit
	bool					lastManOver;		// can't respawn in last man anymore (srv only)
	bool					lastManPlayAgain;	// play again when end game delay is cancelled out before expiring (srv only)
	bool					lastManPresent;		// true when player was in when game started (spectators can't join a running LMS)
	bool					isLagged;			// replicated from server, true if packets haven't been received from client.
	int						isChatting;			// replicated from server, true if the player is chatting.
	
	// timers
	int						minRespawnTime;		// can respawn when time > this, force after g_forcerespawn
	int						maxRespawnTime;		// force respawn after this time
	
	// the first person view values are always calculated, even
	// if a third person view is used
	SbVec3					firstPersonViewOrigin;
	SbMat3					firstPersonViewAxis;
	
	idDragEntity			dragEntity;
	
	idFuncMountedObject*		mountedObject;
	idEntityPtr<idLight>	enviroSuitLight;
	
	bool					healthRecharge;
	int						lastHealthRechargeTime;
	int						rechargeSpeed;
	
	float					new_g_damageScale;
	
	bool					bloomEnabled;
	float					bloomSpeed;
	float					bloomIntensity;
	
public:
	CLASS_PROTOTYPE( idPlayer );
	
	idPlayer();
	virtual					~idPlayer();
	
	void					Spawn();
	void					Think();
	
	void					UpdateLaserSight();
	
	// save games
	void					Save( idSaveGame* savefile ) const;					// archives object for save game file
	void					Restore( idRestoreGame* savefile );					// unarchives object from save game file
	
	virtual void			Hide();
	virtual void			Show();
	
	void					Init();
	void					PrepareForRestart();
	virtual void			Restart();
	void					LinkScriptVariables();
	void					SetupWeaponEntity();
	void					SelectInitialSpawnPoint( SbVec3& origin, idAngles& angles );
	void					SpawnFromSpawnSpot();
	void					SpawnToPoint( const SbVec3&	spawn_origin, const idAngles& spawn_angles );
	void					SetClipModel();	// spectator mode uses a different bbox size
	
	void					SavePersistantInfo();
	void					RestorePersistantInfo();
	void					SetLevelTrigger( const char* levelName, const char* triggerName );
	
	void					CacheWeapons();
	
	void					EnterCinematic();
	void					ExitCinematic();
	
	void					UpdateConditions();
	void					SetViewAngles( const idAngles& angles );
	
	// Controller Shake
	void					ControllerShakeFromDamage( int damage );
	void					SetControllerShake( float highMagnitude, int highDuration, float lowMagnitude, int lowDuration );
	void					ResetControllerShake();
	void					GetControllerShake( int& highMagnitude, int& lowMagnitude ) const;
	
	idAimAssist* 			GetAimAssist()
	{
		return &aimAssist;
	}
	
	// delta view angles to allow movers to rotate the view of the player
	void					UpdateDeltaViewAngles( const idAngles& angles );
	
	virtual bool			Collide( const trace_t& collision, const SbVec3& velocity );
	
	virtual void			GetAASLocation( idAAS* aas, idVec3& pos, int& areaNum ) const;
	virtual void			GetAIAimTargets( const idVec3& lastSightPos, idVec3& headPos, idVec3& chestPos );
	virtual void			DamageFeedback( idEntity* victim, idEntity* inflictor, int& damage );
	void					CalcDamagePoints( idEntity* inflictor, idEntity* attacker, const idDict* damageDef,
			const float damageScale, const int location, int* health, int* armor );
	virtual	void			Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );
	
	// New damage path for instant client feedback.
	void					ServerDealDamage( int damage, idEntity& inflictor, idEntity& attacker, const idVec3& dir, const char* damageDefName, const int location );     // Actually updates the player's health independent of feedback.
	int						AdjustDamageAmount( const int inputDamage );
	
	// use exitEntityNum to specify a teleport with private camera view and delayed exit
	virtual void			Teleport( const idVec3& origin, const idAngles& angles, idEntity* destination );
	
	void					Kill( bool delayRespawn, bool nodamage );
	virtual void			Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
	void					StartFxOnBone( const char* fx, const char* bone );
	
	renderView_t* 			GetRenderView();
	void					CalculateRenderView();	// called every tic by player code
	void					CalculateFirstPersonView();
	
	void					AddChatMessage( int index, int alpha, const idStr& message );
	void					UpdateSpectatingText();
	void					ClearChatMessage( int index );
	
	void					DrawHUD( idMenuHandler_HUD* hudManager );
	
	void					WeaponFireFeedback( const idDict* weaponDef );
	
	float					DefaultFov() const;
	float					CalcFov( bool honorZoom );
	void					CalculateViewWeaponPos( idVec3& origin, idMat3& axis );
	idVec3					GetEyePosition() const;
	void					GetViewPos( idVec3& origin, idMat3& axis ) const;
	void					OffsetThirdPersonView( float angle, float range, float height, bool clip );
	
	bool					Give( const char* statname, const char* value, unsigned int giveFlags );
	bool					GiveItem( idItem* item, unsigned int giveFlags );
	void					GiveItem( const char* name );
	void					GiveHealthPool( float amt );
	
	void							SetPrimaryObjective( idTarget_SetPrimaryObjective* target )
	{
		primaryObjective = target;
	}
	idTarget_SetPrimaryObjective* 	GetPrimaryObjective()
	{
		return primaryObjective;
	}
	
	idInventory& 			GetInventory()
	{
		return inventory;
	}
	bool					GiveInventoryItem( idDict* item, unsigned int giveFlags );
	void					RemoveInventoryItem( idDict* item );
	bool					GiveInventoryItem( const char* name );
	void					RemoveInventoryItem( const char* name );
	idDict* 				FindInventoryItem( const char* name );
	idDict* 				FindInventoryItem( int index );
	int						GetNumInventoryItems();
	void					PlayAudioLog( const idSoundShader* sound );
	void					EndAudioLog();
	void					PlayVideoDisk( const idDeclVideo* decl );
	void					EndVideoDisk();
	const idMaterial* 		GetVideoMaterial()
	{
		return pdaVideoMat;
	}
	
	void					SetQuickSlot( int index, int val );
	int						GetQuickSlot( int index );
	
	void					GivePDA( const idDeclPDA* pda, const char* securityItem );
	//void					GiveVideo( const idDeclVideo* video, const char* itemName );
	//void					GiveEmail( const idDeclEmail* email );
	void					GiveNote( const idDeclNote* note );
	void					GiveSecurity( const char* security );
	void					GiveObjective( const char* title, const char* text, const idMaterial* screenshot );
	void					CompleteObjective( const char* title );
	
	// Quests
	void GiveQuest(const char *title); // TODO: AddQuest?
	void					CompleteQuest( const char* title, bool failed = false); // const CGameQuest &aQuest?
	
	// Levels & XP
	void AddXP(int amount);
	void SetLevel(int level);
	void LevelUp();
	
	bool					GivePowerUp( int powerup, int time, unsigned int giveFlags );
	void					ClearPowerUps();
	bool					PowerUpActive( int powerup ) const;
	float					PowerUpModifier( int type );
	
	int						SlotForWeapon( const char* weaponName );
	void					Reload();
	void					NextWeapon();
	void					NextBestWeapon();
	void					PrevWeapon();
	void					SetPreviousWeapon( int num )
	{
		previousWeapon = num;
	}
	void					SelectWeapon( int num, bool force );
	void					DropWeapon( bool died ) ;
	void					StealWeapon( idPlayer* player );
	void					AddProjectilesFired( int count );
	void					AddProjectileHits( int count );
	void					SetLastHitTime( int time );
	void					LowerWeapon();
	void					RaiseWeapon();
	void					WeaponLoweringCallback();
	void					WeaponRisingCallback();
	void					RemoveWeapon( const char* weap );
	void					RemoveAllButEssentialWeapons();
	bool					CanShowWeaponViewmodel() const;
	
	void					AddAIKill();
	
	void					AdjustHeartRate( int target, float timeInSecs, float delay, bool force );
	void					SetCurrentHeartRate();
	int						GetBaseHeartRate();
	void					UpdateAir();
	
	void					UpdatePowerupHud();
	
	virtual bool			HandleSingleGuiCommand( idEntity* entityGui, idLexer* src );
	bool					GuiActive()
	{
		return focusGUIent != NULL;
	}
	
	bool					HandleGuiEvents( const sysEvent_t* ev );
	void					PerformImpulse( int impulse );
	void					Spectate( bool spectate, bool force = false );
	void					TogglePipBoy();
	void					RouteGuiMouse( idUserInterface* gui );
	void					UpdateHud();
	const idDeclPDA* 		GetPDA() const;
	bool					GetPipBoyOpen() const
	{
		return objectiveSystemOpen;
	}
	//const idDeclVideo* 		GetVideo( int index );
	void					SetInfluenceFov( float fov );
	void					SetInfluenceView( const char* mtr, const char* skinname, float radius, idEntity* ent );
	void					SetInfluenceLevel( int level );
	int						GetInfluenceLevel()
	{
		return influenceActive;
	};
	void					SetPrivateCameraView( idCamera* camView );
	idCamera* 				GetPrivateCameraView() const
	{
		return privateCameraView;
	}
	void					StartFxFov( float duration );
	void					UpdateHudWeapon( bool flashWeapon = true );
	void					UpdateChattingHud();
	void					UpdateHudStats( idMenuHandler_HUD* hudManager );
	void					Event_StopAudioLog();
	bool					IsSoundChannelPlaying( const s_channelType channel = SND_CHANNEL_ANY );
	void					ShowTip( const char* title, const char* tip, bool autoHide );
	void					HideTip();
	bool					IsTipVisible()
	{
		return tipUp;
	};
	void					HideObjective();
	
	virtual void			ClientThink( const int curTime, const float fraction, const bool predict );
	virtual void			WriteToSnapshot( idBitMsg& msg ) const;
	virtual void			ReadFromSnapshot( const idBitMsg& msg );
	void					WritePlayerStateToSnapshot( idBitMsg& msg ) const;
	void					ReadPlayerStateFromSnapshot( const idBitMsg& msg );
	
	virtual bool			ServerReceiveEvent( int event, int time, const idBitMsg& msg );
	
	virtual bool			GetPhysicsToVisualTransform( SbVec3& origin, SbMat3& axis );
	virtual bool			GetPhysicsToSoundTransform( SbVec3& origin, SbMat3& axis );
	
	virtual bool			ClientReceiveEvent( int event, int time, const idBitMsg& msg );
	bool					IsRespawning();
	bool					IsInTeleport();
	
	int						GetSkinIndex() const
	{
		return skinIndex;
	}
	
	idEntity*				GetInfluenceEntity()
	{
		return influenceEntity;
	};
	const idMaterial*		GetInfluenceMaterial()
	{
		return influenceMaterial;
	};
	float					GetInfluenceRadius()
	{
		return influenceRadius;
	};
	
	// server side work for in/out of spectate. takes care of spawning it into the world as well
	void					ServerSpectate( bool spectate );
	// for very specific usage. != GetPhysics()
	idPhysics*				GetPlayerPhysics();
	void					TeleportDeath( int killer );
	void					SetLeader( bool lead );
	bool					IsLeader();
	
	void					UpdateSkinSetup();
	
	bool					OnLadder() const;
	
	virtual	void			UpdatePlayerIcons();
	virtual	void			DrawPlayerIcons();
	virtual	void			HidePlayerIcons();
	bool					NeedsIcon();
	
	void					StartHealthRecharge( int speed );
	void					StopHealthRecharge();
	
	idStr					GetCurrentWeapon();
	int						GetCurrentWeaponSlot()
	{
		return currentWeapon;
	}
	int						GetIdealWeapon()
	{
		return idealWeapon.Get();
	}
	idHashTable<WeaponToggle_t>	GetWeaponToggles() const
	{
		return weaponToggles;
	}
	
	bool					CanGive( const char* statname, const char* value );
	
	void					StopHelltime( bool quick = true );
	void					PlayHelltimeStopSound();
	
	void					DropFlag();	// drop CTF item
	void					ReturnFlag();
	virtual void			FreeModelDef();
	
	bool					SelfSmooth();
	void					SetSelfSmooth( bool b );
	
	const idAngles& 		GetViewBobAngles()
	{
		return viewBobAngles;
	}
	const SbVec3& 			GetViewBob()
	{
		return viewBob;
	}
	
	idAchievementManager& 			GetAchievementManager()
	{
		return achievementManager;
	}
	const idAchievementManager& 	GetAchievementManager() const
	{
		return achievementManager;
	}
	
	int						GetPlayedTime() const
	{
		return playedTimeSecs;
	}
	
	void					HandleUserCmds( const usercmd_t& newcmd );
	
	int						GetClientFireCount() const
	{
		return clientFireCount;
	}
	void					IncrementFireCount()
	{
		++clientFireCount;
	}
	
	void					ShowRespawnHudMessage();
	void					HideRespawnHudMessage();
	
	bool					IsLocallyControlled() const
	{
		return entityNumber == gameLocal.GetLocalClientNum();
	}
	
	gameExpansionType_t		GetExpansionType() const;
	
	void					AddProjectileKills()
	{
		numProjectileKills++;
	}
	int						GetProjectileKills() const
	{
		return numProjectileKills;
	}
	void					ResetProjectileKills()
	{
		numProjectileKills = 0;
	}
private:
	// Stats & achievements
	idAchievementManager	achievementManager;
	
	int						playedTimeSecs;
	int						playedTimeResidual;
	
	jointHandle_t			hipJoint;
	jointHandle_t			chestJoint;
	jointHandle_t			headJoint;
	
	idPhysics_Player		physicsObj;			// player physics
	
	SbList<aasLocation_t, TAG_IDLIB_LIST_PLAYER>	aasLocation;		// for AI tracking the player
	
	int						bobFoot;
	float					bobFrac;
	float					bobfracsin;
	int						bobCycle;			// for view bobbing and footstep generation
	float					xyspeed;
	int						stepUpTime;
	float					stepUpDelta;
	float					idealLegsYaw;
	float					legsYaw;
	bool					legsForward;
	float					oldViewYaw;
	idAngles				viewBobAngles;
	SbVec3					viewBob;
	int						landChange;
	int						landTime;
	
	
	int						currentWeapon;
	idPredictedValue< int >	idealWeapon;
	int						previousWeapon;
	int						weaponSwitchTime;
	bool					weaponEnabled;
	
	int						skinIndex;
	const idDeclSkin* 		skin;
	const idDeclSkin* 		powerUpSkin;
	
	int						numProjectilesFired;	// number of projectiles fired
	int						numProjectileHits;		// number of hits on mobs
	int						numProjectileKills;		// number of kills with a projectile.
	
	bool					airless;
	int						airMsec;				// set to pm_airMsec at start, drops in vacuum
	int						lastAirDamage;
	
	bool					gibDeath;
	bool					gibsLaunched;
	SbVec3					gibsDir;
	
	idInterpolate<float>	zoomFov;
	idInterpolate<float>	centerView;
	bool					fxFov;
	
	float					influenceFov;
	int						influenceActive;		// level of influence.. 1 == no gun or hud .. 2 == 1 + no movement
	idEntity* 				influenceEntity;
	const idMaterial* 		influenceMaterial;
	float					influenceRadius;
	const idDeclSkin* 		influenceSkin;
	
	idCamera* 				privateCameraView;
	
	static const int		NUM_LOGGED_VIEW_ANGLES = 64;		// for weapon turning angle offsets
	idAngles				loggedViewAngles[NUM_LOGGED_VIEW_ANGLES];	// [gameLocal.framenum&(LOGGED_VIEW_ANGLES-1)]
	static const int		NUM_LOGGED_ACCELS = 16;			// for weapon turning angle offsets
	loggedAccel_t			loggedAccel[NUM_LOGGED_ACCELS];	// [currentLoggedAccel & (NUM_LOGGED_ACCELS-1)]
	int						currentLoggedAccel;
	
	// if there is a focusGUIent, the attack button will be changed into mouse clicks
	idEntity* 				focusGUIent;
	idUserInterface* 		focusUI;				// focusGUIent->renderEntity.gui, gui2, or gui3
	idAI* 					focusCharacter;
	int						talkCursor;				// show the state of the focusCharacter (0 == can't talk/dead, 1 == ready to talk, 2 == busy talking)
	int						focusTime;
	idAFEntity_Vehicle* 	focusVehicle;
	idUserInterface* 		cursor;
	
	// full screen guis track mouse movements directly
	int						oldMouseX;
	int						oldMouseY;
	
	const idMaterial* 		pdaVideoMat;
	
	bool					tipUp;
	bool					objectiveUp;
	
	int						lastDamageDef;
	SbVec3					lastDamageDir;
	int						lastDamageLocation;
	int						smoothedFrame;
	bool					smoothedOriginUpdated;
	SbVec3					smoothedOrigin;
	idAngles				smoothedAngles;
	
	idHashTable<WeaponToggle_t>	weaponToggles;
	
	int						hudPowerup;
	int						lastHudPowerup;
	int						hudPowerupDuration;
	
	int mnLevelXP{0};
	int mnTotalXP{0};
	
	int mnLevel{0};
	
	// mp
	bool					respawning;				// set to true while in SpawnToPoint for telefrag checks
	bool					leader;					// for sudden death situations
	int						lastSpectateChange;
	int						lastTeleFX;
	bool					weaponCatchup;			// raise up the weapon silently ( state catchups )
	int						MPAim;					// player num in aim
	int						lastMPAim;
	int						lastMPAimTime;			// last time the aim changed
	int						MPAimFadeTime;			// for GUI fade
	bool					MPAimHighlight;
	bool					isTelefragged;			// proper obituaries
	int						serverOverridePositionTime;
	int						clientFireCount;
	
	idPlayerIcon			playerIcon;
	
	bool					selfSmooth;
	
	netBoolEvent_t			respawn_netEvent;
	
	void					LookAtKiller( idEntity* inflictor, idEntity* attacker );
	
	void					StopFiring();
	void					FireWeapon();
	void					Weapon_Combat();
	void					Weapon_NPC();
	void					Weapon_GUI();
	void					UpdateWeapon();
	void					UpdateFlashlight();
	void					FlashlightOn();
	void					FlashlightOff();
	void					UpdateSpectating();
	void					SpectateFreeFly( bool force );	// ignore the timeout to force when followed spec is no longer valid
	void					SpectateCycle();
	idAngles				GunTurningOffset();
	SbVec3					GunAcceleratingOffset();
	
	void					UseObjects();
	void					CrashLand( const SbVec3& oldOrigin, const SbVec3& oldVelocity );
	void					BobCycle( const SbVec3& pushVelocity );
	void					UpdateViewAngles();
	void					EvaluateControls();
	void					AdjustSpeed();
	void					AdjustBodyAngles();
	void					InitAASLocation();
	void					SetAASLocation();
	void					Move();
	void					Move_Interpolated( float fraction );
	void					RunPhysics_RemoteClientCorrection();
	void					UpdatePowerUps();
	void					UpdateDeathSkin( bool state_hitch );
	void					ClearPowerup( int i );
	void					SetSpectateOrigin();
	bool					AllowClientAuthPhysics();
	virtual int				GetPhysicsTimeStep() const;
	
	void					ClearFocus();
	void					UpdateFocus();
	void					UpdateLocation();
	idUserInterface* 		ActiveGui();
	
	// mp
	void					Respawn_Shared();
	
	bool					WeaponAvailable( const char* name );
	
	void					UseVehicle();
	
	void					Event_GetButtons();
	void					Event_GetMove();
	void					Event_GetViewAngles();
	void					Event_StopFxFov();
	void					Event_EnableWeapon();
	void					Event_DisableWeapon();
	void					Event_GetCurrentWeapon();
	void					Event_GetPreviousWeapon();
	void					Event_SelectWeapon( const char* weaponName );
	void					Event_GetWeaponEntity();
	void					Event_OpenPipBoy();
	void					Event_PDAAvailable();
	void					Event_InPipBoy();
	void					Event_ExitTeleporter();
	void					Event_HideTip();
	void					Event_LevelTrigger();
	void					Event_Gibbed();
	void					Event_ForceOrigin( SbVec3& origin, idAngles& angles );
	void					Event_GiveInventoryItem( const char* name );
	void					Event_RemoveInventoryItem( const char* name );
	
	void					Event_GetIdealWeapon();
	void					Event_WeaponAvailable( const char* name );
	void					Event_SetPowerupTime( int powerup, int time );
	void					Event_IsPowerupActive( int powerup );
	void					Event_StartWarp();
	void					Event_StopHelltime( int mode );
	void					Event_ToggleBloom( int on );
	void					Event_SetBloomParms( float speed, float intensity );
	
	// openmw
private:
	bool                    mAutoMove;
public:
	bool getAutoMove() const;
    void setAutoMove (bool enable);
};

ID_INLINE bool idPlayer::IsRespawning()
{
	return respawning;
};

ID_INLINE idPhysics* idPlayer::GetPlayerPhysics()
{
	return &physicsObj;
};

ID_INLINE bool idPlayer::IsInTeleport()
{
	return ( teleportEntity.GetEntity() != nullptr );
};

ID_INLINE void idPlayer::SetLeader( bool lead )
{
	leader = lead;
};

ID_INLINE bool idPlayer::IsLeader()
{
	return leader;
};

ID_INLINE bool idPlayer::SelfSmooth()
{
	return selfSmooth;
};

ID_INLINE void idPlayer::SetSelfSmooth( bool b )
{
	selfSmooth = b;
};

extern idCVar g_infiniteAmmo;