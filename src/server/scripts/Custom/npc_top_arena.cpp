/*
          _______ _____  _______ _______ _______
         |    ___|     \|    ___|   |   |   |   |
         |    ___|  --  |    ___|       |   |   |
         |_______|_____/|_______|__|_|__|_______|
     Copyright (C) 2013 EmuDevs <http://www.emudevs.com/>
 
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.
 
  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.
 
  You should have received a copy of the GNU General Public License along
  with this program. If not, see <http://www.gnu.org/licenses/>.
*/
class npc_arena_teamTop : public CreatureScript
{
public:
    npc_arena_teamTop() : CreatureScript("npc_arena_teamTop") { }
     
    bool OnGossipHello(Player* player, Creature* me)
    {
        player->ADD_GOSSIP_ITEM(4,"|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_7:26|t|r Top 10: 2v2 Arena", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(4,"|cff00ff00|TInterface\\icons\\Achievement_Arena_3v3_7:26|t|r Top 10: 3v3 Arena", GOSSIP_SENDER_MAIN, 3);
        player->ADD_GOSSIP_ITEM(4,"|cff00ff00|TInterface\\icons\\Achievement_Arena_5v5_7:26|t|r Top 10: 5v5 Arena", GOSSIP_SENDER_MAIN, 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", GOSSIP_SENDER_MAIN, 1);
        player->SEND_GOSSIP_MENU(90085, me->GetGUID());
        return true;
    }
     
    bool OnGossipSelect(Player* player, Creature* me, uint32 /* sender */, uint32 actions)
    {
        if (!player)
            return false;
        player->PlayerTalkClass->ClearMenus();

        if (actions == 1)
            OnGossipHello(player, me);
        else if (actions < 6)
        {
            std::stringstream ss;
            ss << "SELECT name, rating FROM arena_team WHERE type='" << actions << "' ORDER BY rating DESC LIMIT 10";
            QueryResult result = CharacterDatabase.Query(ss.str().c_str());
            std::cout << ss.str() << '\n';

            Field*  fields;
            uint32  rating;
            uint8   rank = 0;
            if (!result)
            {
                player->GetSession()->SendNotification("Arena team not found...");
                return false;
            }

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

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", GOSSIP_SENDER_MAIN, 1);
            player->SEND_GOSSIP_MENU(90085, me->GetGUID());
        }
        else
            player->CLOSE_GOSSIP_MENU();
        return true;
    }
};
     
void AddSC_npc_top_arena()
{
    new npc_arena_teamTop;
}
