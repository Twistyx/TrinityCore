#include "ScriptPCH.h"
#include "ItemEnchantmentMgr.h"

#define H_X             -1258.48f
#define H_Y             63.53f
#define H_Z             127.9f
#define A_X             -1208.98f
#define A_Y             -88.27f
#define A_Z             161.50f
#define TOKEN           41596
#define TOKEN_COUNT     10
#define TITLE_REWARDED  47 // title "The Conqueror"
#define ELEM_COUNT      25 // Amount of enchants showed per page
#define TXT_PREV_PG     "<- Previous page"
#define TXT_GO_BACK     "<-- back to item list"
#define TXT_NEXT_PG     "Next page ->"
#define TXT_LAST_PAGE   "you reach the last page !"
#define TXT_KTHXBY      "Nevermind.."
#define TXT_BAD_SKILL   "Profession NPC: received non-valid skill ID (LearnAllRecipesInProfession)"
#define TXT_ERR_SKILL   "you already have that skill"
#define TXT_ERR_MAX     "you already know two professions!"
#define TXT_PROBLEM     "Internal error occured!"
#define TXT_SPRINT      "Make me the fastest turtle ever!"
#define TXT_GET_TITLE   "Gimi my Title !"
#define TXT_VENDOR_LIST "Show me your goodies..."
#define TXT_ERR_TITLE   "you already have the Title :)"
#define TXT_NO_FLAG     "Gimi Vendor flags plz."
#define FORMAT_END      ":20:20|t "
#define TXT_NEXT_TITLE  "Next title : |TInterface/PvPRankBadges/PvPRank"
#define TXT_REQ_TITLE   "you need to be at least : |TInterface/PvPRankBadges/PvPRank"
#define TXT_MAX_RANK    "you already are max rank !"
#define TXT_WSG_MARK    " Warsong Gulch Mark of Ranking to go! We sell the same items as the Arena Vendors do."

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
    CUSTOM_OPTION_UNLEARN       = 30,
    CUSTOM_OPTION_TITLE_CLIMB   = 31,
    CUSTOM_OPTION_SPRINT        = 32,
    CUSTOM_OPTION_MAX
};

enum NPCs { 
    NPC_ALLIANCE_LOOK_BACK    = 500113,
    NPC_ALLIANCE_LOOK_ARROUND = 500114,
    NPC_HORDE_LOOK_BACK       = 500115,
    NPC_HORDE_LOOK_ARROUND    = 500116
};

