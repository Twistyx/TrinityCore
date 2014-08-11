#include "ScriptPCH.h"
#include "Chat.h"
#include "SocialMgr.h"
 
#define FACTION_SPECIFIC 0
 
std::string GetNameLink(Player* player)
{
    std::string name = player->GetName();
    std::string color;
    switch(player->getClass())
    {
        case CLASS_DEATH_KNIGHT: color = "|cffC41F3B"; break;
        case CLASS_DRUID:        color = "|cffFF7D0A"; break;
        case CLASS_HUNTER:       color = "|cffABD473"; break;
        case CLASS_MAGE:         color = "|cff69CCF0"; break;
        case CLASS_PALADIN:      color = "|cffF58CBA"; break;
        case CLASS_PRIEST:       color = "|cffFFFFFF"; break;
        case CLASS_ROGUE:        color = "|cffFFF569"; break;
        case CLASS_SHAMAN:       color = "|cff0070DE"; break;
        case CLASS_WARLOCK:      color = "|cff9482C9"; break;
        case CLASS_WARRIOR:      color = "|cffC79C6E"; break;
    }
    return "|Hplayer:" + name + "|h|cffFFFFFF[" + color + name + "|cffFFFFFF]|h|r";
}
 
class cs_world_chat : public CommandScript
{
    public:
        cs_world_chat() : CommandScript("cs_world_chat"){}
 
    ChatCommand * GetCommands() const
    {
        static ChatCommand WorldChatCommandTable[] =
        {
            {"chat",  rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, &HandleWorldChatCommand, "", NULL},
            {"c",     rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, &HandleWorldChatCommand, "", NULL},
            {"world", rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, &HandleWorldChatCommand, "", NULL},
            {"w",     rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, &HandleWorldChatCommand, "", NULL},
            {NULL,    0,                                  false,                   NULL, "", NULL}
        };
        return WorldChatCommandTable;
    }
 
    static bool HandleWorldChatCommand(ChatHandler * handler, const char * args)
    {
        std::string temp = args;
        Player * player;
        bool checkMe = false;
 
        if (!args || temp.find_first_not_of(' ') == std::string::npos)
            return false;

        std::string msg = "";
        if (!handler || !handler->GetSession())
            msg += "|cfffa9900[Console] ";
        else
        {
            player = handler->GetSession()->GetPlayer();
            if (!player->CanSpeak())
                return false;

            switch(player->GetSession()->GetSecurity())
            {
                // Player
                case SEC_PLAYER:
                    if (player->GetOTeam() == ALLIANCE)
                        msg += "|cff0000ff[Alliance] ";
                    else
                        msg += "|cffff0000[Horde] ";
                    break;
                case SEC_MODERATOR:     msg += "|cffff8a00[Moderator] "; break;
                case SEC_GAMEMASTER:    msg += "|cff00ffff[GM] ";        break;
                case SEC_ADMINISTRATOR: msg += "|cfffa9900[Admin] ";     break;
                default:                msg += "|cfffa9900[Console] ";   break;
            }
            checkMe = true;
            msg += GetNameLink(player);
        }
        msg += " |cfffaeb00" + temp;
        SessionMap sessions = sWorld->GetAllSessions();
        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            Player* plr = itr->second->GetPlayer();
            if (!plr)
                continue;

            //if (plr->GetTeam() != player->GetTeam() && FACTION_SPECIFIC)
            //    continue;

            if (checkMe && plr->GetSocial()->HasIgnore(player->GetGUIDLow()))
                continue;

            sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), plr);
        }
        return true;
    }
};
 
void AddSC_cs_world_chat()
{
    new cs_world_chat();
}
