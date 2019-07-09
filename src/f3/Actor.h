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

#ifndef __GAME_ACTOR_H__
#define __GAME_ACTOR_H__

#include <memory>

/*
#include "../d3xp/AFEntity.h"
#include "../d3xp/IK.h"
#include "../d3xp/anim/Anim.h"
#include "../d3xp/gamesys/Class.h"
#include "../d3xp/gamesys/Event.h"
#include "../idlib/Heap.h"
#include "../idlib/Str.h"
#include "../idlib/containers/LinkList.h"
#include "../idlib/containers/List.h"
#include "../idlib/containers/StrList.h"
#include "../idlib/math/Angles.h"
#include "../idlib/math/Matrix.h"
#include "../idlib/math/Vector.h"
#include "../renderer/Model.h"
#include "../renderer/RenderWorld.h"
*/

//namespace BFG
//{

namespace MWRender
{
    class Animation;
};

namespace MWWorld
{
    class Ptr;
};

/*
===============================================================================

	idActor

===============================================================================
*/

extern const idEventDef AI_EnableEyeFocus;
extern const idEventDef AI_DisableEyeFocus;
extern const idEventDef EV_Footstep;
extern const idEventDef EV_FootstepLeft;
extern const idEventDef EV_FootstepRight;
extern const idEventDef EV_EnableWalkIK;
extern const idEventDef EV_DisableWalkIK;
extern const idEventDef EV_EnableLegIK;
extern const idEventDef EV_DisableLegIK;
extern const idEventDef AI_SetAnimPrefix;
extern const idEventDef AI_PlayAnim;
extern const idEventDef AI_PlayCycle;
extern const idEventDef AI_AnimDone;
extern const idEventDef AI_SetBlendFrames;
extern const idEventDef AI_GetBlendFrames;

extern const idEventDef AI_SetState;

//class function_t;
//class idAAS;
//class idActor;
//class idClipModel;
class idDeclParticle;
//class idEntity;
//class idRestoreGame;
//class idSaveGame;
//class idThread;

class idAnimState
{
public:
	bool					idleAnim;
	idStr					state;
	int						animBlendFrames;
	int						lastAnimBlendFrames;		// allows override anims to blend based on the last transition time
	
public:
	idAnimState();
	~idAnimState();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	void					Init( idActor* owner, idAnimator* _animator, int animchannel );
	void					Shutdown();
	void					SetState( const char* name, int blendFrames );
	void					StopAnim( int frames );
	void					PlayAnim( int anim );
	void					CycleAnim( int anim );
	void					BecomeIdle();
	bool					UpdateState();
	bool					Disabled() const;
	void					Enable( int blendFrames );
	void					Disable();
	bool					AnimDone( int blendFrames ) const;
	bool					IsIdle() const;
	animFlags_t				GetAnimFlags() const;
	
private:
	idActor* 				self;
	idAnimator* 			animator;
	idThread* 				thread;
	int						channel;
	bool					disabled;
};

class idAttachInfo
{
public:
	idEntityPtr<idEntity>	ent;
	int						channel;
};

typedef struct
{
	jointModTransform_t		mod;
	jointHandle_t			from;
	jointHandle_t			to;
} copyJoints_t;

//namespace MWMechanics
//{

class CharacterController;

/// @brief Holds temporary state for an actor that will be discarded when the actor leaves the scene.
class idActor : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idActor );
	
	int						team;
	int						rank;				// monsters don't fight back if the attacker's rank is higher
	idMat3					viewAxis;			// view axis of the actor
	
	idLinkList<idActor>		enemyNode;			// node linked into an entity's enemy list for quick lookups of who is attacking him
	idLinkList<idActor>		enemyList;			// list of characters that have targeted the player as their enemy
	
