#include "ScriptPCH.h"

static const bool send_cache_packets = true;    // change player cache?

// Remember to add to GetStatName too
static const uint32 statTypes[] = {
  ITEM_MOD_AGILITY,
  ITEM_MOD_STRENGTH,
  ITEM_MOD_INTELLECT,
  ITEM_MOD_SPIRIT,
  ITEM_MOD_DODGE_RATING,
  ITEM_MOD_PARRY_RATING,
  ITEM_MOD_BLOCK_RATING,
  ITEM_MOD_HIT_RATING,
  ITEM_MOD_CRIT_RATING,
  //ITEM_MOD_HASTE_RATING,
  ITEM_MOD_EXPERTISE_RATING,
  ITEM_MOD_ARMOR_PENETRATION_RATING,
  ITEM_MOD_SPELL_PENETRATION
};
static const uint32 stat_type_max = sizeof(statTypes) / sizeof(*statTypes);

typedef std::map<uint32, uint32> itemGuidMap;
typedef UNORDERED_MAP<uint32, itemGuidMap> playerItemMap;
static playerItemMap playerItems; // playerItems[plrguid][itemguid] = entry;

#define REFORGE_MOD 0.5f
#define REFORGE_COST 1*GOLD

namespace
{
    static const char* GetStatName(uint32 ItemStatType)
    {
        switch(ItemStatType)
        {
        case ITEM_MOD_AGILITY                    : return "Agility";
        case ITEM_MOD_STRENGTH                   : return "Strength";
        case ITEM_MOD_INTELLECT                  : return "Intellect";
        case ITEM_MOD_SPIRIT                     : return "Spirit";
        case ITEM_MOD_DODGE_RATING               : return "Dodge rating";
        case ITEM_MOD_PARRY_RATING               : return "Parry rating";
        case ITEM_MOD_BLOCK_RATING               : return "Block rating";
        case ITEM_MOD_HIT_RATING                 : return "Hit rating";
        case ITEM_MOD_CRIT_RATING                : return "Crit rating";
        //case ITEM_MOD_HASTE_RATING               : return "Haste rating";
        case ITEM_MOD_EXPERTISE_RATING           : return "Expertise rating";
        case ITEM_MOD_ARMOR_PENETRATION_RATING   : return "Armor Penetration rating";
        case ITEM_MOD_SPELL_PENETRATION          : return "Spell Penetration rating";
        default: return NULL;
        }
    }

    static const char* GetSlotName(uint8 slot, WorldSession* session)
    {
        switch (slot)
        {
        case EQUIPMENT_SLOT_HEAD      : return "Head";
        case EQUIPMENT_SLOT_SHOULDERS : return "Shoulders";
        case EQUIPMENT_SLOT_CHEST     : return "Chest";
        case EQUIPMENT_SLOT_WAIST     : return "Waist";
        case EQUIPMENT_SLOT_LEGS      : return "Legs";
        case EQUIPMENT_SLOT_FEET      : return "Feet";
        case EQUIPMENT_SLOT_WRISTS    : return "Wrists";
        case EQUIPMENT_SLOT_HANDS     : return "Hands";
        case EQUIPMENT_SLOT_BACK      : return "Back";
        case EQUIPMENT_SLOT_MAINHAND  : return "Main hand";
        case EQUIPMENT_SLOT_OFFHAND   : return "Off hand";
        case EQUIPMENT_SLOT_RANGED    : return "Ranged";
        default: return NULL;
        }
    }

    static Item* GetEquippedItem(Player* player, uint32 guidlow)
    {
        for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                if (pItem->GetGUIDLow() == guidlow)
                    return pItem;
        return NULL;
    }

    static void RefreshItem(Player* player, Item* pItem)
    {
        const uint8 slot = pItem->GetSlot();

        switch (slot)
        {
            case EQUIPMENT_SLOT_MAINHAND:
            case EQUIPMENT_SLOT_OFFHAND:
                player->UpdateExpertise(OFF_ATTACK);
            case EQUIPMENT_SLOT_RANGED:
                player->RecalculateRating(CR_ARMOR_PENETRATION);
                break;
            default: break;
        }
        player->AddEnchantmentDurations(pItem);
        player->AddItemDurations(pItem);

        player->_ApplyItemMods(pItem, slot, true);
    }

