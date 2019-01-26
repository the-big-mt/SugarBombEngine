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

#ifndef __GAME_WEAPON_H__
#define __GAME_WEAPON_H__

#include "PredictedValue.h"

/*
===============================================================================

	Player Weapon

===============================================================================
*/

extern const idEventDef EV_Weapon_State;

typedef enum
{
	WP_READY,
	WP_OUTOFAMMO,
	WP_RELOAD,
	WP_HOLSTERED,
	WP_RISING,
	WP_LOWERING
} weaponStatus_t;

typedef enum //I am not even sure if the enum strings are correct or not
{
    HandToHand = 0,
    MeleeOneHand = 1,
    MeleeTwoHand = 2,
    PistolBallistic = 3,
    PistolEnergy = 4,
    RifleBallistic = 5,
    RifleAutomatic = 6,
    RifleEnergy = 7,
    Handle = 8,
    Launcher = 9,
    Grenade = 10,
    LandMine = 11,
    MineDrop = 12
} weaponAnimation_t;

typedef enum
{
    HandGrip1 = 171,
    HandGrip2 = 172,
    HandGrip3 = 173,
    Default = 255
} gripAnimation_t;

typedef enum
{
    ReloadA = 0,
    ReloadB = 1,
    ReloadC = 2,
    ReloadD = 3,
    ReloadE = 4,
    ReloadF = 5,
    ReloadG = 6,
    ReloadH = 7,
    ReloadI = 8,
    ReloadJ = 9,
    ReloadK = 10
} reloadAnimation_t;

typedef enum
{
    AttackLeft = 26,
    AttackRight = 32,
    Attack3 = 38,
    Attack4 = 44,
    Attack5 = 50,
    Attack6 = 56,
    Attack7 = 62,
  	Attack8 = 68,
	AttackLoop = 74, 
    AttackSpin = 80,
    AttackSpin2 = 86,
    PlaceMine = 97,
    PlaceMine2 = 103,
    AttackThrow = 109,
    AttackThrow2 = 115,
    AttackThrow3 = 121,
    AttackThrow4 = 127,
    AttackThrow5 = 133,	
    Default = 255
} attackAnimation_t;

typedef enum
{
    Perception = 0,
    Endurance = 1,
    Left_Attack = 2,
    Right_Attack = 3,
    Left_Mobility = 4,
    Right_Mobility = 5,
    Brain = 6,
} embeddedWeaponActor_t;

typedef enum
{
    Default = 0,
    Dismember_only = 1,
    Explode_only = 2,
    No_DismemberExplode = 4
} onhit_t;

typedef enum
{
    IgnoreNormalResistance = 0x01,
    Automatic = 0x02,
    Scope = 0x04,
    Drop = 0x08,
    Hide = 0x10,
    Embedded = 0x20,
    No1stISAnim = 0x40,
    NonPlayable = 0x80
} flag1_t;

typedef enum
{
    PlayerOnly = 0x00000001,
    NPCuseammo = 0x00000002,
    NoJam = 0x00000004,
    OverrideActionPoints = 0x00000008,
    MinorCrime = 0x00000010,
    Range_Fixed = 0x00000020,
    Unused = 0x00000040,
    OverrideDamageMult = 0x00000080,
    No3rdISAnim = 0x00000100,
    ShortBurst = 0x00000200,
    Rumble_Alt = 0x00000400,
    LongBurst = 0x00000800,
} flag2_t;

typedef enum
{
    Constant = 0,
    Square = 1,
    Triangle = 2,
    Sawtooth = 3,
} rumble_t;

typedef enum
{
    OnDeath = 0x01
} CRDTflag_t;

typedef int ammo_t;
static const int AMMO_NUMTYPES = 16;

class idPlayer;

static const int LIGHTID_WORLD_MUZZLE_FLASH = 1;
static const int LIGHTID_VIEW_MUZZLE_FLASH = 100;

class idMoveableItem;

