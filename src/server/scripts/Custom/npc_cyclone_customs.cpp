#include "ScriptPCH.h"
#include "ItemEnchantmentMgr.h"
#include "Language.h"

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
    CUSTOM_OPTION_UNLEARN       = 21,
    CUSTOM_OPTION_EXIT          = 22,
    CUSTOM_OPTION_TITLE_PVP     = 23,
    CUSTOM_OPTION_TITLE_CLIMB   = 24,
    CUSTOM_OPTION_SPRINT        = 25,
    GOSSIP_OPTION_HELLO         = 26,
    CUSTOM_OPTION_ITEM_MENU     = 27,
    CUSTOM_OPTION_ITEM_MENU_P2  = 28,
    CUSTOM_OPTION_ITEM_MENU_P3  = 29,
    CUSTOM_OPTION_ITEM_MENU_P4  = 30,
    CUSTOM_OPTION_ITEM_MENU_P5  = 31,
    CUSTOM_OPTION_ITEM_MENU_P6  = 32,
    CUSTOM_OPTION_ITEM_MENU_P7  = 33,
    CUSTOM_OPTION_ITEM_MENU_MAX = 34,
    CUSTOM_OPTION_SUFFIX        = 35,
    CUSTOM_OPTION_PROPERTY      = 36,
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

class npc_suffix : public CreatureScript 
{
public:
npc_suffix() : CreatureScript("npc_suffix") { }
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

class npc_profession : public CreatureScript 
{
public:
    npc_profession() : CreatureScript("npc_profession") {}

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

class npc_squirel : public CreatureScript 
{
public:
    npc_squirel() : CreatureScript("npc_squirel") {}

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

class go_booty_bay_alarm_bell : public GameObjectScript
{
public:
    go_booty_bay_alarm_bell() : GameObjectScript("go_booty_bay_alarm_bell") { }

    bool OnGossipHello(Player* player, GameObject* go) OVERRIDE
    {
        if (!player)
            return true;
        if (player->FindNearestCreature(90061, 5.0f, true))
            return true;

        if (!(sGameEventMgr->GetActiveEventList().find(66) != sGameEventMgr->GetActiveEventList().end()))
            return true;

        //Add check if current time < 8h - 900000ms, or return.
        // Summon trigger
        player->SummonCreature(90061, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), go->GetAngle(player), TEMPSUMMON_TIMED_DESPAWN, 900000);

        // Play sound to all
        WorldPacket data(SMSG_PLAY_SOUND, 4);
        data << uint32(9154) << player->GetGUID();
        sWorld->SendGlobalMessage(&data);

        player->TeleportTo(0, -14354.72460f, 417.546265f, 41.029716f, player->GetOrientation());
        return true;
    }
};

class npc_bell_trigger : public CreatureScript
{
public:
    npc_bell_trigger() : CreatureScript("npc_bell_trigger") { }

    struct npc_bell_triggerAI : public ScriptedAI
    {
        npc_bell_triggerAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() OVERRIDE
        {
            alarmTimer = 150;
            firstTime = true;
        }

        void JustRespawned() OVERRIDE
        {
            Reset();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (alarmTimer <= diff)
            {
                alarmTimer = 600000;
                GameObject* bell = GetClosestGameObjectWithEntry(me, 175948, 0.5f);
                if (bell)
                {
                    bell->SendCustomAnim(0);
                    bell->PlayDirectSound(9154, 0);
                    if (!firstTime)
                        sGameEventMgr->StartEvent(66, true);
                    else
                    {
                        sGameEventMgr->StopEvent(66, true);
                        DoCastAOE(51490, true);
                        firstTime = false;
                    }
                }   
            }
            else
                alarmTimer -= diff;
        }

    private:
        uint32 alarmTimer;
        bool firstTime;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_bell_triggerAI(creature);
    }
};

class npc_ethereum : public CreatureScript
{
public:
    npc_ethereum() : CreatureScript("npc_ethereum") { }