    static void RemoveReforgeSave(Player* player, uint32 itemguid, bool update, bool database);
    static void SendItemPacket(Player* player, uint32 entry, uint32 lowguid = 0)
    {
        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(entry);
        if (!pProto->StatsCount && (pProto->Damage[0].DamageMin && pProto->Quality > 2))
          return;
        FakeStatStruct* reforge = NULL;
        if (lowguid)
        {
            if (sObjectMgr->_itemFakeStatStore.find(lowguid) != sObjectMgr->_itemFakeStatStore.end())
                reforge = &sObjectMgr->_itemFakeStatStore[lowguid];
            else
                RemoveReforgeSave(player, lowguid, false, true);
        }

        // Update player cache (self only) pure visual.
        // HandleItemQuerySingleOpcode copy paste
        std::string Name        = pProto->Name1;
        std::string Description = pProto->Description;
        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (ItemLocale const* il = sObjectMgr->GetItemLocale(pProto->ItemId))
            {
                ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(il->Description, loc_idx, Description);
            }
        }
        WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600);
        data << pProto->ItemId;
        data << pProto->Class;
        data << pProto->SubClass;
        data << pProto->SoundOverrideSubclass;
        data << Name;
        data << uint8(0x00);                                //pProto->Name2; // blizz not send name there, just uint8(0x00); <-- \0 = empty string = empty name...
        data << uint8(0x00);                                //pProto->Name3; // blizz not send name there, just uint8(0x00);
        data << uint8(0x00);                                //pProto->Name4; // blizz not send name there, just uint8(0x00);
        data << pProto->DisplayInfoID;
        data << pProto->Quality;
        data << pProto->Flags;
        data << pProto->Flags2;
        data << pProto->BuyPrice;
        data << pProto->SellPrice;
        data << pProto->InventoryType;
        data << pProto->AllowableClass;
        data << pProto->AllowableRace;
        data << pProto->ItemLevel;
        data << pProto->RequiredLevel;
        data << pProto->RequiredSkill;
        data << pProto->RequiredSkillRank;
        data << pProto->RequiredSpell;
        data << pProto->RequiredHonorRank;
        data << pProto->RequiredCityRank;
        data << pProto->RequiredReputationFaction;
        data << pProto->RequiredReputationRank;
        data << int32(pProto->MaxCount);
        data << int32(pProto->Stackable);
        data << pProto->ContainerSlots;
        data << pProto->StatsCount + (reforge ? 1 : 0); // increase stat count by 1
        bool decreased = false;
        for (uint32 i = 0; i < pProto->StatsCount; ++i)
        {
            data << pProto->ItemStat[i].ItemStatType;
            if (reforge && !decreased && pProto->ItemStat[i].ItemStatType == reforge->decrease)
            {
                data << pProto->ItemStat[i].ItemStatValue-reforge->stat_value;
                decreased = true;
            }
            else
                data << pProto->ItemStat[i].ItemStatValue;
        }
        if (reforge)
        {
            data << reforge->increase;      // set new stat type
            data << reforge->stat_value;    // and value
        }
        data << pProto->ScalingStatDistribution;            // scaling stats distribution
        data << pProto->ScalingStatValue;                   // some kind of flags used to determine stat values column
        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
        {
            data << pProto->Damage[i].DamageMin;
            data << pProto->Damage[i].DamageMax;
            data << pProto->Damage[i].DamageType;
        }

        // resistances (7)
        data << pProto->Armor;
        data << pProto->HolyRes;
        data << pProto->FireRes;
        data << pProto->NatureRes;
        data << pProto->FrostRes;
        data << pProto->ShadowRes;
        data << pProto->ArcaneRes;

        data << pProto->Delay;
        data << pProto->AmmoType;
        data << pProto->RangedModRange;

        for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
        {
            // send DBC data for cooldowns in same way as it used in Spell::SendSpellCooldown
            // use `item_template` or if not set then only use spell cooldowns
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(pProto->Spells[s].SpellId);
            if (spell)
            {
                bool db_data = pProto->Spells[s].SpellCooldown >= 0 || pProto->Spells[s].SpellCategoryCooldown >= 0;

                data << pProto->Spells[s].SpellId;
                data << pProto->Spells[s].SpellTrigger;
                data << uint32(-abs(pProto->Spells[s].SpellCharges));

                if (db_data)
                {
                    data << uint32(pProto->Spells[s].SpellCooldown);
                    data << uint32(pProto->Spells[s].SpellCategory);
                    data << uint32(pProto->Spells[s].SpellCategoryCooldown);
                }
                else
                {
                    data << uint32(spell->RecoveryTime);
                    data << uint32(spell->Category);
                    data << uint32(spell->CategoryRecoveryTime);
                }
            }
            else
            {
                data << uint32(0);
                data << uint32(0);
                data << uint32(0);
                data << uint32(-1);
                data << uint32(0);
                data << uint32(-1);
            }
        }
        data << pProto->Bonding;
        data << Description;
        data << pProto->PageText;
        data << pProto->LanguageID;
        data << pProto->PageMaterial;
        data << pProto->StartQuest;
        data << pProto->LockID;
        data << int32(pProto->Material);
        data << pProto->Sheath;
        data << pProto->RandomProperty;
        data << pProto->RandomSuffix;
        data << pProto->Block;
        data << pProto->ItemSet;
        data << pProto->MaxDurability;
        data << pProto->Area;
        data << pProto->Map;                                // Added in 1.12.x & 2.0.1 client branch
        data << pProto->BagFamily;
        data << pProto->TotemCategory;
        for (int s = 0; s < MAX_ITEM_PROTO_SOCKETS; ++s)
        {
            data << pProto->Socket[s].Color;
            data << pProto->Socket[s].Content;
        }
        data << pProto->socketBonus;
        data << pProto->GemProperties;
        data << pProto->RequiredDisenchantSkill;
        data << pProto->ArmorDamageModifier;
        data << pProto->Duration;                           // added in 2.4.2.8209, duration (seconds)
        data << pProto->ItemLimitCategory;                  // WotLK, ItemLimitCategory
        data << pProto->HolidayId;                          // Holiday.dbc?
        player->GetSession()->SendPacket(&data);
    }

    static void SendReforgePackets(Player* player)
    {
        if (!send_cache_packets)
            return;
        if (playerItems.find(player->GetGUIDLow()) == playerItems.end())
            return;
        for (itemGuidMap::const_iterator it = playerItems[player->GetGUIDLow()].begin(); it != playerItems[player->GetGUIDLow()].end();)
        {
            itemGuidMap::const_iterator old_it = it++;
            SendItemPacket(player, old_it->second, old_it->first);
        }
    }

    static void UpdatePlayerCache(Player* player, bool inBankAlso = false)
    {
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
        {
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                SendItemPacket(player, pItem->GetEntry());
            }
        }
        for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
        {
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                SendItemPacket(player, pItem->GetEntry());
        }

        for (uint8 i = KEYRING_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i)
        {
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                SendItemPacket(player, pItem->GetEntry());
        }

        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
        {
            if (Bag* pBag = player->GetBagByPos(i))
                for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                {
                    if (Item* pItem = player->GetItemByPos(i, j))
                        SendItemPacket(player, pItem->GetEntry());
                }
        }

        if (inBankAlso)
        {
            // checking every item from 39 to 74 (including bank bags)
            for (uint8 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
            {
                if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                    SendItemPacket(player, pItem->GetEntry());
            }

            for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
            {
                if (Bag* pBag = player->GetBagByPos(i))
                    for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
                    {
                        if (Item* pItem = player->GetItemByPos(i, j))
                            SendItemPacket(player, pItem->GetEntry());
                    }
            }
        }
    }

    static void RemoveReforgeSave(Player* player, uint32 itemguid, bool update, bool database)
    {
        uint32 lowguid = player->GetGUIDLow();
        Item* invItem = update ? player->GetItemByGuid(MAKE_NEW_GUID(itemguid, 0, HIGHGUID_ITEM)) : NULL;
        if (invItem)
            player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
        if (playerItems.find(lowguid) != playerItems.end())
            playerItems[lowguid].erase(itemguid);
        sObjectMgr->_itemFakeStatStore.erase(itemguid);
        if (invItem)
            player->_ApplyItemMods(invItem, invItem->GetSlot(), true);

        if (!database)
            return;
        CharacterDatabase.PExecute("DELETE FROM `custom_reforges` WHERE `GUID` = %u", itemguid);
        if (invItem)
            SendItemPacket(player, invItem->GetEntry());
        player->SaveToDB();
    }

    static bool IsReforgable(Item* invItem, Player* player)
    {
        /*if (!invItem->IsEquipped())
        return false;*/
        if (invItem->GetOwnerGUID() != player->GetGUID())
            return false;
        const ItemTemplate* pProto = invItem->GetTemplate();
        //if (pProto->ItemLevel < 200) // WOTLK items have less itemlevel than CATA
        //    return false;
        if (pProto->Quality == ITEM_QUALITY_HEIRLOOM) // block heirlooms necessary? probably.
            return false;
        if (!pProto->StatsCount || pProto->StatsCount >= MAX_ITEM_PROTO_STATS)
            return false;
        if (sObjectMgr->_itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->_itemFakeStatStore.end())
            return false;
        for (uint32 i = 0; i < pProto->StatsCount; ++i)
        {
            if (!GetStatName(pProto->ItemStat[i].ItemStatType))
                continue;
            if (((int32)floorf((float)pProto->ItemStat[i].ItemStatValue * REFORGE_MOD)) > 1)
                return true;
        }
        return false;
    }

    static bool isRandomlyEnchanted(Item* invItem)
    {
        const ItemTemplate* pProto = invItem->GetTemplate();
        if (pProto->RandomSuffix || pProto->RandomProperty)
            return true;
        return false;
    }

    static void UpdatePlayerReforgeStats(Item* invItem, Player* player, uint32 decrease, uint32 increase) // stat types
    {
        const ItemTemplate* pProto = invItem->GetTemplate();

        int32 stat_diff = 0;
        for (uint32 i = 0; i < pProto->StatsCount; i++)
        {
            if (pProto->ItemStat[i].ItemStatType == increase)
                return; // Should not have the increased stat already
            if (pProto->ItemStat[i].ItemStatType == decrease)
                stat_diff = (int32)floorf((float)pProto->ItemStat[i].ItemStatValue * REFORGE_MOD);
        }
        if (stat_diff <= 0)
            return; // Should have some kind of diff

        // Update player stats
        player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
        uint32 guidlow = invItem->GetGUIDLow();
        sObjectMgr->_itemFakeStatStore[guidlow].increase = increase;
        sObjectMgr->_itemFakeStatStore[guidlow].decrease = decrease;
        sObjectMgr->_itemFakeStatStore[guidlow].stat_value = stat_diff;
        player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
        CharacterDatabase.PExecute("REPLACE INTO `custom_reforges` (`GUID`, `increase`, `decrease`, `stat_value`) VALUES (%u, %u, %u, %i)", guidlow, increase, decrease, stat_diff);
        player->ModifyMoney(pProto->SellPrice < (REFORGE_COST) ? (-REFORGE_COST) : -(int32)pProto->SellPrice);
        SendItemPacket(player, invItem->GetEntry(), guidlow);
        player->SaveToDB();
    }
}