static const char   *titlesNames[28] =
{
    "Private",
    "Corporal",
    "Sergeant",
    "Master Sergeant",
    "Sergeant Major",
    "Knight",
    "Knight Lieutenant",
    "Knight Captain",
    "Knight Champion",
    "Lieutenant Commander",
    "Commander",
    "Marshal",
    "Field Marshal",
    "Grand Marshal",
    "Scout",
    "Grunt",
    "Sergeant",
    "Senior Sergeant",
    "First Sergeant",
    "Stone Guard",
    "Blood Guard",
    "Legionnaire",
    "Centurion",
    "Champion",
    "Lieutenant General",
    "General",
    "Warlord",
    "High Warlord"
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

static uint32 GetTotalTokens(Player* Player) 
{
    //intialize an item
    Item*           pItem;
    //loop in the currencytoken slots
    for (uint8 i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i) 
    {
        //try to set or item to the one from this slot
        pItem = Player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        //if player have an item in this slot and his entry is the same has our token return the count
        if (pItem && pItem->GetEntry() == TOKEN) 
            return (pItem->GetCount());
    }
    return (0);
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

class profession_npc : public CreatureScript 
{
public:
    profession_npc() : CreatureScript("profession_npc") {}

    bool LearnAllRecipesInProfession(Player* player, SkillType skill)
    {
        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);

        uint16  skill_level = 225;

        if (!SkillInfo) 
        {
            sLog->outError(LOG_FILTER_PLAYER_SKILLS, TXT_BAD_SKILL);
            return (false);
        }
        if (skill == SKILL_ENGINEERING)
            skill_level = 150;
        player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), skill_level, skill_level);
        return (true);
    }

    bool ForgotSkill(Player* player, SkillType skill)
    {
        if (!player->HasSkill(skill))
            return (false);
        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
        if (!SkillInfo) 
        {
            sLog->outError(LOG_FILTER_PLAYER_SKILLS, TXT_BAD_SKILL);
            return (false);
        }
        player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), 0, 0);
        return (true);
    }

    bool PlayerAlreadyHasTwoProfessions(Player *player)
    {
        uint32 skillCount = player->HasSkill(SKILL_MINING)   + player->HasSkill(SKILL_SKINNING)
                          + player->HasSkill(SKILL_HERBALISM)+ player->HasSkill(SKILL_ENGINEERING);
        if (skillCount >= 2)
            return true;
        return false;
    }

    void CompleteLearnProfession(Player *player, SkillType skill)
    {
        if (PlayerAlreadyHasTwoProfessions(player))
            player->GetSession()->SendNotification(TXT_ERR_MAX);
        else if (!LearnAllRecipesInProfession(player, skill))
            player->GetSession()->SendNotification(TXT_PROBLEM);
    }

    bool OnGossipHello(Player *player, Creature* me) 
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,    "Engineering", GOSSIP_SENDER_MAIN,    SKILL_ENGINEERING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,    "Herbalism",   GOSSIP_SENDER_MAIN,    SKILL_HERBALISM);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,    "Skinning",    GOSSIP_SENDER_MAIN,    SKILL_SKINNING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,    "Mining",      GOSSIP_SENDER_MAIN,    SKILL_MINING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Unlearn All", CUSTOM_OPTION_UNLEARN, 0);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT,        TXT_KTHXBY, CUSTOM_OPTION_EXIT,    0);
        player->PlayerTalkClass->SendGossipMenu(907, me->GetGUID());
        return (true);
    }
 
    bool OnGossipSelect(Player* Player, Creature* me, uint32 uiSender, uint32 skill) 
    {
        Player->PlayerTalkClass->ClearMenus();
 
        if (uiSender == CUSTOM_OPTION_UNLEARN) 
        {
            ForgotSkill(Player, SKILL_ENGINEERING);
            ForgotSkill(Player, SKILL_HERBALISM);
            ForgotSkill(Player, SKILL_SKINNING);
            ForgotSkill(Player, SKILL_MINING);
        }
        else if (uiSender == GOSSIP_SENDER_MAIN) 
        {
            if(Player->HasSkill(skill))
                Player->GetSession()->SendNotification(TXT_ERR_SKILL);
            else
                CompleteLearnProfession(Player, (SkillType)skill);
        }
        if (uiSender == CUSTOM_OPTION_EXIT)
            Player->PlayerTalkClass->SendCloseGossip();
        else
            OnGossipHello(Player, me);
        return (true);
    }
};

class squirel_pew_pew : public CreatureScript 
{
public:
    squirel_pew_pew() : CreatureScript("squirel_pew_pew") {}

    bool OnGossipHello(Player* player, Creature* me)
    {
        player->PlayerTalkClass->ClearMenus();
        if (me->IsVendor())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, TXT_VENDOR_LIST, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
        else
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, TXT_NO_FLAG, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
        if (player->HasTitle(TITLE_REWARDED))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, TXT_GET_TITLE, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_TITLE_CLIMB);
        else
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, TXT_ERR_TITLE, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_TITLE_CLIMB);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, TXT_SPRINT, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_SPRINT);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_KTHXBY, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
        player->PlayerTalkClass->SendGossipMenu(8, me->GetGUID());
        return (true);
    }

    bool OnGossipSelect(Player* player, Creature* me, uint32 /*uiSender*/, uint32 uiAction) 
    {
        if (uiAction == CUSTOM_OPTION_TITLE_CLIMB) 
        {
            player->SetTitle(sCharTitlesStore.LookupEntry(TITLE_REWARDED));
            player->PlayerTalkClass->SendCloseGossip();
        }
        else if (uiAction == CUSTOM_OPTION_SPRINT) 
        {
            player->CastSpell(player, 50085, 0); // Spell Slow Fall
            player->CastSpell(player, 51582, 0); // Spell Rocket Boots Engaged
            player->PlayerTalkClass->SendCloseGossip();
        }
        else if (uiAction == GOSSIP_OPTION_VENDOR) 
        {
            player->PlayerTalkClass->ClearMenus();
            player->GetSession()->SendListInventory(me->GetGUID());
        }
        else
            player->PlayerTalkClass->SendCloseGossip();
        return (true);
    }
};

