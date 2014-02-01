#include "ScriptPCH.h"
#include "Pet.h"
 
class Npc_Beastmaster : public CreatureScript {
public:
	Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }
	void CreatePet(Player *player, Creature * m_creature, uint32 entry) {
		if(player->getClass() != CLASS_HUNTER) {
		m_creature->MonsterWhisper("You are not Hunter!", player->GetGUID());
			return ;
		}
		if(player->GetPet()) {
			m_creature->MonsterWhisper("First drop your current Pet!", player->GetGUID());
			player->PlayerTalkClass->SendCloseGossip();
			return ;
		}
		Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
		if(!creatureTarget)
			return ;
		Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
		if(!pet)
			return ;
		creatureTarget->setDeathState(JUST_DIED);
		creatureTarget->RemoveCorpse();
		creatureTarget->SetHealth(0); // just for nice GM-mode view
		pet->SetPower(POWER_HAPPINESS, 1048000);
		pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
		pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
		pet->GetMap()->AddToMap(pet->ToCreature());
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());
		pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
		if(!pet->InitStatsForLevel(player->getLevel()))
			sLog->outError(LOG_FILTER_PETS, "Fail: no init stats for entry %u", entry);
		pet->UpdateAllStats();
		player->SetMinion(pet, true);
		pet->SavePetToDB(PET_SAVE_AS_CURRENT);
		pet->InitTalentForLevel();
		player->PetSpellInitialize();
		player->PlayerTalkClass->SendCloseGossip();
		m_creature->MonsterWhisper("Pet successfully added!", player->GetGUID());
	}

	bool OnGossipHello(Player *player, Creature * m_creature)
	{
		if(player->getClass() != CLASS_HUNTER) {
			m_creature->MonsterWhisper("You are not Hunter!", player->GetGUID());
			return (true);
		}
		player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
		player->ADD_GOSSIP_ITEM(4, "Nevermind",     GOSSIP_SENDER_MAIN, 150);
		player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
		return (true);
	}

	bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
	{
		(void)sender;
		player->PlayerTalkClass->ClearMenus();
		if (action == 100) {
			player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
			player->ADD_GOSSIP_ITEM(4, "Nevermind",     GOSSIP_SENDER_MAIN, 150);
		}
		else if (action == 30) {
			player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
			player->ADD_GOSSIP_ITEM(6, "Hyena.",       GOSSIP_SENDER_MAIN, 4127);
			player->ADD_GOSSIP_ITEM(6, "Ravager.",     GOSSIP_SENDER_MAIN, 17527);
			player->ADD_GOSSIP_ITEM(6, "Bat.",         GOSSIP_SENDER_MAIN, 6182);
			player->ADD_GOSSIP_ITEM(6, "Boar.",        GOSSIP_SENDER_MAIN, 345);
			player->ADD_GOSSIP_ITEM(6, "Spider.",      GOSSIP_SENDER_MAIN, 2349);
			player->ADD_GOSSIP_ITEM(6, "Wolf.",        GOSSIP_SENDER_MAIN, 3637);
			player->ADD_GOSSIP_ITEM(6, "Raptor.",      GOSSIP_SENDER_MAIN, 17348);
			player->ADD_GOSSIP_ITEM(6, "Bear.",        GOSSIP_SENDER_MAIN, 2233);
			player->ADD_GOSSIP_ITEM(6, "Crab.",        GOSSIP_SENDER_MAIN, 3823);
			player->ADD_GOSSIP_ITEM(6, "Adder.",       GOSSIP_SENDER_MAIN, 5048);
			player->ADD_GOSSIP_ITEM(4, "Nevermind",    GOSSIP_SENDER_MAIN, 150);
			player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
		}
		else if (action == 150)
			player->CLOSE_GOSSIP_MENU();
		else
			CreatePet(player, m_creature, action);
		return (true);
	}
};
 
void AddSC_Npc_Beastmaster()
{
	new Npc_Beastmaster();
}
