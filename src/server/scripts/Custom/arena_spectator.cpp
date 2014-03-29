/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: Arena Spectator
%Complete: 100
Comment: Script allow spectate arena games
Category: Custom Script
EndScriptData */

#include "ScriptPCH.h"
#include "Chat.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "WorldSession.h"
#include "Player.h"
#include "ArenaTeam.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "CreatureTextMgr.h"
#include "Config.h"

enum NpcSpectatorAtions
{
    GOSSIP_ACTION_SHOW_MENU       = 100,
    // GOSSIP_ACTION + ArenaTeamType
    GOSSIP_ACTION_SHOW_GAMES_PAGE = 500,
    GOSSIP_ACTION_SHOW_GAMES      = 1000,
    GOSSIP_ACTION_TOP_10          = 2000,
    // GOSSIP_ACTION + playerGuid
    GOSSIP_ACTION_SPECTATE        = 3000
};

enum ArenaTeamSlots
{
    ARENA_SLOT_2v2      = 0,
    ARENA_SLOT_3v3      = 1,
    ARENA_SLOT_5v5      = 2
};

#define MAX_PER_PAGE 15

bool m_usingGossip;

class arena_spectator_commands : public CommandScript
{
public:
    arena_spectator_commands() : CommandScript("arena_spectator_commands") { }