class title_vendor : public CreatureScript 
{
public:
    title_vendor() : CreatureScript("title_vendor") {}

    uint16 NeededToken(uint32 rank) 
    {
        uint16 total = 0;

        while (rank) 
        {
            total += TOKEN_COUNT * rank;
            rank--;
        }
        return (total);
    }

    bool OnGossipHello(Player* player, Creature* me) 
    {
        std::ostringstream  ss;
        const uint32        totalTokens = GetTotalTokens(player);
        const int8          faction     = (player->GetTeam() == ALLIANCE) ? -1 : 13;
        const uint8         npcTitle    = me->GetCreatureTemplate()->maxgold;
        const uint16        reqTokens   = NeededToken(npcTitle);

        if (totalTokens >= reqTokens) 
        {
            if (!me->IsVendor())
                return (false);
            player->PlayerTalkClass->ClearMenus();
            player->GetSession()->SendListInventory(me->GetGUID());
        }
        else 
        {
            player->PlayerTalkClass->ClearMenus();
            ss << TXT_REQ_TITLE;
            if (npcTitle < 10)
                ss << "0";
            ss << 0 + npcTitle << FORMAT_END << titlesNames[npcTitle + faction];
            ss << " still " << reqTokens - totalTokens << TXT_WSG_MARK;
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, ss.str().c_str(), GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_KTHXBY, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
            player->PlayerTalkClass->SendGossipMenu(8, me->GetGUID());
        }
        return (true);
    }

    bool OnGossipSelect(Player* player, Creature* me, uint32 data, uint32 uiAction) 
    {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_OPTION_VENDOR)
            player->GetSession()->SendListInventory(me->GetGUID());
        else if (uiAction >= CUSTOM_OPTION_ITEM_MENU && uiAction < CUSTOM_OPTION_ITEM_MENU_MAX)
            return (GetList(player, me, data, uiAction - CUSTOM_OPTION_ITEM_MENU + 1));
        else if (uiAction > CUSTOM_OPTION_MAX) //in that case uiAction == itemID lol
            SellItem(player, me, data, uiAction);
        else
            player->PlayerTalkClass->SendCloseGossip();
        return (true);
    }
};

class title_npc : public CreatureScript 
{
public:
    title_npc() : CreatureScript("title_npc") {}

    void RewardTitles(Player *player, uint8 *nextTitle, uint16 *reqTokens, const uint16 totalTokens, const uint8 faction)
    {
        while ((*nextTitle < 14) && (((*reqTokens += (1 + *nextTitle) * TOKEN_COUNT)) <= totalTokens)) 
        {
            (*nextTitle)++;
            player->SetTitle(sCharTitlesStore.LookupEntry(*nextTitle + faction));
        }
    }

    const char *GetNextTitleName(const uint8 nextTitle, const uint16 reqTokens, const uint16 totalTokens, const uint8 faction)
    {
        std::ostringstream  ss;

        if (nextTitle > 13)
            ss << TXT_MAX_RANK;
        else
        {
            ss << TXT_NEXT_TITLE;
            if (nextTitle < 9)
                ss << '0';
            ss << 1 + nextTitle << FORMAT_END << titlesNames[nextTitle + faction];
            ss << " in " << reqTokens - totalTokens << " tokens.";
        }
        return (ss.str().c_str());
    }

