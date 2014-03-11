#include "ScriptPCH.h"
#include "ItemEnchantmentMgr.h"

#define ELEM_COUNT     25 // Amount of items showed per page
#define TXT_PREV_PG     "<- Previous page"
#define TXT_GO_BACK     "<-- back to item list"
#define TXT_NEXT_PG     "Next page ->"
#define TXT_LAST_PAGE   "you reach the last page !"
#define TXT_KTHXBY      "Nevermind.."

enum Gossip_Option_Custom
{
    CUSTOM_OPTION_NONE          = 20,
    CUSTOM_OPTION_EXIT          = 21,
    CUSTOM_OPTION_ITEM_MENU     = 22,
    CUSTOM_OPTION_ITEM_MENU_P2  = 23,
    CUSTOM_OPTION_ITEM_MENU_P3  = 24,
    CUSTOM_OPTION_ITEM_MENU_P4  = 25,
    CUSTOM_OPTION_ITEM_MENU_P5  = 26,
    CUSTOM_OPTION_ITEM_MENU_P6  = 27,
    CUSTOM_OPTION_ITEM_MENU_P7  = 28,
    CUSTOM_OPTION_ITEM_MENU_MAX = 29,
    CUSTOM_OPTION_MAX
};

// this function generate the gossip menu for selecting random enchants
static bool GetList(Player* player, Creature* me, uint32 data, uint8 page)
{
    player->PlayerTalkClass->ClearMenus();

    //verify if we have the needed data
    if (!data)
        return (false);

    //isolate the itemId from the 24th first bits of data
    const uint32 itemId = (data & 0x7FFFFF);

    //Get item template from itemId
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemId);

    //if the item wasn't found stop here
    if (!itemProto)
        return (false);

    if (page > 1) //if we have more than one page, generate the next page gossip
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_PREV_PG, itemId, CUSTOM_OPTION_ITEM_MENU + page - 2);
    else          //else generate a link back to the vendor list
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_GO_BACK, 0, GOSSIP_OPTION_VENDOR);

    //initialize ench, it will contain all the items info, suffix need to be sent as negative value
    _EnchGossipList ench;
    if (itemProto->RandomProperty)
        ench = GetAllItemEnchant(itemProto->RandomProperty);
    else if (itemProto->RandomSuffix)
        ench = GetAllItemEnchant(-itemProto->RandomSuffix);
    else // if the item doesn't have a random suffix, stop here. (should never happen)
        return (false);

    //initialize a constant iterator to look into the list of random enchants
    _EnchGossipList::const_iterator it = ench.begin();
    int8 elem = 0;      //used to limit the amount of item / pages
    uint32 newData = 0; //used to hold the new data send to the GossipSelectMenu

    //if our enchantements list has more possibilities than the max element selected, generate next / last gossip menu
    if (ench.size() > ELEM_COUNT)
    {
        if ((page * ELEM_COUNT) < ench.size())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_NEXT_PG, itemId, CUSTOM_OPTION_ITEM_MENU + page);
        else
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_LAST_PAGE, itemId, CUSTOM_OPTION_ITEM_MENU + page - 1);
    }

    //if needed, skip to a specific page
    it += ((page - 1) * ELEM_COUNT);

    //loop trought our iterator until we reach the end or the limit of elements
    while (it != ench.end() && (elem <= ELEM_COUNT))
    {
        newData = it->ench;             //fill data with the id of the enchantement spell
        newData |= (data & 0xFF000000); //copy the 8 last bits of data to enchant.
        if (itemProto->RandomSuffix)    //if we are on a random suffix item
            newData |= 0x800000;        //set the 24th bit to 1 so i know i have to do a negation.
        //add the gossip for each enchant (description is stored in ench)
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, it->description, newData, itemId);
        elem++; //count + 1 elem
        it++;   //go to the next enchant
    }
    //add gossip to exit
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_KTHXBY, 0, CUSTOM_OPTION_EXIT);
    //add upper text
    player->PlayerTalkClass->SendGossipMenu(907, me->GetGUID());
    return (true);
}

//this function handle sending the buyitem decomposed opcode
static bool SellItem(Player* player, Creature* me, uint32 data, uint32 itemId)
{
    ItemPosCountVec dest;
    int32 randomEnchant = (data & 0x7FFFFF);   //init ench to the 23 first bits
    uint32 vendorslot = ((data >> 24) & 0xFF); // get the last 8 bits for the vendorslot
    if ((data >> 23) & 0x1)                    //check if the 24th bit is set
        randomEnchant = -randomEnchant;        //it's a suffix, we need to send a negative value
    player->BuyItemFromVendorSlot(me->GetGUID(), vendorslot, itemId, 1, NULL_BAG, NULL_SLOT, randomEnchant);

    //return to the vendor list
    player->PlayerTalkClass->ClearMenus();
    player->GetSession()->SendListInventory(me->GetGUID());
    return true;
}

class suffix_npc : public CreatureScript 
{
public:
suffix_npc() : CreatureScript("suffix_npc") { }
    bool OnGossipHello(Player *player, Creature *me) 
    {
        player->PlayerTalkClass->ClearMenus();
        player->GetSession()->SendListInventory(me->GetGUID());
        return (true);
    }

    bool OnGossipSelect(Player* player, Creature* me, uint32 data, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_OPTION_VENDOR) 
        {
            player->PlayerTalkClass->ClearMenus();
            player->GetSession()->SendListInventory(me->GetGUID());
        }
        else if (uiAction >= CUSTOM_OPTION_ITEM_MENU && uiAction < CUSTOM_OPTION_ITEM_MENU_MAX)
            return (GetList(player, me, data, uiAction - CUSTOM_OPTION_ITEM_MENU + 1));
        else if (uiAction > CUSTOM_OPTION_MAX) //in that case uiAction == itemID
            SellItem(player, me, data, uiAction);
        else
            player->CLOSE_GOSSIP_MENU();
        return (true);
    }
};

void AddSC_cyclone_customs() 
{
    new suffix_npc();
}