class REFORGE_PLAYER : public PlayerScript
{
public:
    REFORGE_PLAYER() : PlayerScript("REFORGE_PLAYER")
    {
        CharacterDatabase.Execute("DELETE FROM `custom_reforges` WHERE NOT EXISTS (SELECT 1 FROM `item_instance` WHERE `item_instance`.`guid` = `custom_reforges`.`GUID`)");
    }

    class SendRefPackLogin : public BasicEvent
    {
    public:
        SendRefPackLogin(Player* _player) : player(_player)
        {
            _player->m_Events.AddEvent(this, _player->m_Events.CalculateTime(1000));
        }

        bool Execute(uint64, uint32)
        {
            UpdatePlayerCache(player, true);
            //SendReforgePackets(player);
            return true;
        }
        Player* player;
    };

    void OnLogin(Player* player)
    {
        uint32 playerGUID = player->GetGUIDLow();
        QueryResult result = CharacterDatabase.PQuery("SELECT custom_reforges.`GUID`, `increase`, `decrease`, `stat_value`, item_instance.itementry FROM `custom_reforges`, item_instance WHERE item_instance.`owner_guid` = %u and item_instance.`guid` = custom_reforges.GUID", playerGUID);
        if (result)
        {
            do
            {
                uint32 lowGUID = (*result)[0].GetUInt32();
                uint32 entry = (*result)[4].GetUInt32();
                Item* invItem = player->GetItemByGuid(MAKE_NEW_GUID(lowGUID, 0, HIGHGUID_ITEM));
                if (invItem)
                    player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
                sObjectMgr->_itemFakeStatStore[lowGUID].increase = (*result)[1].GetUInt32();
                sObjectMgr->_itemFakeStatStore[lowGUID].decrease = (*result)[2].GetUInt32();
                sObjectMgr->_itemFakeStatStore[lowGUID].stat_value = (*result)[3].GetInt32();
                playerItems[playerGUID][lowGUID] = entry;
                if (invItem)
                    player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
                // SendItemPacket(player, entry, lowGUID);
            } while (result->NextRow());

            // SendReforgePackets(player);
            new SendRefPackLogin(player);
        }
    }

