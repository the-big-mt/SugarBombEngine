#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "vaultserver/Database.hpp"
#include "vaultserver/Record.hpp"
#include "vaultserver/Reference.hpp"
#include "vaultserver/Exterior.hpp"
#include "vaultserver/Weapon.hpp"
#include "vaultserver/Race.hpp"
#include "vaultserver/NPC.hpp"
#include "vaultserver/BaseContainer.hpp"
#include "vaultserver/Item.hpp"
#include "vaultserver/Terminal.hpp"
#include "vaultserver/Interior.hpp"
#include "vaultserver/AcReference.hpp"

class ObjectFactory
{
    public:
        static void Initialize();
    private:
        static Database<DB::Record> dbRecords;
	static Database<DB::Reference> dbReferences;
	static Database<DB::Exterior> dbExteriors;
	static Database<DB::Weapon> dbWeapons;
	static Database<DB::Race> dbRaces;
	static Database<DB::NPC> dbNpcs;
	static Database<DB::BaseContainer> dbContainers;
	static Database<DB::Item> dbItems;
	static Database<DB::Terminal> dbTerminals;
	static Database<DB::Interior> dbInteriors;
	static Database<DB::AcReference> dbAcReferences;
};

#endif // OBJECTFACTORY_H
