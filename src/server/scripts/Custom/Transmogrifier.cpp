/*
5.1
Transmogrification 3.3.5a - Gossip menu
By Rochet2

ScriptName for NPC:
Creature_Transmogrify

TODO:
Make DB saving even better (Deleting)? What about coding?

Fix the cost formula
-- Too much data handling, use default costs

Are the qualities right?
Blizzard might have changed the quality requirements.
(TC handles it with stat checks)

Cant transmogrify rediculus items // Foereaper: would be fun to stab people with a fish
-- Cant think of any good way to handle this easily, could rip flagged items from cata DB
*/

#include "ScriptPCH.h"
#include "Config.h"
#include "Language.h"
#include "Transmogrification.h"

#define sT  sTransmogrification
#define GTS session->GetTrinityString // dropped translation support, no one using?

class CS_Transmogrification : public CreatureScript
{
public:
    CS_Transmogrification() : CreatureScript("Creature_Transmogrify") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        Item* head = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD);
        if (head && head->GetEntry() == 19972)
            ShowTransmogItems(player, creature, 0);
        else
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, std::string(LFH_LINK)+std::string(" Not equipped !"), 0, 0, std::string("You don't have the lucky fishing hat on you.\nCome back when you do !\n\n")+std::string(LFH_LINK), 0, false);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        WorldSession* session = player->GetSession();
        switch(sender)
        {
        case EQUIPMENT_SLOT_END: // Show items you can use
        case EQUIPMENT_SLOT_END+1: // Main menu
            OnGossipHello(player, creature);
            break;
        case EQUIPMENT_SLOT_END+2: // Remove Transmogrifications
            {
                bool removed = false;
                SQLTransaction trans = CharacterDatabase.BeginTransaction();
                for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
                {
                    if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    {
                        if (!sT->GetFakeEntry(newItem->GetGUID()))
                            continue;
                        sT->DeleteFakeEntry(player, slot, newItem, &trans);
                        removed = true;
                    }
                }
                if (removed)
                {
                    session->SendAreaTriggerMessage(GTS(LANG_ERR_UNTRANSMOG_OK));
                    CharacterDatabase.CommitTransaction(trans);
                }
                else
                    session->SendNotification(LANG_ERR_UNTRANSMOG_NO_TRANSMOGS);
                OnGossipHello(player, creature);
            } break;
        case EQUIPMENT_SLOT_END+3: // Remove Transmogrification from single item
            {
                if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
                {
                    if (sT->GetFakeEntry(newItem->GetGUID()))
                    {
                        sT->DeleteFakeEntry(player, action, newItem);
                        session->SendAreaTriggerMessage(GTS(LANG_ERR_UNTRANSMOG_OK));
                    }
                    else
                        session->SendNotification(LANG_ERR_UNTRANSMOG_NO_TRANSMOGS);
                }
                OnGossipSelect(player, creature, EQUIPMENT_SLOT_END, action);
            } break;
        default: // Transmogrify
            {
                if (!sender && !action)
                {
                    OnGossipHello(player, creature);
                    return true;
                }
                // sender = slot, action = display
                TransmogTrinityStrings res = sT->Transmogrify(player, MAKE_NEW_GUID(action, 0, HIGHGUID_ITEM), sender);
                if (res == LANG_ERR_TRANSMOG_OK)
                    session->SendAreaTriggerMessage(GTS(LANG_ERR_TRANSMOG_OK));
                else
                    session->SendNotification(res);
                OnGossipSelect(player, creature, EQUIPMENT_SLOT_END, sender);
            } break;
        }
        return true;
    }

    void ShowTransmogItems(Player* player, Creature* creature, uint8 slot) // Only checks bags while can use an item from anywhere in inventory
    {
        WorldSession* session = player->GetSession();
        Item* oldItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (oldItem)
        {
            uint32 limit = 0;
            std::ostringstream ss;
            ss << std::endl;

            for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            {
                if (limit > MAX_OPTIONS)
                    break;
                Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
                if (!newItem)
                    continue;
                if (!sT->CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
                    continue;
                if (sT->GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
                    continue;
                ++limit;
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, sT->GetItemIcon(newItem->GetEntry(), 30, 30, -18, 0)+sT->GetItemLink(newItem, session), slot, newItem->GetGUIDLow(), "Using this item for transmogrify will bind it to you and make it non-refundable and non-tradeable.\nDo you wish to continue?\n\n"+sT->GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10)+sT->GetItemLink(newItem, session)+ss.str(), 0, false);
            }

            for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
            {
                Bag* bag = player->GetBagByPos(i);
                if (!bag)
                    continue;
                for (uint32 j = 0; j < bag->GetBagSize(); ++j)
                {
                    if (limit > MAX_OPTIONS)
                        break;
                    Item* newItem = player->GetItemByPos(i, j);
                    if (!newItem)
                        continue;
                    if (!sT->CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
                        continue;
                    if (sT->GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
                        continue;
                    ++limit;
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, sT->GetItemIcon(newItem->GetEntry(), 30, 30, -18, 0)+sT->GetItemLink(newItem, session), slot, newItem->GetGUIDLow(), "Using this item for transmogrify will bind it to you and make it non-refundable and non-tradeable.\nDo you wish to continue?\n\n"+sT->GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10)+sT->GetItemLink(newItem, session)+ss.str(), 0, false);
                }
            }
        }
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Enchant_Disenchant:30:30:-18:0|tRemove transmogrification", EQUIPMENT_SLOT_END+3, slot, "Remove transmogrification from the slot?", 0, false);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-18:0|tUpdate menu", EQUIPMENT_SLOT_END, slot);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
};

class PS_Transmogrification : public PlayerScript
{
public:
    PS_Transmogrification() : PlayerScript("Player_Transmogrify") { }

    void OnLogin(Player* player)
    {
        uint64 playerGUID = player->GetGUID();
        sT->entryMap.erase(playerGUID);
        QueryResult result = CharacterDatabase.PQuery("SELECT GUID, FakeEntry FROM custom_transmogrification WHERE Owner = %u", player->GetGUIDLow());
        if (result)
        {
            do
            {
                uint64 itemGUID = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_ITEM);
                uint32 fakeEntry = (*result)[1].GetUInt32();
                if (sObjectMgr->GetItemTemplate(fakeEntry))
                {
                    sT->dataMap[itemGUID] = playerGUID;
                    sT->entryMap[playerGUID][itemGUID] = fakeEntry;
                }
            } while (result->NextRow());

            for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
            {
                if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    player->SetVisibleItemSlot(slot, item);
            }
        }
    }

    void OnLogout(Player* player)
    {
        uint32 pGUID = player->GetGUID();
        for (Transmogrification::transmogData::const_iterator it = sT->entryMap[pGUID].begin(); it != sT->entryMap[pGUID].end(); ++it)
            sT->dataMap.erase(it->first);
        sT->entryMap.erase(pGUID);
    }
};

class WS_Transmogrification : public WorldScript
{
public:
    WS_Transmogrification() : WorldScript("WS_Transmogrification") { }

    void OnConfigLoad(bool reload)
    {
        sT->LoadConfig(reload);
    }

    void OnStartup()
    {
        sT->LoadConfig(false);
        CharacterDatabase.Execute("DELETE FROM custom_transmogrification WHERE NOT EXISTS (SELECT 1 FROM item_instance WHERE item_instance.guid = custom_transmogrification.GUID)");
    }
};

void AddSC_CPWS_Transmogrification()
{
    new CS_Transmogrification();
    new PS_Transmogrification();
    new WS_Transmogrification();
}

#undef sT
#undef GTS
