#include "ScriptPCH.h"
#include "Chat.h"
#include "Language.h"
#include "ReputationMgr.h"

class npc_changer : public CreatureScript
{
public:
    npc_changer() : CreatureScript("npc_changer"){ }
 
    bool OnGossipHello(Player *player, Creature * m_creature)
    {
        player->ADD_GOSSIP_ITEM(4, "Change My Race ", GOSSIP_SENDER_MAIN, 0);
        player->ADD_GOSSIP_ITEM(4, "Change My Faction", GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM(4, "Customize me !", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(4, "Debug my faction (don't use if not bug or you will loose some)", GOSSIP_SENDER_MAIN, 3);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Nevermind..", GOSSIP_SENDER_MAIN, 1000);
        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player *player, Creature * m_creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        (void)m_creature;
        if(!player)
            return true;

        if(uiAction == 0)
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '128' WHERE guid = %u", player->GetGUID());
            ChatHandler(player->GetSession()).PSendSysMessage("You have to relog to change your race!");
        }
        else if (uiAction == 1)
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", player->GetGUID());
            ChatHandler(player->GetSession()).PSendSysMessage("You have to relog to change your faction!");
        }
        else if (uiAction == 2)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_CUSTOMIZE));
            ChatHandler(player->GetSession()).PSendSysMessage("You have to relog to customize !");
            player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
            stmt->setUInt32(1, player->GetGUIDLow());
            CharacterDatabase.Execute(stmt);
        }
        else if (uiAction == 3)
        {
            if (player->GetTeam() == ALLIANCE)
            {
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(72), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(47), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(69), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(930), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(730), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(978), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(54), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(946), 50, false);
            }
            else
            {
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(76), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(68), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(81), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(911), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(729), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(941), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(530), 50, false);
                player->GetReputationMgr().SetOneFactionReputation(sFactionStore.LookupEntry(947), 50, false);
            }
            ChatHandler(player->GetSession()).PSendSysMessage("You have to relog to customize !");
        }
        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }
};
 
void AddSC_npc_changer()
{
    new npc_changer();
}
