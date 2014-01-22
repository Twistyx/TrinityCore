	

    enum Enchants
    {
        ENCHANT_WEP_FIERY                           = 803,
        ENCHANT_WEP_ICEY                            = 1894,
        ENCHANT_WEP_SPELLPOWER              = 2504,
        ENCHANT_WEP_SPIRIT                  = 3864,
        ENCHANT_WEP_CRUSADER                = 1900,
        ENCHANT_WEP_AGILITY1H               = 2618,
        ENCHANT_WEP_LIFESTEALING            = 1898,
        ENCHANT_WEP_STRENGTH                = 2563,
        ENCHANT_WEP_INTELLECT               = 2568,
        ENCHANT_2WEP_FIERY                  = 803,
        ENCHANT_2WEP_ICEY                                   = 1894,
        ENCHANT_2WEP_CRUSADER                               = 1900,
        ENCHANT_2WEP_SPELLPOWER             = 2504,
        ENCHANT_2WEP_AGILITY                        = 1102,        
        ENCHANT_2WEP_LIFESTEALING           = 1898,
     
        ENCHANT_SHIELD_PROTECTION           = 848,
        ENCHANT_SHIELD_VITALITY             = 1890,
        ENCHANT_SHIELD_BLOCK                        = 1984,
        ENCHANT_SHIELD_FROST                        = 2987,
        ENCHANT_SHIELD_SPIRIT                       = 2937,
        ENCHANT_SHIELD_STAM                                 = 929,
       
        ENCHANT_CLOAK_PROT                  = 744,
        ENCHANT_CLOAK_DEFENSE               = 1889,
        ENCHANT_CLOAK_SUBLETY               = 2621,
        ENCHANT_CLOAK_FIRE                  = 2483,
        ENCHANT_CLOAK_AGILITY               = 2893,
        ENCHANT_CLOAK_STEALTH               = 910,
        ENCHANT_CLOAK_GRESIST               = 3242,
        ENCHANT_CLOAK_RESIST                = 1888,
     
        ENCHANT_GLOVES_STRENGTH             = 927,
        ENCHANT_GLOVES_HASTE                = 931,
        ENCHANT_GLOVES_SHADOW               = 2614,
        ENCHANT_GLOVES_HEALING              = 2320,
        ENCHANT_GLOVES_AGILITY              = 883,
        ENCHANT_GLOVES_FIRE                 = 2616,
        ENCHANT_GLOVES_FROST                = 2615,
        ENCHANT_GLOVES_THREAT               = 2613,
     
        ENCHANT_BRACERS_INTELLECT           = 905,
        ENCHANT_BRACERS_STAM                = 1885,
        ENCHANT_BRACERS_STRENGTH            = 1886,
        ENCHANT_BRACERS_SPIRIT              = 1884,
        ENCHANT_BRACERS_DEFLECTION          = 923,
        ENCHANT_BRACERS_HEALING             = 2319,
        ENCHANT_BRACERS_AGILITY             = 247,
     
        ENCHANT_CHEST_ABSORB                = 63,
        ENCHANT_CHEST_MANA                  = 1893,
        ENCHANT_CHEST_HEALTH                = 1892,
        ENCHANT_CHEST_STATS                 = 1891,
     
        ENCHANT_BOOTS_SPEED                 = 911,
        ENCHANT_BOOTS_SPIRIT                = 851,
        ENCHANT_BOOTS_AGILITY               = 1887,
        ENCHANT_BOOTS_ACCURACY              = 3858,
        ENCHANT_BOOTS_STAM                  = 929,
     
    };
     
    #include "ScriptPCH.h"
     
    void Enchant(Player* player, Item* item, uint32 enchantid)
    {
        if (!item)
        {
            player->GetSession()->SendNotification("You must equip the item you would like to enchant, in order to enchant it!");
            return;
        }
     
        if (!enchantid)
        {
            player->GetSession()->SendNotification("Something blew up! Sorry for the inconvenience, we'll send the troubleshooting gnomes right on it.");
            return;
        }
                   
        player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
            item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
            player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
        player->GetSession()->SendNotification("|cff800080%s |cffFF0000 Successfully enchanted!", item->GetTemplate()->Name1.c_str());
            }
     
    void RemoveEnchant(Player* player, Item* item)
    {
            if (!item)
            {
                    player->GetSession()->SendNotification("You don't have the item equipped.");
                    return;
            }
     
            item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
    }
     
    class enchanter : public CreatureScript
    {
    public:
        enchanter() : CreatureScript("enchanter") { }
     
            bool OnGossipHello(Player* player, Creature* creature)
            {
                player->ADD_GOSSIP_ITEM(1, "Weapon", GOSSIP_SENDER_MAIN, 1);
                            player->ADD_GOSSIP_ITEM(1, "Off-Hand Weapon", GOSSIP_SENDER_MAIN, 13);
                            player->ADD_GOSSIP_ITEM(1, "2H Weapon", GOSSIP_SENDER_MAIN, 2);
                            player->ADD_GOSSIP_ITEM(1, "Shield", GOSSIP_SENDER_MAIN, 3);
                player->ADD_GOSSIP_ITEM(1, "Cloak", GOSSIP_SENDER_MAIN, 6);
                player->ADD_GOSSIP_ITEM(1, "Chest", GOSSIP_SENDER_MAIN, 7);
                player->ADD_GOSSIP_ITEM(1, "Bracers", GOSSIP_SENDER_MAIN, 8);
                player->ADD_GOSSIP_ITEM(1, "Gloves", GOSSIP_SENDER_MAIN, 9);
                player->ADD_GOSSIP_ITEM(1, "Feet", GOSSIP_SENDER_MAIN, 11);
                            player->ADD_GOSSIP_ITEM(1, "I would like to remove an enchantment.", GOSSIP_SENDER_MAIN, 14);
     
                player->PlayerTalkClass->SendGossipMenu(100001, creature->GetGUID());
                            return true;
                    }
     
            bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
            {
                            player->PlayerTalkClass->ClearMenus();
                            Item * item;
     
                switch (action)
                {
                                   
                    case 1: // Enchant Weapon
     
                        player->ADD_GOSSIP_ITEM(1, "Icy Weapon", GOSSIP_SENDER_MAIN, 100);
                        player->ADD_GOSSIP_ITEM(1, "Mighty Spirit", GOSSIP_SENDER_MAIN, 101);
                        player->ADD_GOSSIP_ITEM(1, "Fiery Weapon", GOSSIP_SENDER_MAIN, 104);
                        player->ADD_GOSSIP_ITEM(1, "Crusader", GOSSIP_SENDER_MAIN, 105);
                        player->ADD_GOSSIP_ITEM(1, "Spell Power", GOSSIP_SENDER_MAIN, 106);
                        player->ADD_GOSSIP_ITEM(1, "Lifestealing", GOSSIP_SENDER_MAIN, 107);
                        player->ADD_GOSSIP_ITEM(1, "Agility (+15 Agility)", GOSSIP_SENDER_MAIN, 108);
                        player->ADD_GOSSIP_ITEM(1, "Srength", GOSSIP_SENDER_MAIN, 109);
                        player->ADD_GOSSIP_ITEM(1, "Mighty Intellect (+22 Intellect)", GOSSIP_SENDER_MAIN, 110);
                        player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                        player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
                                            break;
     
                     case 2: // Enchant 2H Weapon
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendNotification("You must equip a Two-Handed weapon first.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                                                    {
                                                            player->ADD_GOSSIP_ITEM(1, "Crusader", GOSSIP_SENDER_MAIN, 105);
                                                            player->ADD_GOSSIP_ITEM(1, "Agility (+25 Agility)", GOSSIP_SENDER_MAIN, 115);
                                                            player->ADD_GOSSIP_ITEM(1, "Fiery Weapon", GOSSIP_SENDER_MAIN, 116);
                                                            player->ADD_GOSSIP_ITEM(1, "Icy Weapon", GOSSIP_SENDER_MAIN, 117);
                                                            player->ADD_GOSSIP_ITEM(1, "Spellpower", GOSSIP_SENDER_MAIN, 106);
                                                            player->ADD_GOSSIP_ITEM(1, "Lifestealing", GOSSIP_SENDER_MAIN, 107);
                                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                                            player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
                                                            return true;
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendNotification("You don't have a Two-Handed weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                    case 3: // Enchant Shield
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendNotification("You must equip a shield first.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                            player->ADD_GOSSIP_ITEM(1, "Lesser Protection", GOSSIP_SENDER_MAIN, 118);
                                                            player->ADD_GOSSIP_ITEM(1, "Vitality", GOSSIP_SENDER_MAIN, 119);
                                                            player->ADD_GOSSIP_ITEM(1, "Lesser Block", GOSSIP_SENDER_MAIN, 120);
                                                            player->ADD_GOSSIP_ITEM(1, "Frost Resistance", GOSSIP_SENDER_MAIN, 121);
                                                            player->ADD_GOSSIP_ITEM(1, "Greater Spirit", GOSSIP_SENDER_MAIN, 122);
                                                            player->ADD_GOSSIP_ITEM(1, "Greater Stamina", GOSSIP_SENDER_MAIN, 123);
                                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                                            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
                                                            return true;
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendNotification("You do not have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                    case 6: // Enchant Cloak
                        player->ADD_GOSSIP_ITEM(1, "Lesser Protection", GOSSIP_SENDER_MAIN, 147);
                        player->ADD_GOSSIP_ITEM(1, "Superior Defense", GOSSIP_SENDER_MAIN, 148);                    
                        player->ADD_GOSSIP_ITEM(1, "Sublety", GOSSIP_SENDER_MAIN, 149);
                        player->ADD_GOSSIP_ITEM(1, "Lesser Fire Resistance", GOSSIP_SENDER_MAIN, 150);
                        player->ADD_GOSSIP_ITEM(1, "Lesser Agility", GOSSIP_SENDER_MAIN, 151);                    
                        player->ADD_GOSSIP_ITEM(1, "Stealth", GOSSIP_SENDER_MAIN, 152);
                        player->ADD_GOSSIP_ITEM(1, "Greater Resistance", GOSSIP_SENDER_MAIN, 153);
                        player->ADD_GOSSIP_ITEM(1, "Resistance", GOSSIP_SENDER_MAIN, 154);
                        player->ADD_GOSSIP_ITEM(1, "Lesser Shadow Resistance", GOSSIP_SENDER_MAIN, 155);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                        player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
                                            return true;
                                            break;
     
                                    case 7: //Enchant chest
                                            player->ADD_GOSSIP_ITEM(1, "Lesser Absorption", GOSSIP_SENDER_MAIN, 158);
                                            player->ADD_GOSSIP_ITEM(1, "Major Mana", GOSSIP_SENDER_MAIN, 159);
                                            player->ADD_GOSSIP_ITEM(1, "Major Health", GOSSIP_SENDER_MAIN, 160);
                                            player->ADD_GOSSIP_ITEM(1, "Greater Stats", GOSSIP_SENDER_MAIN, 161);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                            player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
                                            return true;
                                            break;
     
                                    case 8: //Enchant Bracers
                                            player->ADD_GOSSIP_ITEM(1, "Intellect", GOSSIP_SENDER_MAIN, 163);
                                            player->ADD_GOSSIP_ITEM(1, "Superior Strength", GOSSIP_SENDER_MAIN, 164);
                                            player->ADD_GOSSIP_ITEM(1, "Superior Stamina", GOSSIP_SENDER_MAIN, 165);
                                            player->ADD_GOSSIP_ITEM(1, "Superior Spirit", GOSSIP_SENDER_MAIN, 166);
                                            player->ADD_GOSSIP_ITEM(1, "Deflection", GOSSIP_SENDER_MAIN, 167);
                                            player->ADD_GOSSIP_ITEM(1, "Healing Power", GOSSIP_SENDER_MAIN, 168);
                                            player->ADD_GOSSIP_ITEM(1, "Minor Agility", GOSSIP_SENDER_MAIN, 169);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                            player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
                                            return true;
                                            break;
     
                                    case 9: //Enchant Gloves
                                            player->ADD_GOSSIP_ITEM(1, "Greater Strength", GOSSIP_SENDER_MAIN, 178);
                                            player->ADD_GOSSIP_ITEM(1, "Shadow Power", GOSSIP_SENDER_MAIN, 180);
                                            player->ADD_GOSSIP_ITEM(1, "Healing Power", GOSSIP_SENDER_MAIN, 181);
                                            player->ADD_GOSSIP_ITEM(1, "Superior Agility", GOSSIP_SENDER_MAIN, 182);
                                            player->ADD_GOSSIP_ITEM(1, "Fire Power", GOSSIP_SENDER_MAIN, 183);
                                            player->ADD_GOSSIP_ITEM(1, "Frost Power", GOSSIP_SENDER_MAIN, 184);
                                            player->ADD_GOSSIP_ITEM(1, "Threat", GOSSIP_SENDER_MAIN, 185);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                            player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
                                            return true;
                                            break;
     
                                    case 11: //Enchant feet
                                            player->ADD_GOSSIP_ITEM(1, "Minor Speed", GOSSIP_SENDER_MAIN, 191);
                                            player->ADD_GOSSIP_ITEM(1, "Spirit", GOSSIP_SENDER_MAIN, 192);
                                            player->ADD_GOSSIP_ITEM(1, "Greater Agility", GOSSIP_SENDER_MAIN, 193);
                                            player->ADD_GOSSIP_ITEM(1, "Lesser Accuracy", GOSSIP_SENDER_MAIN, 194);
                                            player->ADD_GOSSIP_ITEM(1, "Greater Stamina", GOSSIP_SENDER_MAIN, 195);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                            player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
                                            return true;
                                            break;
     
     
     
                                    case 13: //Enchant Off-Hand weapons
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendNotification("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPONMAINHAND)
                                                    {
                                                            player->ADD_GOSSIP_ITEM(1, "Fiery Weapon", GOSSIP_SENDER_MAIN, 205);
                                                            player->ADD_GOSSIP_ITEM(1, "Icey Weapon", GOSSIP_SENDER_MAIN, 206);
                                                            player->ADD_GOSSIP_ITEM(1, "Crusader", GOSSIP_SENDER_MAIN, 207);
                                                            player->ADD_GOSSIP_ITEM(1, "Agility", GOSSIP_SENDER_MAIN, 208);
                                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                                            player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
                                                            return true;
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendNotification("Your Off-Hand is not a weapon.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                                    case 14: //Remove enchant menu
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Main-hand", GOSSIP_SENDER_MAIN, 400);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Off-hand", GOSSIP_SENDER_MAIN, 401);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Cloak", GOSSIP_SENDER_MAIN, 404);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Chest", GOSSIP_SENDER_MAIN, 405);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Bracers", GOSSIP_SENDER_MAIN, 406);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Gloves", GOSSIP_SENDER_MAIN, 407);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Feet", GOSSIP_SENDER_MAIN, 409);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Legs", GOSSIP_SENDER_MAIN, 411);
                                            player->ADD_GOSSIP_ITEM(1, "Remove the enchantment on my Helm.", GOSSIP_SENDER_MAIN, 412);
                                            player->ADD_GOSSIP_ITEM(1, "<-Back", GOSSIP_SENDER_MAIN, 300);
                                            player->PlayerTalkClass->SendGossipMenu(100014, creature->GetGUID());
                                            return true;
                                            break;
     
                    case 100:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                    case 101:
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                    case 104:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_FIERY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 105:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_CRUSADER);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 106:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPELLPOWER);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 107:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFESTEALING);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 108:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY1H);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 109:
                                           
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_STRENGTH);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 110:
                        Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_INTELLECT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
     
                                    case 113:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_CRUSADER);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 115:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 116:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_FIERY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 117:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
     
                                                    if (!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_ICEY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 118:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_PROTECTION);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }                                              
                                            }
                        break;
     
                                    case 119:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_VITALITY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 120:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_BLOCK);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 121:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_FROST);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 122:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_SPIRIT);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                    case 123:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAM);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                        break;
     
                                   
     
                             case 147:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_PROT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 148:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DEFENSE);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 149:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SUBLETY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 150:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_FIRE);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 151:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_AGILITY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 152:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_STEALTH);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 153:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GRESIST);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 154:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_RESIST);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                            case 158:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_ABSORB);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 159:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_MANA);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 160:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_HEALTH);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 161:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_STATS);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 163:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 164:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_STAM);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 165:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_STRENGTH);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 166:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SPIRIT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 167:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_DEFLECTION);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 168:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_HEALING);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 169:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_AGILITY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 178:
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_STRENGTH);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 179:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HASTE);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 180:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_SHADOW);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 181:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HEALING);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 182:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 183:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_FIRE);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 184:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_FROST);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 185:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_THREAT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 191:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SPEED);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 192:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SPIRIT);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 193:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_AGILITY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 194:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ACCURACY);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                    case 195:
                                           
                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_STAM);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
     
                                   
                                    case 205:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_FIERY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                                    case 206:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                           
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                                                    {
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEY);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                                    case 207:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
                                                   
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                                                    {
                                                           
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_CRUSADER);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
                                    case 208:
                                            {
                                                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
     
                                                    if(!item)
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                            return false;
                                                    }
     
                                                    if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                                                    {
                                                            (player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND));
                                                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY1H);
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                                    else
                                                    {
                                                            player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                                                            player->PlayerTalkClass->SendCloseGossip();
                                                    }
                                            }
                                            break;
     
     
                                    case 300: //<-Back menu
                                                    player->ADD_GOSSIP_ITEM(1, "Weapon", GOSSIP_SENDER_MAIN, 1);
                                                    player->ADD_GOSSIP_ITEM(1, "Off-Hand Weapon", GOSSIP_SENDER_MAIN, 13);
                                                    player->ADD_GOSSIP_ITEM(1, "2H Weapon", GOSSIP_SENDER_MAIN, 2);
                                                    player->ADD_GOSSIP_ITEM(1, "Shield", GOSSIP_SENDER_MAIN, 3);
                                                    player->ADD_GOSSIP_ITEM(1, "Cloak", GOSSIP_SENDER_MAIN, 6);
                                                    player->ADD_GOSSIP_ITEM(1, "Chest", GOSSIP_SENDER_MAIN, 7);
                                                    player->ADD_GOSSIP_ITEM(1, "Bracers", GOSSIP_SENDER_MAIN, 8);
                                                    player->ADD_GOSSIP_ITEM(1, "Gloves", GOSSIP_SENDER_MAIN, 9);
                                                    player->ADD_GOSSIP_ITEM(1, "Feet", GOSSIP_SENDER_MAIN, 11);
                                                    break;
     
                                    case 400: //Remove enchant for mainhand
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                                    case 401: //Remove enchant for offhand
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                                    case 404: //remove enchant for cloak
                                            RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK));
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                                    case 405: //remove enchant for chest
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                                    case 406: //remove enchant for bracers
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                                    case 407: //remove enchant for gloves
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
     
                                    case 409: //remove enchant for feet
                                           
                                            player->PlayerTalkClass->SendCloseGossip();
                                            break;
     
                            }
            }
     
     
    };
     
    void AddSC_enchanter()
    {
      new enchanter();
    }