    bool OnGossipHello(Player* player, Creature* me)
    {
        const uint16  totalTokens = GetTotalTokens(player);
        const uint8   faction     = (player->GetTeam() == ALLIANCE) ? 0 : 14;
        uint8         nextTitle   = 0;
        uint16        reqTokens   = 0;

        RewardTitles(player, &nextTitle, &reqTokens, totalTokens, faction);
        const char *gossipText = GetNextTitleName(nextTitle, reqTokens, totalTokens, faction);
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, gossipText, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, TXT_KTHXBY, GOSSIP_SENDER_MAIN, CUSTOM_OPTION_EXIT);
        player->PlayerTalkClass->SendGossipMenu(9425, me->GetGUID());
        return (true);
    }

    bool OnGossipSelect(Player* player, Creature* /*me*/, uint32 /*uiSender*/, uint32 /*uiAction*/) 
    {
        player->PlayerTalkClass->SendCloseGossip();
        return (true);
    }
};

class npc_teleport_donation : public CreatureScript {
public:
    npc_teleport_donation() : CreatureScript("npc_teleport_donation"){ }
    bool OnGossipHello(Player *player, Creature * m_creature) {
        player->ADD_GOSSIP_ITEM(3, "Bring me to the Donation Vendors", GOSSIP_SENDER_MAIN, 0);
        player->ADD_GOSSIP_ITEM(4, "Nevermind",                        GOSSIP_SENDER_MAIN, 150);
        player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
        return (true);
    }
 
    bool OnGossipSelect(Player *player, Creature * /*me*/, uint32 /*from*/, uint32 action) {
        if (action == 0)
            player->TeleportTo(43, 118.367088f, 239.832443f, -96.010773f, 6.237127f);
        else if (action == 150)
            player->CLOSE_GOSSIP_MENU();
        return (true);
    }
};

class npc_teleport_guard : public CreatureScript {
public:
    npc_teleport_guard() : CreatureScript("npc_teleport_guard") {}
    struct npc_teleport_guardAI : public ScriptedAI {
        npc_teleport_guardAI(Creature* me) : ScriptedAI(me) {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL,     true);
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
        }
        void TeleportPlayerOut(Player* you, const float x, const float y, const float z) {
            you->CastSpell(you,  4801, 0); // teleport effect
            you->CastSpell(you, 51581, 0); // stun spell
            you->TeleportTo(1, x, y, z, you->GetOrientation());
            you->SendAttackSwingCancelAttack();
            you->getHostileRefManager().deleteReferences();
            you->CombatStop();
        }
        void Reset()                      OVERRIDE {}
        void EnterCombat(Unit* /*who*/)   OVERRIDE {}
        void AttackStart(Unit* /*who*/)   OVERRIDE {}
        void UpdateAI(uint32  /*diff*/)   OVERRIDE {}
        void MoveInLineOfSight(Unit* who) OVERRIDE {
            if (!who || !who->IsInWorld() || who->GetZoneId() != 1638)
                return;
            if (!me->IsWithinDist(who, 65.0f, false))
                return;
            Player* you = who->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!you || you->IsGameMaster() || you->IsBeingTeleported())
                return;
            if (you->GetTeam() == HORDE) {
                if (  (me->GetEntry() == NPC_ALLIANCE_LOOK_BACK && me->isInBackInMap(who, 12.0f))
                    || me->GetEntry() == NPC_ALLIANCE_LOOK_ARROUND)
                    TeleportPlayerOut(you, A_X, A_Y, A_Z);
            }
            else {
                if (  (me->GetEntry() == NPC_HORDE_LOOK_BACK && me->isInBackInMap(who, 12.0f))
                    || me->GetEntry() == NPC_HORDE_LOOK_ARROUND)
                    TeleportPlayerOut(you, H_X, H_Y, H_Z);
            }
            me->SetOrientation(me->GetHomePosition().GetOrientation());
            return;
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE {
        return new npc_teleport_guardAI(creature);
    }
};

void AddSC_cyclone_customs() 
{
    new title_vendor();
    new title_npc();
    new squirel_pew_pew();
    new profession_npc();
    new suffix_npc();
    new npc_teleport_guard();
    new npc_teleport_donation();
}