    void OnLevelChanged(Player* player, uint8 /*oldLevel*/)
    {
        UpdatePlayerCache(player, true);
    }

    void OnLogout(Player* player)
    {
        if (playerItems.find(player->GetGUIDLow()) == playerItems.end())
            return;
        for (itemGuidMap::const_iterator it = playerItems[player->GetGUIDLow()].begin(); it != playerItems[player->GetGUIDLow()].end();)
        {
            itemGuidMap::const_iterator old_it = it++;
            RemoveReforgeSave(player, old_it->first, false, false);
        }
    }
};

class REFORGER_NPC : public CreatureScript
{
public:
    REFORGER_NPC() : CreatureScript("REFORGER_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        selectedItem.erase(player->GetGUIDLow());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Select slot of the item:", MAIN_MENU, 0);
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
        {
            if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                if (IsReforgable(invItem, player))
                {
                    if (const char* slotname = GetSlotName(slot, player->GetSession()))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Reforge: "+std::string(slotname), SELECT_STAT_REDUCE, slot);
                }
                else if (isRandomlyEnchanted(invItem))
                {
                    if (const char* slotname = GetSlotName(slot, player->GetSession()))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Reroll: "+std::string(slotname), SELECT_STAT_REROLL, slot);
                }
            }
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Remove reforges", SELECT_RESTORE, 0);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Update menu", MAIN_MENU, 0);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (!player) return false;
        player->PlayerTalkClass->ClearMenus();
        switch(sender)
        {
            case MAIN_MENU: OnGossipHello(player, creature); break;
            case SELECT_STAT_REROLL: // action = slot
            {
                Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action);
                if (invItem && isRandomlyEnchanted(invItem))
                {
                    player->_ApplyItemMods(invItem, invItem->GetSlot(), false);

                    // Get Possible Enchant list
                    const ItemTemplate* pProto = invItem->GetTemplate();
                    _EnchGossipList ench = GetAllItemEnchant((pProto->RandomProperty) ? pProto->RandomProperty : -pProto->RandomSuffix);

                    // Pick one and check it's different from the current enchant
                    const uint16 max = ench.size() - 1;
                    uint16 roll = urand(0, max);
                    while (abs(invItem->GetItemRandomPropertyId()) == ench[roll].ench)
                        roll = urand(0, max);
                    invItem->SetItemRandomProperties((pProto->RandomProperty) ? ench[roll].ench : -ench[roll].ench);

                    ChatHandler(player->GetSession()).PSendSysMessage(("New roll: " + ench[roll].description).c_str());
                    RefreshItem(player, invItem);
                    OnGossipHello(player, creature);
                }
                else
                {
                    player->GetSession()->SendNotification("Invalid item selected");
                    OnGossipHello(player, creature);
                }
            } break;
            case SELECT_STAT_REDUCE: // action = slot
            {
                Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action);
                if (invItem && IsReforgable(invItem, player))
                {
                    selectedItem[player->GetGUIDLow()] = invItem->GetGUIDLow();
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Stat to decrease:", sender, action);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back..", MAIN_MENU, 0);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                else
                {
                    player->GetSession()->SendNotification("Invalid item selected");
                    OnGossipHello(player, creature);
                }
            } break;
            case SELECT_STAT_INCREASE: // action = StatsCount id
            {
                Item* invItem = GetEquippedItem(player, selectedItem[player->GetGUIDLow()]);
                if (invItem)
                {
                    const ItemTemplate* pProto = invItem->GetTemplate();
                    int32 stat_diff = ((int32)floorf((float)pProto->ItemStat[action].ItemStatValue * REFORGE_MOD));

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Stat to increase:", sender, action);
                    for (uint32 i = 0; i < stat_type_max; ++i)
                    {
                        bool cont = false;
                        for (uint32 j = 0; j < pProto->StatsCount; ++j)
                        {
                            if (statTypes[i] == pProto->ItemStat[j].ItemStatType) // skip existing stats on item
                            {
                                cont = true;
                                break;
                            }
                        }
                        if (cont) continue;
                        if (const char* stat_name = GetStatName(statTypes[i]))
                        {
                            std::ostringstream oss;
                            oss << stat_name << " |cFF3ECB3C+" << stat_diff << "|r";
                            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, oss.str(), pProto->ItemStat[action].ItemStatType, statTypes[i], "Are you sure you want to reforge\n\n"+pProto->Name1, (pProto->SellPrice < (REFORGE_COST) ? (REFORGE_COST) : pProto->SellPrice), false);
                        }
                    }
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back..", SELECT_STAT_REDUCE, invItem->GetSlot());
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                else
                {
                    player->GetSession()->SendNotification("Invalid item selected");
                    OnGossipHello(player, creature);
                }
            } break;
            case SELECT_RESTORE:
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Select slot to remove reforge from:", sender, action);
                for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
                {
                    if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    {
                        if (sObjectMgr->_itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->_itemFakeStatStore.end())
                        {
                            if (const char* slotname = GetSlotName(slot, player->GetSession()))
                                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, slotname, RESTORE, slot, "Really remove reforge from\n\n"+std::string(slotname), 0, false);
                        }
                    }
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Update menu", sender, action);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back..", MAIN_MENU, 0);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            } break;
            case RESTORE:
            // action = slot
            {
                if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
                {
                    if (sObjectMgr->_itemFakeStatStore.find(invItem->GetGUIDLow()) != sObjectMgr->_itemFakeStatStore.end())
                        RemoveReforgeSave(player, invItem->GetGUIDLow(), true, true);
                }
                OnGossipHello(player, creature);
            } break;
            default: // Reforge, sender = stat type to decrease, action = stat type to increase
            {
                Item* invItem = GetEquippedItem(player, selectedItem[player->GetGUIDLow()]);
                if (invItem && IsReforgable(invItem, player))
                {
                    if (player->HasEnoughMoney(invItem->GetTemplate()->SellPrice < (REFORGE_COST) ? (REFORGE_COST) : invItem->GetTemplate()->SellPrice))
                    {
                        // int32 stat_diff = ((int32)floorf((float)invItem->GetTemplate()->ItemStat[action].ItemStatValue * REFORGE_MOD));
                        UpdatePlayerReforgeStats(invItem, player, sender, action); // rewrite this function
                    }
                    else player->GetSession()->SendNotification("Not enough money");
                }
                else player->GetSession()->SendNotification("Invalid item selected");
                OnGossipHello(player, creature);
            } break;
        }
        return true;
    }

    enum Senders
    {
        MAIN_MENU = 100,
        SELECT_ITEM,
        SELECT_STAT_REDUCE,
        SELECT_STAT_REROLL,
        SELECT_STAT_INCREASE,
        SELECT_RESTORE,
        RESTORE,
        REFORGE,
    };

private:

    UNORDERED_MAP<uint32, uint32> selectedItem;
};

void AddSC_REFORGER_NPC()
{
    new REFORGER_NPC;
    new REFORGE_PLAYER;
}

#undef FOR_REFORGE_ITEMS