    struct npc_ethereumAI : public ScriptedAI
    {
        npc_ethereumAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() OVERRIDE
        {
            buffTimer = 1500;
        }

        void JustRespawned() OVERRIDE
        {
            Reset();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (buffTimer <= diff)
            {
                buffTimer = 35000;
                me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
            }
            else
                buffTimer -= diff;
        }

    private:
        uint32 buffTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_ethereumAI(creature);
    }
};

class npc_gamon : public CreatureScript
{
public:
    npc_gamon() : CreatureScript("npc_gamon") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_gamonAI(creature);
    }

    struct npc_gamonAI : public ScriptedAI
    {
        npc_gamonAI(Creature* creature) : ScriptedAI(creature) {}

        void UpdateAI(uint32 diff) OVERRIDE
        {
        }

        void ReceiveEmote(Player* player, uint32 emote) OVERRIDE
        {
            switch (emote)
            {
                case TEXT_EMOTE_DANCE:
                    EnterEvadeMode();
                    break;
                case TEXT_EMOTE_RUDE:
                case TEXT_EMOTE_CHICKEN:
                    if (me->IsWithinDistInMap(player, 5)) {
                        player->CastSpell(player, 50085, 0); // Spell Slow Fall
                        DoCast(player, 11027, 0); // Spell kick
                    }
                    else
                        me->HandleEmoteCommand(EMOTE_ONESHOT_RUDE);
                    break;
                case TEXT_EMOTE_WAVE:
                    if (me->IsWithinDistInMap(player, 5))
                        DoCast(player, 6754, false);
                    else
                        me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                    break;
                case TEXT_EMOTE_BOW:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                    break;
                case TEXT_EMOTE_KISS:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_FLEX);
                    break;
            }
        }
    };
};


class npc_title_vendor : public CreatureScript 
{
public:
    npc_title_vendor() : CreatureScript("npc_title_vendor") {}

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

class npc_title_giver : public CreatureScript 
{
public:
    npc_title_giver() : CreatureScript("npc_title_giver") {}

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

#define GOSSIP_OPTION_HELLO_MENU 300
#define GOSSIP_OPTION_SHOWRACIALS 600
#define GOSSIP_OPTION_LEARNRACIALS 900
class npc_talent : public CreatureScript
{
public:
    npc_talent() : CreatureScript("npc_talent"){ }
 
    typedef struct  s_racialSpell
    {
        uint16      id;
        std::string iconName;
    }               racialSpell;

