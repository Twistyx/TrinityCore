#include "ScriptPCH.h"
#include "Pet.h"
 
class Npc_Beastmaster : public CreatureScript
{
public:
        Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }
 
        void CreatePet(Player *player, Creature * m_creature, uint32 entry) {
 
                if(player->getClass() != CLASS_HUNTER) {
                m_creature->MonsterWhisper("You are not Hunter!", player->GetGUID());
                        return;
                }
 
                if(player->GetPet()) {
                        m_creature->MonsterWhisper("First drop your current Pet!", player->GetGUID());
                        player->PlayerTalkClass->SendCloseGossip();
                        return;
                }
 
                Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
                if(!creatureTarget) return;
 
                Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
                if(!pet) return;
 
                creatureTarget->setDeathState(JUST_DIED);
                creatureTarget->RemoveCorpse();
                creatureTarget->SetHealth(0);                       // just for nice GM-mode view
 
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
 
                if(player->getClass() != CLASS_HUNTER)
                {
                        m_creature->MonsterWhisper("You are not Hunter!", player->GetGUID());
                        return true;
                }
                player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
                if (player->CanTameExoticPets())
                {
                        player->ADD_GOSSIP_ITEM(7, "Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
                }
            player->ADD_GOSSIP_ITEM(2, "Visit the Stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                        player->ADD_GOSSIP_ITEM(1, "Foods for my Pet", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
                        player->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 150);
                player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                return true;
        }
 
        bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
        {
                player->PlayerTalkClass->ClearMenus();
                switch (action)
                {
 
                case 100:
                        player->ADD_GOSSIP_ITEM(3, "Get a New Pet", GOSSIP_SENDER_MAIN, 30);
                        if (player->CanTameExoticPets())
                        {
                                player->ADD_GOSSIP_ITEM(7, "Get a New Exotic Pet.", GOSSIP_SENDER_MAIN, 50);
                        }
                        player->ADD_GOSSIP_ITEM(2, "Visit the Stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                        player->ADD_GOSSIP_ITEM(1, "Foods for my Pet", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
                        player->ADD_GOSSIP_ITEM(4, "Nevermind", GOSSIP_SENDER_MAIN, 150);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;
 
                case 150:
                        player->CLOSE_GOSSIP_MENU();
                        break;
 
                case 30:
                        player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
                        player->ADD_GOSSIP_ITEM(6, "Hyena.", GOSSIP_SENDER_MAIN, 1);
                        player->ADD_GOSSIP_ITEM(6, "Ravager.", GOSSIP_SENDER_MAIN, 2);
                        player->ADD_GOSSIP_ITEM(6, "Bat.", GOSSIP_SENDER_MAIN, 3);
                        player->ADD_GOSSIP_ITEM(6, "Boar.", GOSSIP_SENDER_MAIN, 4);
                        player->ADD_GOSSIP_ITEM(6, "Spider.", GOSSIP_SENDER_MAIN, 5);
                        player->ADD_GOSSIP_ITEM(6, "Wolf.", GOSSIP_SENDER_MAIN, 6);
                        player->ADD_GOSSIP_ITEM(6, "Raptor.", GOSSIP_SENDER_MAIN, 7);
                        player->ADD_GOSSIP_ITEM(6, "Bear.", GOSSIP_SENDER_MAIN, 8);
                        player->ADD_GOSSIP_ITEM(6, "Crab.", GOSSIP_SENDER_MAIN, 9);
                        player->ADD_GOSSIP_ITEM(6, "Adder.", GOSSIP_SENDER_MAIN, 10);
                        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
                        break;

 
                case GOSSIP_OPTION_STABLEPET:
                        player->GetSession()->SendStablePet(m_creature->GetGUID());
                        break;
                case GOSSIP_OPTION_VENDOR:
                        player->GetSession()->SendListInventory(m_creature->GetGUID());
                        break;
                case 1: //Hyena
                        CreatePet(player, m_creature, 4127);
                        break;
                case 2: //Ravager
                        CreatePet(player, m_creature, 17527);
                        break;
                case 3: //Bat
                        CreatePet(player, m_creature, 6182);
                        break;
                case 4: //Boar
                        CreatePet(player, m_creature, 345);
                        break;
                case 5: //Spider
                        CreatePet(player, m_creature, 2349);
                        break;
                case 6: //Raptor
                        CreatePet(player, m_creature, 3637);
                        break;
                case 7: //Bear
                        CreatePet(player, m_creature, 17348);
                        break;
                case 8: //Crab
                        CreatePet(player, m_creature, 2233);
                        break;     
                case 9: //Wolf
                        CreatePet(player, m_creature, 3823);
                        break;   
                case 9: //Adder
                        CreatePet(player, m_creature, 5048);
                        break;                   
                }
                return true;
        }
};
 
void AddSC_Npc_Beastmaster()
{
        new Npc_Beastmaster();
}