public:
	idActor();
	idActor(const MWWorld::Ptr& ptr, MWRender::Animation* animation); // NOTE: OpenMW
	virtual					~idActor();
	
	void					Spawn();
	virtual void			Restart();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	virtual void			Hide();
	virtual void			Show();
	virtual int				GetDefaultSurfaceType() const;
	virtual void			ProjectOverlay( const idVec3& origin, const idVec3& dir, float size, const char* material );
	
	virtual bool			LoadAF();
	void					SetupBody();
	
	void					CheckBlink();
	
	virtual bool			GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );
	virtual bool			GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );
	
	// script state management
	void					ShutdownThreads();
	virtual bool			ShouldConstructScriptObjectAtSpawn() const;
	virtual idThread* 		ConstructScriptObject();
	void					UpdateScript();
	const function_t*		GetScriptFunction( const char* funcname );
	void					SetState( const function_t* newState );
	void					SetState( const char* statename );
	
	// vision testing
	void					SetEyeHeight( float height );
	float					EyeHeight() const;
	idVec3					EyeOffset() const;
	idVec3					GetEyePosition() const;
	virtual void			GetViewPos( idVec3& origin, idMat3& axis ) const;
	void					SetFOV( float fov );
	bool					CheckFOV( const idVec3& pos ) const;
	bool					CanSee( idEntity* ent, bool useFOV ) const;
	bool					PointVisible( const idVec3& point ) const;
	virtual void			GetAIAimTargets( const idVec3& lastSightPos, idVec3& headPos, idVec3& chestPos );
	
	// damage
	void					SetupDamageGroups();
	virtual	void			Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );
	int						GetDamageForLocation( int damage, int location );
	const char* 			GetDamageGroup( int location );
	void					ClearPain();
	virtual bool			Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
	
	// model/combat model/ragdoll
	void					SetCombatModel();
	idClipModel* 			GetCombatModel() const;
	virtual void			LinkCombat();
	virtual void			UnlinkCombat();
	bool					StartRagdoll();
	void					StopRagdoll();
	virtual bool			UpdateAnimationControllers();
	
	// delta view angles to allow movers to rotate the view of the actor
	const idAngles& 		GetDeltaViewAngles() const;
	void					SetDeltaViewAngles( const idAngles& delta );
	
	bool					HasEnemies() const;
	idActor* 				ClosestEnemyToPoint( const idVec3& pos );
	idActor* 				EnemyWithMostHealth();
	
	virtual bool			OnLadder() const;
	
	virtual void			GetAASLocation( idAAS* aas, idVec3& pos, int& areaNum ) const;
	
	void					Attach( idEntity* ent );
	
	virtual void			Teleport( const idVec3& origin, const idAngles& angles, idEntity* destination );
	
	virtual	renderView_t* 	GetRenderView();
	
	// animation state control
	int						GetAnim( int channel, const char* name );
	void					UpdateAnimState();
	void					SetAnimState( int channel, const char* name, int blendFrames );
	const char* 			GetAnimState( int channel ) const;
	bool					InAnimState( int channel, const char* name ) const;
	const char* 			WaitState() const;
	void					SetWaitState( const char* _waitstate );
	bool					AnimDone( int channel, int blendFrames ) const;
	virtual void			SpawnGibs( const idVec3& dir, const char* damageDefName );
	
	idEntity*				GetHeadEntity()
	{
		return head.GetEntity();
	};
	
	// NOTE: OpenMW below
	
	/// Notify this actor of its new base object Ptr, use when the object changed cells
	void updatePtr(const MWWorld::Ptr& newPtr);

	CharacterController* getCharacterController();
	void restoreDynamicStats(bool sleep);

	void updateHeadTracking(const MWWorld::Ptr& targetActor, MWWorld::Ptr& headTrackTarget, float& sqrHeadTrackDistance);
	
	void addBoundItem (const std::string& itemId);
	void removeBoundItem (const std::string& itemId);
	
	bool isAttackPreparing(const MWWorld::Ptr& ptr);
	bool isRunning(const MWWorld::Ptr& ptr);
	bool isSneaking(const MWWorld::Ptr& ptr);
	
	
	bool isCastingSpell(const MWWorld::Ptr& ptr) const;
	bool isReadyToBlock(const MWWorld::Ptr& ptr) const;
	bool isAttackingOrSpell(const MWWorld::Ptr& ptr) const;
private:
	void calculateDynamicStats ();
	
	void adjustMagicEffects (const MWWorld::Ptr& creature);

	void calculateCreatureStatModifiers (const MWWorld::Ptr& ptr, float duration);
	void calculateNpcStatModifiers (const MWWorld::Ptr& ptr, float duration);

	void calculateRestoration (const MWWorld::Ptr& ptr, float duration);

	void updateDrowning (const MWWorld::Ptr& ptr, float duration, bool isKnockedOut, bool isPlayer);

	void updateEquippedLight (const MWWorld::Ptr& ptr, float duration, bool mayEquip);

	void updateCrimePursuit (const MWWorld::Ptr& ptr, float duration);