typedef struct
{
	char			name[64];
	char			particlename[128];
	bool			active;
	int				startTime;
	jointHandle_t	joint;			//The joint on which to attach the particle
	bool			smoke;			//Is this a smoke particle
	const idDeclParticle* particle;		//Used for smoke particles
	idFuncEmitter*  emitter;		//Used for non-smoke particles
} WeaponParticle_t;

typedef struct
{
	char			name[64];
	bool			active;
	int				startTime;
	jointHandle_t	joint;
	int				lightHandle;
	renderLight_t	light;
} WeaponLight_t;

class idWeapon : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idWeapon );
	
	idWeapon();
	virtual					~idWeapon();
	
	// Init
	void					Spawn();
	void					SetOwner( idPlayer* owner );
	idPlayer*				GetOwner();
	virtual bool			ShouldConstructScriptObjectAtSpawn() const;
	void					SetFlashlightOwner( idPlayer* owner );
	
	static void				CacheWeapon( const char* weaponName );
	
	// save games
	void					Save( idSaveGame* savefile ) const;					// archives object for save game file
	void					Restore( idRestoreGame* savefile );					// unarchives object from save game file
	
	// Weapon definition management
	void					Clear();
	void					GetWeaponDef( const char* objectname, int ammoinclip );
	bool					IsLinked();
	bool					IsWorldModelReady();
	
	// GUIs
	const char* 			Icon() const;
	void					UpdateGUI();
	const char* 			PdaIcon() const;
	const char* 			DisplayName() const;
	const char* 			Description() const;
	
	virtual void			SetModel( const char* modelname );
	bool					GetGlobalJointTransform( bool viewModel, const jointHandle_t jointHandle, idVec3& offset, idMat3& axis );
	void					SetPushVelocity( const idVec3& pushVelocity );
	bool					UpdateSkin();
	
	// State control/player interface
	void					Think();
	void					Raise();
	void					PutAway();
	void					Reload();
	void					LowerWeapon();
	void					RaiseWeapon();
	void					HideWeapon();
	void					ShowWeapon();
	void					HideWorldModel();
	void					ShowWorldModel();
	void					OwnerDied();
	void					BeginAttack();
	void					EndAttack();
	bool					IsReady() const;
	bool					IsReloading() const;
	bool					IsHolstered() const;
	bool					ShowCrosshair() const;
	idEntity* 				DropItem( const idVec3& velocity, int activateDelay, int removeDelay, bool died );
	bool					CanDrop() const;
	void					WeaponStolen();
	void					ForceAmmoInClip();
	
	weaponStatus_t			GetStatus()
	{
		return status;
	};
	
	
	// Script state management
	virtual idThread* 		ConstructScriptObject();
	virtual void			DeconstructScriptObject();
	void					SetState( const char* statename, int blendFrames );
	void					UpdateScript();
	void					EnterCinematic();
	void					ExitCinematic();
	void					NetCatchup();
	
	// Visual presentation
	void					PresentWeapon( bool showViewModel );
	int						GetZoomFov();
	void					GetWeaponAngleOffsets( int* average, float* scale, float* max );
	void					GetWeaponTimeOffsets( float* time, float* scale );
	bool					BloodSplat( float size );
	void					SetIsPlayerFlashlight( bool bl )
	{
		isPlayerFlashlight = bl;
	}
	void					FlashlightOn();
	void					FlashlightOff();
	
	// Ammo
	static ammo_t			GetAmmoNumForName( const char* ammoname );
	static const char*		GetAmmoNameForNum( ammo_t ammonum );
	static const char*		GetAmmoPickupNameForNum( ammo_t ammonum );
	ammo_t					GetAmmoType() const;
	int						AmmoAvailable() const;
	int						AmmoInClip() const;
	void					ResetAmmoClip();
	int						ClipSize() const;
	int						LowAmmo() const;
	int						AmmoRequired() const;
	int						AmmoCount() const;
	int						GetGrabberState() const;
	
	// Flashlight
	idAnimatedEntity* 		GetWorldModel()
	{
		return worldModel.GetEntity();
	}
	
	virtual void			WriteToSnapshot( idBitMsg& msg ) const;
	virtual void			ReadFromSnapshot( const idBitMsg& msg );
	
	enum
	{
		EVENT_RELOAD = idEntity::EVENT_MAXEVENTS,
		EVENT_ENDRELOAD,
		EVENT_CHANGESKIN,
		EVENT_MAXEVENTS
	};
	virtual bool			ClientReceiveEvent( int event, int time, const idBitMsg& msg );
	
	virtual void			ClientPredictionThink();
	virtual void			ClientThink( const int curTime, const float fraction, const bool predict );
	void					MuzzleFlashLight();
	void					RemoveMuzzleFlashlight();
	
	// Get a global origin and axis suitable for the laser sight or bullet tracing
	// Returns false for hands, grenades, and chainsaw.
	// Can't be const because a frame may need to be created.
	bool					GetMuzzlePositionWithHacks( idVec3& origin, idMat3& axis );
	
	void					GetProjectileLaunchOriginAndAxis( idVec3& origin, idMat3& axis );
	
	const idDeclEntityDef* GetDeclEntityDef()
	{
		return weaponDef;
	}
	
	void                    SetIronsight( bool i );
	
	/// @param damage Unmitigated weapon damage of the attack
	/// @param hit Was the attack successful?
	/// @param weapon The weapon used.
	/// @note if the weapon is unequipped as result of condition damage, a new Ptr will be assigned to \a weapon.
	void ReduceCondition (float damage, bool hit, MWWorld::Ptr& weapon, const MWWorld::Ptr& attacker);

	/// Adjust weapon damage based on its condition. A used weapon will be less effective.
	void AdjustDamage (float& damage, const MWWorld::Ptr& weapon, const MWWorld::Ptr& attacker);
	
	friend class idPlayer;