    static bool HandleSpectateCommand(ChatHandler* handler, char const* args)
    {
    	if (!handler)
    		return false;
        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
    		return false;

        if (player->IsInCombat())
        {
            handler->PSendSysMessage("You are in Combat.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target;
        uint64 target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        if (target == player || target_guid == player->GetGUID())
        {
            handler->PSendSysMessage("Can't Spectate yourself.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!target)
        {
            handler->PSendSysMessage("Target not found.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Map* cMap = target->GetMap();
        if (!cMap->IsBattleArena())
        {
            handler->PSendSysMessage("Player not in an arena.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetPet()) // TODO : Auto remove pet instead
        {
            handler->PSendSysMessage("You must recall your pet.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((player->GetMap()->IsBattlegroundOrArena() && !player->isSpectator()) || player->GetMap()->IsBattleground())
        {
            handler->PSendSysMessage("Not here.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // all's well, set bg id
        // when porting out from the bg, it will be reset to 0
        player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
        // remember current position as entry point for return at bg end teleportation
        if (!player->GetMap()->IsBattlegroundOrArena())
            player->SetBattlegroundEntryPoint();

        if (target->isSpectator())
        {
            handler->PSendSysMessage("Your target is spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (player->IsInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->CleanupAfterTaxiFlight();
        }
        else // save only in non-flight case
            player->SaveRecallPosition();

        // search for two teams
        Battleground *bGround = target->GetBattleground();
        if (bGround->isRated())
        {
            const uint8 slot = (bGround->GetArenaType() > 3) ? 2 : bGround->GetArenaType() - 2;
            uint32 firstTeamID = target->GetArenaTeamId(slot);
            uint32 secondTeamID = 0;
            Player *firstTeamMember  = target;
            Player *secondTeamMember = NULL;
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
			{
                if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (tmpPlayer->isSpectator())
                        continue;

                    uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                    if (tmpID != firstTeamID && tmpID > 0)
                    {
                        secondTeamID = tmpID;
                       secondTeamMember = tmpPlayer;
                        break;
                    }
                }
			}
            if (firstTeamID > 0 && secondTeamID > 0 && secondTeamMember)
            {
                ArenaTeam *firstTeam  = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
                ArenaTeam *secondTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
                if (firstTeam && secondTeam)
                {
                    handler->PSendSysMessage("You entered to rated arena.");
                    handler->PSendSysMessage("Teams:");
                    handler->PSendSysMessage("%s - %s", firstTeam->GetName().c_str(), secondTeam->GetName().c_str());
                    handler->PSendSysMessage("%u(%u) - %u(%u)", firstTeam->GetRating(), firstTeam->GetAverageMMR(firstTeamMember->GetGroup()),
                                                                secondTeam->GetRating(), secondTeam->GetAverageMMR(secondTeamMember->GetGroup()));
                }
            }
        }

        // to point to see at target with same orientation
        float x, y, z;
        target->GetContactPoint(player, x, y, z);

        player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
        player->SetPhaseMask(target->GetPhaseMask(), true);
        player->SetSpectate(true);
        target->GetBattleground()->AddSpectator(player->GetGUID());

        return true;
    }

    static bool HandleSpectateCancelCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player =  handler->GetSession()->GetPlayer();

        if (!player->isSpectator())
        {
            handler->PSendSysMessage("You are not a spectator.");
           handler->SetSentErrorMessage(true);
            return false;
        }

        player->GetBattleground()->RemoveSpectator(player->GetGUID());
        player->CancelSpectate();
        player->TeleportToBGEntryPoint();

        return true;
    }

    static bool HandleSpectateFromCommand(ChatHandler* handler, const char *args)
    {
        Player* target;
        uint64 target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        Player* player = handler->GetSession()->GetPlayer();

        if (!target)
        {
            handler->PSendSysMessage("Can't find the player.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!player->isSpectator())
        {
            handler->PSendSysMessage("You are not spectator, spectate someone first.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->isSpectator() && target != player)
        {
            handler->PSendSysMessage("Can't do that. Your target is spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetMap() != target->GetMap())
        {
            handler->PSendSysMessage("Can't do that. Different arenas?");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check for arena preparation
        // if exists than battle didn`t begin
        if (target->HasAura(32728) || target->HasAura(32727))
        {
            handler->PSendSysMessage("Can't do that. Arena didn't started.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target == player && player->getSpectateFrom())
        	player->SetViewpoint(player->getSpectateFrom(), false);
        else
        	player->SetViewpoint(target, true);
        return true;
    }

    static bool HandleSpectateResetCommand(ChatHandler* handler, const char * /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
        {
            handler->PSendSysMessage("Cant find player.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!player->isSpectator())
        {
            handler->PSendSysMessage("You are not spectator!");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Battleground *bGround = player->GetBattleground();
        if (!bGround)
            return false;

        if (bGround->GetStatus() != STATUS_IN_PROGRESS)
            return true;

        for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
		{
            if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
            {
                if (tmpPlayer->isSpectator())
                    continue;

                uint32 tmpID = bGround->GetPlayerTeam(tmpPlayer->GetGUID());

                // generate addon message
                std::string pName = tmpPlayer->GetName();
                std::string tName = "";

                if (Player *target = tmpPlayer->GetSelectedPlayer())
                    tName = target->GetName();

                SpectatorAddonMsg msg;
                msg.SetPlayer(pName);
                if (tName != "")
                    msg.SetTarget(tName);
                msg.SetStatus(tmpPlayer->IsAlive());
                msg.SetClass(tmpPlayer->getClass());
                msg.SetCurrentHP(tmpPlayer->GetHealth());
                msg.SetMaxHP(tmpPlayer->GetMaxHealth());
                const Powers powerType = tmpPlayer->getPowerType();
                msg.SetMaxPower(tmpPlayer->GetMaxPower(powerType));
                msg.SetCurrentPower(tmpPlayer->GetPower(powerType));
                msg.SetPowerType(powerType);
                msg.SetTeam(tmpID);
                msg.SendPacket(player->GetGUID());
            }
		}
        return true;
    }

    ChatCommand* GetCommands() const
    {
        static ChatCommand spectateCommandTable[] =
        {
            { "player", rbac::RBAC_PERM_COMMAND_SPECATE_VIEW,   true,  &HandleSpectateCommand,       "", NULL },
            { "view",   rbac::RBAC_PERM_COMMAND_SPECATE_RESET,  true,  &HandleSpectateFromCommand,   "", NULL },
            { "reset",  rbac::RBAC_PERM_COMMAND_SPECATE_LEAVE,  true,  &HandleSpectateResetCommand,  "", NULL },
            { "leave",  rbac::RBAC_PERM_COMMAND_SPECATE_PLAYER, true,  &HandleSpectateCancelCommand, "", NULL },
            { NULL,     0,                                      false, NULL,                         "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "spectate", rbac::RBAC_PERM_COMMAND_SPECATE, false,  NULL, "", spectateCommandTable },
            { NULL,       0,                               false,  NULL, "", NULL }
        };
        return commandTable;
    }
};

class npc_arena_spectator : public CreatureScript
{
public:
    npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

    bool OnGossipHello(Player* player, Creature* me)
    {
        std::string   msg = GetRankIcon(1273) + " " +
                            GetClassIconById(CLASS_SHAMAN) +
                            GetClassIconById(CLASS_ROGUE) +
                            "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " +
                            GetClassIconById(CLASS_PRIEST) +
                            GetClassIconById(CLASS_PALADIN) + " " +
                            GetRankIcon(1768);

        std::string  msg0 = GetRankIcon(-1) + " " +
                            GetClassIconById(CLASS_WARRIOR) +
                            GetClassIconById(CLASS_PRIEST) +
                            "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " +
                            GetClassIconById(CLASS_MAGE) +
                            GetClassIconById(CLASS_WARLOCK) + " " +
                            GetRankIcon(-1);

        std::string  msg1 = GetRankIcon(1576) + " " +
                            GetClassIconById(CLASS_PALADIN) +
                            GetClassIconById(CLASS_HUNTER) +
                            GetClassIconById(CLASS_SHAMAN) +
                            "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " +
                            GetClassIconById(CLASS_PRIEST) +
                            GetClassIconById(CLASS_SHAMAN) +
                            GetClassIconById(CLASS_MAGE) + " " +
                            GetRankIcon(2354);

        std::string  msg2 = GetRankIcon(700) + " " +
                            GetClassIconById(CLASS_MAGE) +
                            GetClassIconById(CLASS_ROGUE) +
                            GetClassIconById(CLASS_DRUID) +
                            "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " +
                            GetClassIconById(CLASS_HUNTER) +
                            GetClassIconById(CLASS_PRIEST) +
                            GetClassIconById(CLASS_PALADIN) + " " +
                            GetRankIcon(1856);

        std::string  msg3 = GetRankIcon(-1) + " " +
                            GetClassIconById(CLASS_HUNTER) +
                            GetClassIconById(CLASS_MAGE) +
                            GetClassIconById(CLASS_ROGUE) +
                            "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " +
                            GetClassIconById(CLASS_SHAMAN) +
                            GetClassIconById(CLASS_WARLOCK) +
                            GetClassIconById(CLASS_WARRIOR) + " " +
                            GetRankIcon(-1);

        player->ADD_GOSSIP_ITEM(0, "2v2 output example:", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_GAMES);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_GAMES);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, msg0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_GAMES);
        player->ADD_GOSSIP_ITEM(0, "3v3 output example:", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_GAMES);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, msg1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TOP_10 + ARENA_TEAM_2v2);
     	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, msg2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TOP_10 + ARENA_TEAM_3v3);
     	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, msg3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TOP_10 + ARENA_TEAM_5v5);

        GetGameListMenu(player, ARENA_TEAM_2v2);
        GetGameListMenu(player, ARENA_TEAM_3v3);
        GetGameListMenu(player, ARENA_TEAM_5v5);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
   	}

    std::string GetGameListMenu(Player* player, const uint8 type)
    {
        std::stringstream ss;
        uint32 action;
        ss << "|TInterface\\icons\\Achievement_Arena_" << (int)type << 'v' << (int)type << '_';
        const uint16 count = ListActiveArena(player, 0, true, type);
        if (count == 1)
        {
            ss << "7:26|t " << "Only one game avaible in " << (int)type << 'v' << (int)type;
            action = GOSSIP_ACTION_SHOW_GAMES + type;
        }
        else if (count)
        {
            ss << "7:26|t " << "Browse the " << (int)count << " games of " << (int)type << 'v' << (int)type;
            action = GOSSIP_ACTION_SHOW_GAMES + type;
        }
        else
        {
            ss << urand(1, 3) << ":26|t " << "No games of " << (int)type << 'v' << (int)type << " right now.";
            action = GOSSIP_ACTION_SHOW_MENU;
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, ss.str(), GOSSIP_SENDER_MAIN, action);
        return ss.str();
    }

    bool OnGossipSelect(Player* player, Creature* me, uint32 page, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action >= GOSSIP_ACTION_SPECTATE)
        {
            if (Player* target = ObjectAccessor::FindPlayer(action - GOSSIP_ACTION_SPECTATE))
            {
                ChatHandler handler(player->GetSession());
                char const* pTarget = target->GetName().c_str();
                m_usingGossip = true;
                arena_spectator_commands::HandleSpectateCommand(&handler, pTarget);
                return true;
            }
        }
        else if (action >= GOSSIP_ACTION_TOP_10)
            ListTopArena(player, action - GOSSIP_ACTION_TOP_10);
        else if (action >= GOSSIP_ACTION_SHOW_GAMES)
            ListActiveArena(player, 0, false, action - GOSSIP_ACTION_SHOW_GAMES);
        else if (action >= GOSSIP_ACTION_SHOW_GAMES_PAGE)
            ListActiveArena(player, page, false, action - GOSSIP_ACTION_SHOW_GAMES_PAGE);
        else
        {
            OnGossipHello(player, me);
            return true;
        }
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }

    void ListTopArena(Player* player, const uint8 type)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- return to menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MENU);
        if (type > 6)
            return;

        std::stringstream ss;
        ss << "SELECT name, rating FROM arena_team WHERE type='" << (int)type << "' ORDER BY rating DESC LIMIT 10";
        QueryResult result = CharacterDatabase.Query(ss.str().c_str());
 
        if (!result)
        {
            player->GetSession()->SendNotification("Arena teams not found...");
            return;
        }

        Field*  fields;
        uint32  rating;
        uint8   rank = 0;
        do
        {
            rank++;
            ss.str(std::string());
            fields = result->Fetch();
            rating = fields[1].GetUInt32();
            ss << "#" << rank + 0 << ' ' << fields[0].GetString() << " (" << rating << ')';
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), GOSSIP_SENDER_MAIN, 1);
        }
        while (result->NextRow());
    }

    std::string GetClassColorById(uint8 id)
    {
        std::string sClass = "";
        switch (id)
        {
            case CLASS_WARRIOR:
                sClass = "|cffC79C6E";
                break;
            case CLASS_PALADIN:
                sClass = "|cffF58CBA";
                break;
            case CLASS_HUNTER:
                sClass = "|cffABD473";
                break;
            case CLASS_ROGUE:
                sClass = "|cffFFF569";
                break;
            case CLASS_PRIEST:
                sClass = "|cffFFFFFF";
                break;
            case CLASS_DEATH_KNIGHT:
                sClass = "|cffC41F3B";
                break;
            case CLASS_SHAMAN:
                sClass = "|cff0070DE";
                break;
            case CLASS_MAGE:
                sClass = "|cff69CCF0";
                break;
            case CLASS_WARLOCK:
                sClass = "|cff9482C9";
                break;
            case CLASS_DRUID:
                sClass = "|cffFF7D0A";
                break;
        }
        return sClass;
    }

//Interface\BUTTONS\UI-GroupLoot-Dice-Up.blp //Interface\BUTTONS\UI-GroupLoot-Dice-Up.blp
    std::string GetRankIcon(const int32 rating)
    {
        if (rating < 0) // skirmish
            return (std::string("|TInterface\\BUTTONS\\UI-GroupLoot-Dice-Up:26|t"));

        uint8 iconID = 1;
        for (uint16 min = 1000; iconID < 15 && rating > min; min += 100)
            iconID++;

        std::stringstream ss;
        ss << "|TInterface\\PvPRankBadges\\PvPRank";
        if (iconID < 10)
            ss << '0';
        ss << (int)iconID << ":26|t";
        return ss.str();
    }

    std::string GetClassIconById(uint8 id)
    {
        std::string sClass = "";
        switch (id)
        {
            case CLASS_WARRIOR:
                sClass = "|TInterface\\icons\\Inv_sword_27:26|t";
                break;
            case CLASS_PALADIN:
                sClass = "|TInterface\\icons\\Ability_thunderbolt:26|t";
                break;
            case CLASS_HUNTER:
                sClass = "|TInterface\\icons\\Inv_weapon_bow_07:26|t";
                break;
            case CLASS_ROGUE:
                sClass = "|TInterface\\icons\\Inv_throwingknife_04:26|t";
                break;
            case CLASS_PRIEST:
                sClass = "|TInterface\\icons\\Inv_staff_30:26|t";
                break;
            case CLASS_DEATH_KNIGHT:
                sClass = "|TInterface\\CharacterFrame\\Button_ClassIcon_DeathKnight:26|t";
                break;
            case CLASS_SHAMAN:
                sClass = "|TInterface\\icons\\Spell_nature_bloodlust:26|t";
                break;
            case CLASS_MAGE:
                sClass = "|TInterface\\icons\\Inv_staff_13:26|t";
                break;
            case CLASS_WARLOCK:
                sClass = "|TInterface\\icons\\Spell_nature_drowsy:26|t";
                break;
            case CLASS_DRUID:
                sClass = "|TInterface\\icons\\inv_misc_monsterclaw_04:26|t";
                break;
        }
        return sClass;
    }

    std::string GetGamesStringData(Battleground* arena)
    {
        std::stringstream ss;
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (player->isSpectator())
                    continue;
                ss << GetClassIconById(player->getClass());
            }
        }
        return ss.str();
    }

    std::string GetGameIcons(Battleground* arena)
    {
        std::string teamsMember[BG_TEAMS_COUNT];
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (!player->isSpectator())
                    teamsMember[player->GetTeamId()] += GetClassIconById(player->getClass());
            }
        }
        if (arena->isRated())
            return GetRankIcon(arena->GetArenaMatchmakerRatingByIndex(0)) + teamsMember[0] + "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " + teamsMember[1] + GetRankIcon(arena->GetArenaMatchmakerRatingByIndex(1));
        return GetRankIcon(-1) + teamsMember[0] + "  |TInterface\\RAIDFRAME\\UI-RaidFrame-Threat:26|t  " + teamsMember[1] + GetRankIcon(-1);
    }

    uint64 GetFirstPlayerGuid(Battleground* arena)
    {
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
        {
            if (ObjectAccessor::FindPlayer(itr->first))
                return itr->first;
        }
        return 0;
    }

    uint16 ListActiveArena(Player* player, uint16 page, bool ammountOnly, const uint8 type)
    {
        uint16 elemCount = 0;
        bool haveNextPage = false;
        if (!ammountOnly)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- return to menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SHOW_MENU);
        for (uint8 i = 0; i <= MAX_BATTLEGROUND_TYPE_ID; ++i)
        {
            if (!sBattlegroundMgr->IsArenaType(BattlegroundTypeId(i)))
                continue;

            const BattlegroundContainer arenas = sBattlegroundMgr->GetBattlegroundsByType(BattlegroundTypeId(i));

            if (arenas.empty())
                continue;

	        for (BattlegroundContainer::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
            {
                Battleground* arena = itr->second;

                if (!arena->GetPlayersSize())
                    continue;

                if (type != arena->GetArenaType())
                    continue;

                elemCount++;

                if (ammountOnly)
                    continue;

                if (elemCount > (page + 1) * MAX_PER_PAGE)
                {
                    haveNextPage = true;
                    break;
                }
                //std::stringstream ss;

                //if (ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(type)))
                //    ss << at->GetName();

                if (!page || elemCount >= page * MAX_PER_PAGE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGameIcons(arena), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SPECTATE + GetFirstPlayerGuid(arena));
            }
        }
        if (!ammountOnly)
        {
            if (page > 0)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Prev..",  page - 1, GOSSIP_ACTION_SHOW_GAMES_PAGE + type);

            if (haveNextPage)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Next..", page + 1, GOSSIP_ACTION_SHOW_GAMES_PAGE + type);
        }
        return elemCount;
    }
};


void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}