protected:
	friend class			idAnimState;
	
	float					fovDot;				// cos( fovDegrees )
	idVec3					eyeOffset;			// offset of eye relative to physics origin
	idVec3					modelOffset;		// offset of visual model relative to the physics origin
	
	idAngles				deltaViewAngles;	// delta angles relative to view input angles
	
	int						pain_debounce_time;	// next time the actor can show pain
	int						pain_delay;			// time between playing pain sound
	int						pain_threshold;		// how much damage monster can take at any one time before playing pain animation
	
	idStrList				damageGroups;		// body damage groups
	idList<float, TAG_ACTOR>			damageScale;		// damage scale per damage gruop
	
	bool						use_combat_bbox;	// whether to use the bounding box for combat collision
	idEntityPtr<idAFAttachment>	head;
	idList<copyJoints_t, TAG_ACTOR>		copyJoints;			// copied from the body animation to the head model
	
	// state variables
	const function_t*		state;
	const function_t*		idealState;
	
	// joint handles
	jointHandle_t			leftEyeJoint;
	jointHandle_t			rightEyeJoint;
	jointHandle_t			soundJoint;
	
	idIK_Walk				walkIK;
	
	idStr					animPrefix;
	idStr					painAnim;
	
	// blinking
	int						blink_anim;
	int						blink_time;
	int						blink_min;
	int						blink_max;
	
	// script variables
	idThread* 				scriptThread;
	idStr					waitState;
	idAnimState				headAnim;
	idAnimState				torsoAnim;
	idAnimState				legsAnim;
	
	bool					allowPain;
	bool					allowEyeFocus;
	bool					finalBoss;
	
	int						painTime;
	bool					damageNotByFists;
	
	idList<idAttachInfo, TAG_ACTOR>	attachments;
	
	int						damageCap;
	
	virtual void			Gib( const idVec3& dir, const char* damageDefName );
	
	// removes attachments with "remove" set for when character dies
	void					RemoveAttachments();
	
	// copies animation from body to head joints
	void					CopyJointsFromBodyToHead();
	
private:
	void					SyncAnimChannels( int channel, int syncToChannel, int blendFrames );
	void					FinishSetup();
	void					SetupHead();
	void					PlayFootStepSound();
	
	void					Event_EnableEyeFocus();
	void					Event_DisableEyeFocus();
	void					Event_Footstep();
	void					Event_EnableWalkIK();
	void					Event_DisableWalkIK();
	void					Event_EnableLegIK( int num );
	void					Event_DisableLegIK( int num );
	void					Event_SetAnimPrefix( const char* name );
	void					Event_LookAtEntity( idEntity* ent, float duration );
	void					Event_PreventPain( float duration );
	void					Event_DisablePain();
	void					Event_EnablePain();
	void					Event_GetPainAnim();
	void					Event_StopAnim( int channel, int frames );
	void					Event_PlayAnim( int channel, const char* name );
	void					Event_PlayCycle( int channel, const char* name );
	void					Event_IdleAnim( int channel, const char* name );
	void					Event_SetSyncedAnimWeight( int channel, int anim, float weight );
	void					Event_OverrideAnim( int channel );
	void					Event_EnableAnim( int channel, int blendFrames );
	void					Event_SetBlendFrames( int channel, int blendFrames );
	void					Event_GetBlendFrames( int channel );
	void					Event_AnimState( int channel, const char* name, int blendFrames );
	void					Event_GetAnimState( int channel );
	void					Event_InAnimState( int channel, const char* name );
	void					Event_FinishAction( const char* name );
	void					Event_AnimDone( int channel, int blendFrames );
	void					Event_HasAnim( int channel, const char* name );
	void					Event_CheckAnim( int channel, const char* animname );
	void					Event_ChooseAnim( int channel, const char* animname );
	void					Event_AnimLength( int channel, const char* animname );
	void					Event_AnimDistance( int channel, const char* animname );
	void					Event_HasEnemies();
	void					Event_NextEnemy( idEntity* ent );
	void					Event_ClosestEnemyToPoint( const idVec3& pos );
	void					Event_StopSound( int channel, int netsync );
	void					Event_SetNextState( const char* name );
	void					Event_SetState( const char* name );
	void					Event_GetState();
	void					Event_GetHead();
	void					Event_SetDamageGroupScale( const char* groupName, float scale );
	void					Event_SetDamageGroupScaleAll( float scale );
	void					Event_GetDamageGroupScale( const char* groupName );
	void					Event_SetDamageCap( float _damageCap );
	void					Event_SetWaitState( const char* waitState );
	void					Event_GetWaitState();
	
	std::unique_ptr<CharacterController> mCharacterController;
	// vaultmp
