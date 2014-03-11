#include "ScriptPCH.h"
#include "Chat.h"
#include "Language.h"

class npc_changer : public CreatureScript
{
public:
    npc_changer() : CreatureScript("npc_changer"){ }
 
    bool OnGossipHello(Player *player, Creature * m_creature)
    {
        player->ADD_GOSSIP_ITEM(4, "Change My Race ", GOSSIP_SENDER_MAIN, 0);
        player->ADD_GOSSIP_ITEM(4, "Change My Faction", GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM(4, "Customize me !", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(4, "Reset my talents, I have no talents anyway !", GOSSIP_SENDER_MAIN, 3);
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
            player->GetSession()->SendNotification("You have to relog, to change your race!");
        }
        else if (uiAction == 1)
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", player->GetGUID());
            player->GetSession()->SendNotification("You have to relog, to change your faction!");
        }
        else if (uiAction == 2)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_CUSTOMIZE));
            player->GetSession()->SendNotification("Yey Customize !!");
            player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
            stmt->setUInt32(1, player->GetGUIDLow());
            CharacterDatabase.Execute(stmt);
        }
        else if (uiAction == 3)
        {
            player->resetTalents(true);
            player->SendTalentsInfoData(false);
            ChatHandler(player->GetSession()).SendSysMessage(LANG_RESET_TALENTS);
            return true;
        }
        else
            player->PlayerTalkClass->SendCloseGossip();
        return true;
    }
};
 
void AddSC_npc_changer()
{
    new npc_changer();
}