private:
	// script control
	idScriptBool			WEAPON_ATTACK;
	idScriptBool			WEAPON_RELOAD;
	idScriptBool			WEAPON_NETRELOAD;
	idScriptBool			WEAPON_NETENDRELOAD;
	idScriptBool			WEAPON_NETFIRING;
	idScriptBool			WEAPON_RAISEWEAPON;
	idScriptBool			WEAPON_LOWERWEAPON;
	idScriptBool            WEAPON_IRONSIGHT;
	weaponStatus_t			status;
	idThread* 				thread;
	idStr					state;
	idStr					idealState;
	int						animBlendFrames;
	int						animDoneTime;
	bool					isLinked;
	bool					isPlayerFlashlight;
	
	// precreated projectile
	idEntity*				projectileEnt;
	
	idPlayer* 				owner;
	idEntityPtr<idAnimatedEntity>	worldModel;
	
	// hiding (for GUIs and NPCs)
	int						hideTime;
	float					hideDistance;
	int						hideStartTime;
	float					hideStart;
	float					hideEnd;
	float					hideOffset;
	bool					hide;
	bool					disabled;
	
	// berserk
	int						berserk;
	
	// these are the player render view parms, which include bobbing
	idVec3					playerViewOrigin;
	idMat3					playerViewAxis;
	
	// the view weapon render entity parms
	idVec3					viewWeaponOrigin;
	idMat3					viewWeaponAxis;
	
	// the muzzle bone's position, used for launching projectiles and trailing smoke
	idVec3					muzzleOrigin;
	idMat3					muzzleAxis;
	
	idVec3					pushVelocity;
	
	// weapon definition
	// we maintain local copies of the projectile and brass dictionaries so they
	// do not have to be copied across the DLL boundary when entities are spawned
	const idDeclEntityDef* 	weaponDef;
	const idDeclEntityDef* 	meleeDef;
	idDict					projectileDict;
	float					meleeDistance;
	idStr					meleeDefName;
	idDict					brassDict;
	int						brassDelay;
	idStr					icon;
	idStr					pdaIcon;
	idStr					displayName;
	idStr					itemDesc;
	
	// view weapon gui light
	renderLight_t			guiLight;
	int						guiLightHandle;
	
	// muzzle flash
	renderLight_t			muzzleFlash;		// positioned on view weapon bone
	int						muzzleFlashHandle;
	
	renderLight_t			worldMuzzleFlash;	// positioned on world weapon bone
	int						worldMuzzleFlashHandle;
	
	float					fraccos;
	float					fraccos2;
	
	idVec3					flashColor;
	int						muzzleFlashEnd;
	int						flashTime;
	bool					lightOn;
	bool					silent_fire;
	bool					allowDrop;
	
	// effects
	bool					hasBloodSplat;
	
	// weapon kick
	int						kick_endtime;
	int						muzzle_kick_time;
	int						muzzle_kick_maxtime;
	idAngles				muzzle_kick_angles;
	idVec3					muzzle_kick_offset;
	
	// ammo management
	ammo_t					ammoType;
	int						ammoRequired;		// amount of ammo to use each shot.  0 means weapon doesn't need ammo.
	int						clipSize;			// 0 means no reload
	idPredictedValue< int >	ammoClip;
	int						lowAmmo;			// if ammo in clip hits this threshold, snd_
	bool					powerAmmo;			// true if the clip reduction is a factor of the power setting when
	// a projectile is launched
	// mp client
	bool					isFiring;
	
	// zoom
	int						zoomFov;			// variable zoom fov per weapon
	
	// joints from models
	jointHandle_t			barrelJointView;
	jointHandle_t			flashJointView;
	jointHandle_t			ejectJointView;
	jointHandle_t			guiLightJointView;
	jointHandle_t			ventLightJointView;
	
	jointHandle_t			flashJointWorld;
	jointHandle_t			barrelJointWorld;
	jointHandle_t			ejectJointWorld;
	
	jointHandle_t			smokeJointView;
	
	idHashTable<WeaponParticle_t>	weaponParticles;
	idHashTable<WeaponLight_t>		weaponLights;
	
	// sound
	const idSoundShader* 	sndHum;
	
	// new style muzzle smokes
	const idDeclParticle* 	weaponSmoke;			// null if it doesn't smoke
	int						weaponSmokeStartTime;	// set to gameLocal.time every weapon fire
	bool					continuousSmoke;		// if smoke is continuous ( chainsaw )
	const idDeclParticle*   strikeSmoke;			// striking something in melee
	int						strikeSmokeStartTime;	// timing
	idVec3					strikePos;				// position of last melee strike
	idMat3					strikeAxis;				// axis of last melee strike
	int						nextStrikeFx;			// used for sound and decal ( may use for strike smoke too )
	
	// nozzle effects
	bool					nozzleFx;			// does this use nozzle effects ( parm5 at rest, parm6 firing )
	// this also assumes a nozzle light atm
	int						nozzleFxFade;		// time it takes to fade between the effects
	int						lastAttack;			// last time an attack occured
	renderLight_t			nozzleGlow;			// nozzle light
	int						nozzleGlowHandle;	// handle for nozzle light
	
	idVec3					nozzleGlowColor;	// color of the nozzle glow
	const idMaterial* 		nozzleGlowShader;	// shader for glow light
	float					nozzleGlowRadius;	// radius of glow light
	
	// weighting for viewmodel angles
	int						weaponAngleOffsetAverages;
	float					weaponAngleOffsetScale;
	float					weaponAngleOffsetMax;
	float					weaponOffsetTime;
	float					weaponOffsetScale;
	
	// flashlight
	void					AlertMonsters();
	
	// Visual presentation
	void					InitWorldModel( const idDeclEntityDef* def );
	void					MuzzleRise( idVec3& origin, idMat3& axis );
	void					UpdateNozzleFx();
	void					UpdateFlashPosition();
	
	// script events
	void					Event_Clear();
	void					Event_GetOwner();
	void					Event_WeaponState( const char* statename, int blendFrames );
	void					Event_SetWeaponStatus( float newStatus );
	void					Event_WeaponReady();
	void					Event_WeaponOutOfAmmo();
	void					Event_WeaponReloading();
	void					Event_WeaponHolstered();
	void					Event_WeaponRising();
	void					Event_WeaponLowering();
	void					Event_UseAmmo( int amount );
	void					Event_AddToClip( int amount );
	void					Event_AmmoInClip();
	void					Event_AmmoAvailable();
	void					Event_TotalAmmoCount();
	void					Event_ClipSize();
	void					Event_PlayAnim( int channel, const char* animname );
	void					Event_PlayCycle( int channel, const char* animname );
	void					Event_AnimDone( int channel, int blendFrames );
	void					Event_SetBlendFrames( int channel, int blendFrames );
	void					Event_GetBlendFrames( int channel );
	void					Event_Next();
	void					Event_SetSkin( const char* skinname );
	void					Event_Flashlight( int enable );
	void					Event_GetLightParm( int parmnum );
	void					Event_SetLightParm( int parmnum, float value );
	void					Event_SetLightParms( float parm0, float parm1, float parm2, float parm3 );
	void					Event_LaunchProjectiles( int num_projectiles, float spread, float fuseOffset, float launchPower, float dmgPower );
	void					Event_CreateProjectile();
	void					Event_EjectBrass();
	void					Event_Melee();
	void					Event_GetWorldModel();
	void					Event_AllowDrop( int allow );
	void					Event_AutoReload();
	void					Event_NetReload();
	void					Event_IsInvisible();
	void					Event_NetEndReload();
	
	idGrabber				grabber;
	int						grabberState;
	
	void					Event_Grabber( int enable );
	void					Event_GrabberHasTarget();
	void					Event_GrabberSetGrabDistance( float dist );
	void					Event_LaunchProjectilesEllipse( int num_projectiles, float spreada, float spreadb, float fuseOffset, float power );
	void					Event_LaunchPowerup( const char* powerup, float duration, int useAmmo );
	
	void					Event_StartWeaponSmoke();
	void					Event_StopWeaponSmoke();
	
	void					Event_StartWeaponParticle( const char* name );
	void					Event_StopWeaponParticle( const char* name );
	
	void					Event_StartWeaponLight( const char* name );
	void					Event_StopWeaponLight( const char* name );
	
	// F3 weapon stats
	
	// DATA
	int32_t nHealth;
	int32_t nValue; // shop value?
	float fWeight;
	int nBaseDmg; // base dmg per shot
	uint8_t nClipSize;
	
	// DNAM
	uint32_t nanimType;
	unint8_t nflags1;
	unint8_t ngripanim;
	unint8_t nreloadanim;
	float fanimMult;
	float fFireRate; 
	float fReach;
	float fCritMult;
	float fminSpread;
	float fSpread;
	int nCritDmg;
	uint8_t nAmmoUse;
	float fSightFOV;
	std::string sProjectile;
	uint8_t nvatsHitChance;
	uint8_t nattackAnimation;
	uint8_t nprojectileCound;
	uint8_t nembeddedweapon;
	float fminrange;
	float fmaxrange;
	uint32_t nonhit;
	uint32_t nflags2;
	float fanimattackmult;
	float foverrideAction;
	float frumbleLeft;
	float frumbleRight;
	float frumbleDuration;
	float frumbleOverrideDamagemult;
	float fAttackShotsPerSecond;
	float freloadtime;
	float fjamtime;
	float faimarc;
	uint32_t nskill; // Values/Actor Values.md in TES5Edit, not implemented yet
	uint32_t npattern;
	float fwavelength;
	float flimbMult;
	uint32_t nresistanceType; // not implemented yet
	float fsightUsage;
	float ffireDelayMin;
	float ffiredelaymax;
	
	
	
	// int nVW; // TODO: wut? // I have no idea where this came from.
	// CRDT
	uint16_t ncritDamage;
	float fCriticalMult;
	float flags;
	std::string sEffect;
	
};

ID_INLINE bool idWeapon::IsLinked()
{
	return isLinked;
}

ID_INLINE bool idWeapon::IsWorldModelReady()
{
	return ( worldModel.GetEntity() != NULL );
}

ID_INLINE idPlayer* idWeapon::GetOwner()
{
	return owner;
}

#endif /* !__GAME_WEAPON_H__ */