public:
	// Actor values
	enum class Values : unsigned char
	{
		Aggression = 0x00,
		Confidence = 0x01,
		Energy = 0x02,
		Responsibility = 0x03,
		Mood = 0x04,
		
		Strength = 0x05,
		Perception = 0x06,
		Endurance = 0x07,
		Charisma = 0x08,
		Intelligence = 0x09,
		Agility = 0x0A,
		Luck = 0x0B,
		
		ActionPoints = 0x0C,
		CarryWeight = 0x0D,
		CritChance = 0x0E,
		HealRate = 0x0F,
		Health = 0x10,
		MeleeDamage = 0x11,
		DamageResistance = 0x12,
		PoisonResistance = 0x13,
		RadResistance = 0x14,
		SpeedMultiplier = 0x15,
		Fatigue = 0x16,
		Karma = 0x17,
		XP = 0x18,
		
		Head = 0x19, // PerceptionCondition
		Torso = 0x1A, // EnduranceCondition
		LeftArm = 0x1B, // LeftAttackCondition
		RightArm = 0x1C, // RightAttackCondition
		LeftLeg = 0x1D, // LeftMobilityCondition
		RightLeg = 0x1E, // RightMobilityCondition
		Brain = 0x1F, // BrainCondition
		
		Barter = 0x20,
		BigGuns = 0x21, // F3 only
		EnergyWeapons = 0x22,
		Explosives = 0x23,
		Lockpick = 0x24,
		Medicine = 0x25,
		MeleeWeapons = 0x26,
		Repair = 0x27,
		Science = 0x28,
		SmallGuns = 0x29,
		//Guns = 0x29, // Alias for SmallGuns (FNV)
		Sneak = 0x2A,
		Speech = 0x2B,
		Survival = 0x2C, // Throwing in F3 (unused)
		Unarmed = 0x2D,
		
		InventoryWeight = 0x2E,
		Paralysis = 0x2F,
		Invisibility = 0x30,
		Chameleon = 0x31,
		NightEye = 0x32,
		DetectLifeRange = 0x33, // Turbo
		FireResistance = 0x34,
		WaterBreathing = 0x35,
		RadLevel = 0x36, // RadiationRads
		BloodyMess = 0x37,
		UnarmedDamage = 0x38,
		Assistance = 0x39,

		ElectricResistance = 0x3A,
		FrostResistance = 0x3B,
		EnergyResistance = 0x3C,
		EMPResistance = 0x3D,
		
		Var1Medical = 0x3E,
		Variable02 = 0x3F,
		Variable03 = 0x40,
		Variable04 = 0x41,
		Variable05 = 0x42,
		Variable06 = 0x43,
		Variable07 = 0x44,
		Variable08 = 0x45,
		Variable09 = 0x46,
		Variable10 = 0x47,
		
		IgnoreCrippledLimbs = 0x48, // Ignore Negative Effects
		
		// FNV additional values
		
		// hardcore mode values
		Dehydration = 0x49,
		Hunger = 0x4A,
		SleepDeprivation = 0x4B,
		
		DamageThreshold = 0x4C
	};
	};
	
	/**
	 * \brief Limb values
	 */
	enum class Limbs //: unsigned short
	{
		Limb_None = -1,
		Limb_Torso = 0,
		Limb_Head1 = 1,
		Limb_Head2 = 2,
		Limb_LeftArm1 = 3,
		Limb_LeftArm2 = 4,
		Limb_RightArm1 = 5,
		Limb_RightArm2 = 6,
		Limb_LeftLeg1 = 7,
		Limb_LeftLeg2 = 8,
		Limb_LeftLeg3 = 9,
		Limb_RightLeg1 = 10,
		Limb_RightLeg2 = 11,
		Limb_RightLeg3 = 12,
		Limb_Brain = 13,
		Limb_Weapon = 14,
	};
	
	/**
	 * \brief Cause of death values
	 */
	enum class Death/*Type/Cause*/ //: char
	{
		None = -1,
		
		Explosion = 0,
		
		Gun = 2,
		BluntWeapon,
		HandToHand,
		ObjectImpact,
		Poison,
		Radiation,
	};

	static const std::map<unsigned char, std::pair<const float, const float>> default_values;
private:
	std::unordered_map<unsigned char, Value<float>> Values;
	std::unordered_map<unsigned char, Value<float>> BaseValues;
	
	Value<unsigned int> Race;
	Value<int> Age;
	
	Value<unsigned int> anim_Idle;
	Value<unsigned char> anim_Moving;
	Value<unsigned char> anim_Weapon;
	
	Value<unsigned char> state_MovingXY;
	
	Value<bool> state_Female;
	Value<bool> state_Alerted;
	Value<bool> state_Sneaking;
	Value<bool> state_Dead;
	
	Value<unsigned short> death_Limbs;
	Value<char> death_Cause;
};

//}; // namespace MWMechanics

//} // namespace BFG

#endif /* !__GAME_ACTOR_H__ */