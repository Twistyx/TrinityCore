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
    // will be used for scrolling
    NPC_SPECTATOR_ACTION_LIST_GAMES         = 1000,
    NPC_SPECTATOR_ACTION_LIST_TOP_GAMES     = 2000,

    // NPC_SPECTATOR_ACTION_SELECTED_PLAYER + player.Guid()
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER    = 3000
};

const uint8  GamesOnPage    = 15;

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

		if (target && (target->HasAura(32728) || target->HasAura(32727)) && !m_usingGossip) // Check Arena Preparation thx XXsupr
        {
            handler->PSendSysMessage("Arena didn't started yet.");
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
        // save only in non-flight case
        else
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

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
     	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate : 3v3 Games", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
     	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate : 2v2 Games", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES);
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        return true;
   	}

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action >= NPC_SPECTATOR_ACTION_LIST_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES)
        {
            ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_GAMES, false);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        }
        else if (action >= NPC_SPECTATOR_ACTION_LIST_TOP_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES)
        {
            ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_TOP_GAMES, true);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        }
        else
        {
            if (Player* target = ObjectAccessor::FindPlayer(action - NPC_SPECTATOR_ACTION_SELECTED_PLAYER))
            {
                ChatHandler handler(player->GetSession());
                char const* pTarget = target->GetName().c_str();
				m_usingGossip = true;
                arena_spectator_commands::HandleSpectateCommand(&handler, pTarget);
            }
        }
        return true;
    }

    std::string GetClassNameById(uint8 id)
    {
        std::string sClass = "";
        switch (id)
        {
            case CLASS_WARRIOR:         sClass = "Warr ";          break;
            case CLASS_PALADIN:         sClass = "Pala ";           break;
            case CLASS_HUNTER:          sClass = "Hunt ";           break;
            case CLASS_ROGUE:           sClass = "Rogue ";          break;
            case CLASS_PRIEST:          sClass = "Priest ";         break;
            case CLASS_DEATH_KNIGHT:    sClass = "DK ";             break;
            case CLASS_SHAMAN:          sClass = "Sham ";           break;
            case CLASS_MAGE:            sClass = "Mage ";           break;
            case CLASS_WARLOCK:         sClass = "Lock ";           break;
            case CLASS_DRUID:           sClass = "Druid ";          break;
        }
        return sClass;
    }

    std::string GetGamesStringData(Battleground* team, uint16 mmr)
    {
        std::string teamsMember[BG_TEAMS_COUNT];
        uint32 firstTeamId = 0;
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (player->isSpectator())
                    continue;

                const uint32 team = itr->second.Team;
                if (!firstTeamId)
                    firstTeamId = team;

                teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
            }
        }

        std::string data = teamsMember[0] + "(";
        std::stringstream ss;
		std::stringstream sstwo;
        ss << mmr;
        data += ss.str();
        data += ") - " + teamsMember[1];		
        return data;
    }

    uint64 GetFirstPlayerGuid(Battleground* team)
    {
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
            if (ObjectAccessor::FindPlayer(itr->first)) // maybe unsafe ?
                return itr->first;
        return 0;
    }

    void ShowPage(Player* player, uint16 page, bool isTop)
    {
        uint16 TypeTwo = 0;
        uint16 TypeThree = 0;
        bool haveNextPage = false;
        for (uint8 i = 0; i <= MAX_BATTLEGROUND_TYPE_ID; ++i)
        {
            if (!sBattlegroundMgr->IsArenaType(BattlegroundTypeId(i)))
                continue;

            BattlegroundContainer arenas = sBattlegroundMgr->GetBattlegroundsByType(BattlegroundTypeId(i));

            if (arenas.empty())
                continue;

	        for (BattlegroundContainer::const_iterator itr = arenas.begin(); itr != arenas.end(); ++itr)
            {
                Battleground* arena = itr->second;

                if (!arena->GetPlayersSize())
                   continue;

                uint16 mmrTwo = arena->GetArenaMatchmakerRatingByIndex(0);
                uint16 mmrThree = arena->GetArenaMatchmakerRatingByIndex(1);

                if (isTop && arena->GetArenaType() == ARENA_TYPE_3v3)
                {
                    TypeThree++;
                    if (TypeThree > (page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                   	}

                    if (TypeThree >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrThree), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                }
                else if (!isTop && arena->GetArenaType() == ARENA_TYPE_2v2)
                {
                    TypeTwo++;
                    if (TypeTwo > (page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                    }

                    if (TypeTwo >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrTwo), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                }
            }
        }

        if (page > 0)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Prev..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page - 1);

        if (haveNextPage)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Next..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page + 1);
    }
};


void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}