    void Human(bool learn, Player* player)
    {
        const uint16 spells[6] = { 59752, 20599, 20598, 58985, 20597, 20864 };

        if (learn)
        {
            learnRacials(spells, player);
        }
        else
        {
            const char *icons[6] = { "spell_shadow_charm", "inv_misc_note_02", "inv_enchant_shardbrilliantsmall", "spell_nature_sleep", "ability_meleedamage", "inv_hammer_05" };
            showDetails(spells, icons, player, RACE_HUMAN);
        }
    }
    void Orc(bool learn, Player* player)
    {
        const uint16 spells[6] = { 20575, 33697, 20573, 20574, 0, 0 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "ability_warrior_warcry", "racial_orc_berserkerstrength", "inv_helmet_23", "inv_axe_02", "", "" };
            showDetails(spells, icons, player, RACE_ORC);
        }        
    }
    void Dwarf(bool learn, Player* player)
    {
        const uint16 spells[6] = { 2481, 20594, 20596, 20595, 59224 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "racial_dwarf_findtreasure", "spell_shadow_unholystrength", "spell_frost_wizardmark", "inv_musket_03", "inv_hammer_05", "" };
            showDetails(spells, icons, player, RACE_DWARF);
        }        
    }
    void Nightelf(bool learn, Player* player)
    {
        const uint16 spells[6] = { 58984, 20585, 21009, 20582, 20551 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "ability_ambush", "spell_nature_wispsplode", "ability_racial_ultravision", "ability_racial_shadowmeld", "spell_nature_spiritarmor", "" };
            showDetails(spells, icons, player, RACE_NIGHTELF);
        }        
    }
    void Undead(bool learn, Player* player)
    {
        const uint16 spells[6] = { 20577, 20579, 5227, 7744, 0, 0 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "ability_racial_cannibalize", "spell_shadow_detectinvisibility", "spell_shadow_demonbreath", "spell_shadow_raisedead", "", "" };
            showDetails(spells, icons, player, RACE_UNDEAD_PLAYER);
        }        
    }
    void Tauren(bool learn, Player* player)
    {
        const uint16 spells[6] = { 20549, 20552, 20550, 20551, 0, 0 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "ability_warstomp", "inv_misc_flower_01", "spell_nature_unyeildingstamina", "spell_nature_spiritarmor", "", "" };
            showDetails(spells, icons, player, RACE_TAUREN);
        }        
    }
    void Gnome(bool learn, Player* player)
    {
        const uint16 spells[6] = { 20589, 20591, 20592, 20593, 0, 0 };

        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "ability_rogue_trip", "inv_enchant_essenceeternallarge", "spell_nature_wispsplode", "inv_misc_gear_01", "", "" };
            showDetails(spells, icons, player, RACE_GNOME);
        }        
    }
    void Troll(bool learn, Player* player)
    {
        const uint16 spells[6] = { 26297, 58943, 20555, 26290, 20558, 20557 };
        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "racial_troll_berserk", "inv_misc_idol_02", "spell_nature_regenerate", "inv_weapon_bow_12", "inv_throwingaxe_03", "inv_misc_pelt_bear_ruin_02" };
            showDetails(spells, icons, player, RACE_TROLL);
        }
        
    }
    void Draenai(bool learn, Player* player)
    {
        const uint16 spells[6] = { 59542, 28878, 20579, 0, 0, 0 };
        if (learn)
            learnRacials(spells, player);
        else
        {
            const char *icons[6] = { "spell_holy_holyprotection", "inv_helmet_21", "spell_shadow_detectinvisibility", "", "", "" };
            showDetails(spells, icons, player, RACE_DRAENEI);
        }
    }
    void Bloodelf(bool learn, bool rogue, Player* player)
    {
        const uint16 spells[6] = { 28730, 822, 0, 0, 0, 0 };
        const uint16 spellsRogue[6] = { 25046, 822, 0, 0, 0, 0 };
        if (learn)
            learnRacials((rogue) ? spellsRogue : spells, player);
        else
        {
            const char *icons[6] = { "spell_shadow_teleport", "spell_shadow_antimagicshell", "", "", "", "" };
            showDetails((rogue) ? spellsRogue : spells, icons, player, RACE_BLOODELF);
        }
    }

    void learnRacials(const uint16 spells[6], Player* player)
    {
        int i = 6;
        while (i)
        {
            i--;
            if (spells[i])
                player->learnSpell(spells[i], false);
        }
        player->learnSpell(81, false);
    }

    void showDetails(const uint16 spells[6], const char *icons[6], Player* player, uint32 race)
    {
        int i = 6;
        while (i)
        {
            i--;
            if (spells[i])
            {
                std::ostringstream ss;
                ss << "|TInterface/ICONS/" << icons[i] << ":24:24:0:0|t";
                ss << sSpellMgr->GetSpellInfo(spells[i])->SpellName[0];
                if (spells[i] == 59542)
                    ss << " (Nerfed by 50%)";
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, ss.str().c_str(), race, GOSSIP_OPTION_HELLO_MENU);
            }
        }
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Learn those racials !", race, GOSSIP_OPTION_LEARNRACIALS, "Are you sure you can't go back !", 0, false);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "...Go back to the list", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_HELLO_MENU);
    }

    void listRaces(Player* player)
    {
        const char *msg[MAX_RACES] =
        {
            "",
            "|TInterface/ICONS/inv_misc_tournaments_banner_human:24:24:0:0|tShow me the Human racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_orc:24:24:0:0|tShow me the Orc racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_dwarf:24:24:0:0|tShow me the Dwarf racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_nightelf:24:24:0:0|tShow me the Nightelf racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_scourge:24:24:0:0|tShow me the Undead racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_tauren:24:24:0:0|tShow me the Tauren racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_gnome:24:24:0:0|tShow me the Gnome racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_troll:24:24:0:0|tShow me the Troll racials",
            "", // gobelins
            "|TInterface/ICONS/inv_misc_tournaments_banner_bloodelf:24:24:0:0|tShow me the Bloodelf racials",
            "|TInterface/ICONS/inv_misc_tournaments_banner_draenei:24:24:0:0|tShow me the Draenei racials"
        };
        for (uint8 i = RACE_NONE; i < MAX_RACES; ++i)
        {
            if (sObjectMgr->GetPlayerInfo(i, player->getClass()))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, msg[i], i, GOSSIP_OPTION_SHOWRACIALS);
        }
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (!player)
            return false;
        if (!creature)
            return false;

        if (!player->HasSpell(81))
            listRaces(player);
        else
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());
            if (creature->IsTrainer())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Train me !", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);
            if (player->GetSpecsCount() == 1)
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Learn Dual Spec", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_LEARNDUALSPEC, "Are you sure you want to pay this much ?", 100000, false);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Reset my talents.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_UNLEARNTALENTS);
        }
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 data, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_TRAIN:
                player->GetSession()->SendTrainerList(creature->GetGUID());
                break;
            case GOSSIP_OPTION_UNLEARNTALENTS:
                player->CLOSE_GOSSIP_MENU();
                player->SendTalentWipeConfirm(creature->GetGUID());
                break;
            case GOSSIP_OPTION_LEARNDUALSPEC:
                if (player->GetSpecsCount() == 1)
                {
                    if (!player->HasEnoughMoney(100000))
                    {
                        player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                        player->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    else
                    {
                        player->ModifyMoney(-100000);

                        // Cast spells that teach dual spec
                        // Both are also ImplicitTarget self and must be cast by player
                        player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
                        player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());

                        // Should show another Gossip text with "Congratulations..."
                        player->PlayerTalkClass->SendCloseGossip();
                    }
                }
                break;
            case GOSSIP_OPTION_HELLO_MENU:
            {
                OnGossipHello(player, creature);
                break;
            }
            case GOSSIP_OPTION_SHOWRACIALS:
            case GOSSIP_OPTION_LEARNRACIALS:
            {
                switch (data)
                {
                    case RACE_HUMAN:
                        Human((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_ORC:
                        Orc((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_DWARF:
                        Dwarf((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_NIGHTELF:
                        Nightelf((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_UNDEAD_PLAYER:
                        Undead((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_TAUREN:
                        Tauren((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_GNOME:
                        Gnome((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_TROLL:
                        Troll((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                    case RACE_BLOODELF:
                        Bloodelf((action == GOSSIP_OPTION_LEARNRACIALS), (player->getClass() == CLASS_ROGUE), player);
                        break;
                    case RACE_DRAENEI:
                        Draenai((action == GOSSIP_OPTION_LEARNRACIALS), player);
                        break;
                }
                if (action != GOSSIP_OPTION_LEARNRACIALS)
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                else
                {

                    player->PlayerTalkClass->SendCloseGossip();
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
            }
        }
        return true;
    }
};
void AddSC_cyclone_customs() 
{
    new go_booty_bay_alarm_bell();
    new npc_talent();
    new npc_title_vendor();
    new npc_title_giver();
    new npc_squirel();
    new npc_gamon();
    new npc_ethereum();
    new npc_bell_trigger();
    new npc_profession();
    new npc_suffix();
    new npc_teleport_guard();
    new npc_teleport_donation();
}
