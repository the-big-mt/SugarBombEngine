/// @file

#pragma once

// NOTE: Many inventory items (including Ingestibles, which actually appear in the Game Effects category) 
// have an Equip Type that dictates how an item is equipped (i.e. how the armor is worn), and which hotkey 
// image is to be used for the item (Mentats and Jet will use the same chems hotkey icon). Here are the different equip types:
enum class EquipType : int
{
#ifdef GAME_F3
	BigGuns = 0, // (F3-specific)
#endif
	EnergyWeapons,
#ifdef GAME_F3
	SmallGuns, // (F3-specific)
#endif
#ifdef GAME_FNV
	Guns,
#endif
	MeleeWeapons,
	UnarmedWeapons,
	ThrowWeapons,
	Mine,
	BodyWear,
	HeadWear,
	HandWear,
	Chems,
	Stimpack,
	Food,
	Alcohol
};

class Item
{
private:
	idStr msInventoryImage; ///< Filename for the icon (.dds) shown on the inventory screen
	idStr msMessageIcon; ///< Filename for the icon (.dds) that is shown in the HUD alongside a message when an item is added to your inventory
	idStr msModel; ///< Filename for the 3D art (.NIF) used to display the item in the world
	
	// TODO: mDestructionData; // Button that can set up the item's destructible states
	
	EquipType meEquipType; ///< Determines how the item is equipped, how the hotkey will be displayed, and what major skill is necessary to use this item
};

class Ammo : public Item
{
};

class Armor : public Item
{
};

class ArmorAddon : public Item
{
};

class Book : public Item
{
};

// NOTE: Keys are all displayed on the Keyring in the inventory. A key has no weight and cannot be bought or sold. 
// The value field is used to weight the chance of getting caught when pickpocketing the key. The higher the value, the higher the chance of being detected
class Key : public Item
{
};

class LeveledItem : public Item
{
};

// NOTE: Miscellaneous items are objects than can be placed in the player's inventory, 
// but do not otherwise have a function. They react to Havok physics, have a value and a weight, but are otherwise inert
class MiscItem : public Item
{
};

class Note : public Item
{
};

class Weapon : public Item
{
private:
	float mfWeight; ///< The weight of the weapon
	
	int mnSightFOV;
};