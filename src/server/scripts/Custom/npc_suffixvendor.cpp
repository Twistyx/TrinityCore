//	
//void npc_suffixvendor()
//{
//    class npc_suffix : public CreatureScript
//    {
//    public:
//        npc_suffix() : CreatureScript("npc_suffixvendor") { }
//     
//            bool OnGossipHello(Player *player, Creature *creature)
//            {  
//                    player->PlayerTalkClass->ClearMenus();
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_TUMCLOAK, 900, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_WRANGWRIST, 901, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SHIMBRACER, 902, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FORTB, 1076, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SCOUTGLOVE, 903, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_DEFENDG, 911, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EARTHGIRDLE, 904, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_VIGBELT, 905, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ENUMWRAP, 909, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SCOUTTROU, 906, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_GRUNTL, 910, 0);
//                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_NEXT, 2000, 0);
//                    player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//     
//                    return true;
//            }
//     
//            bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 uiAction)
//            {
//                    ItemPosCountVec dest;
//                    uint8 msg;
//                    uint32 itemId = 0;
//                    uint32 suffixId = 0;
//                    switch(sender)
//                    {
//     
//                    case 900:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1000, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1001, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BEAR, 1002, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BANDIT, 1056, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SOLDIER, 1057, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_CHAMPION, 1058, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ELDER, 1059, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MOON, 1060, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SORCERER, 1061, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 901:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_AGI, 1003, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1004, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_INT, 1005, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_HEAL, 1006, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_POWER, 1007, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1008, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FALCON, 1009, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1010, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_OWL, 1011, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_WHALE, 1012, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_NAT, 1013, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ARC, 1014, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 902:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1015, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_INT, 1016, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SPI, 1017, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_HEAL, 1018, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1019, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_OWL, 1020, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_WHALE, 1021, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FIRE, 1022, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FROST, 1023, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ARC, 1024, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SHAD, 1025, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 903:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SPI, 1026, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_HEAL, 1027, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_DEF, 1028, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1029, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BEAR, 1030, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1031, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FALCON, 1032, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_TIGER, 1033, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_WHALE, 1034, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_OWL, 1035, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_GORILLA, 1036, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ARC, 1037, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 904:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STR, 1038, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1039, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BEAR, 1040, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SOLDIER, 1062, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_CHAMPION, 1063, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SORCERER, 1064, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 905:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1041, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BANDIT, 1065, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_VISION, 1066, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MOON, 1067, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 906:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ARC, 1042, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_HEAL, 1068, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_POWER, 1043, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FALCON, 1044, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1045, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_WHALE, 1046, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 907:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1047, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 908:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FIRER, 1048, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_FROSTR, 1049, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_NATR, 1050, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ARCR, 1051, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SHADR, 1052, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 909:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_STAM, 1069, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1070, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SORCERER, 1071, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_VISION, 1072, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 910:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1502, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1503, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//                           
//     
//                    case 911:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_EAGLE, 1500, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MON, 1501, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//                   
//                    case 912:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_SUN, 1074, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_MOON, 1075, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 913:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_HEAL, 1077, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_TIGER, 1078, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//     
//                    case 2000:
//                            player->PlayerTalkClass->ClearMenus();
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_GREENSAND, 907, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_ZIRCBAND, 908, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_BANSHEE, 912, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_GRUNTS, 1073, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_DEFENDERB, 913, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_DEFENDERL, 1504, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_DEFENDERBO, 1505, 0);
//                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, T_PREV, 2001, 0);
//                            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
//                            break;
//     
//                    case 2001:
//                            OnGossipHello(player, creature);
//                            break;
//     
//                    case 1000:
//                            itemId = TUMCLOAK;
//                            suffixId = A_STAM;
//                            break;
//     
//                    case 1001:
//                            itemId = TUMCLOAK;
//                            suffixId = A_MON;
//                            break;
//     
//                    case 1002:
//                            itemId = TUMCLOAK;
//                            suffixId = A_BEAR;
//                            break;
//     
//                    case 1003:
//                            itemId = WRANGWRIST;
//                            suffixId = B_AGI;
//                            break;
//     
//                    case 1004:
//                            itemId = WRANGWRIST;
//                            suffixId = B_STAM;
//                            break;
//     
//                    case 1005:
//                            itemId = WRANGWRIST;
//                            suffixId = B_INT;
//                            break;
//     
//                    case 1006:
//                            itemId = WRANGWRIST;
//                            suffixId = B_HEAL;
//                            break;
//     
//                    case 1007:
//                            itemId = WRANGWRIST;
//                            suffixId = B_POWER;
//                            break;
//     
//                    case 1008:
//                            itemId = WRANGWRIST;
//                            suffixId = B_EAGLE;
//                            break;
//     
//                    case 1009:
//                            itemId = WRANGWRIST;
//                            suffixId = B_FALCON;
//                            break;
//     
//                    case 1010:
//                            itemId = WRANGWRIST;
//                            suffixId = B_MON;
//                            break;
//     
//                    case 1011:
//                            itemId = WRANGWRIST;
//                            suffixId = B_OWL;
//                            break;
//     
//                    case 1012:
//                            itemId = WRANGWRIST;
//                            suffixId = B_WHALE;
//                            break;
//     
//                    case 1013:
//                            itemId = WRANGWRIST;
//                            suffixId = B_NAT;
//                            break;
//     
//                    case 1014:
//                            itemId = WRANGWRIST;
//                            suffixId = B_ARC;
//                            break;
//     
//                    case 1015:
//                            itemId = SHIMBRACER;
//                            suffixId = C_STAM;
//                            break;
//     
//                    case 1016:
//                            itemId = SHIMBRACER;
//                            suffixId = C_INT;
//                            break;
//     
//                    case 1017:
//                            itemId = SHIMBRACER;
//                            suffixId = C_SPI;
//                            break;
//     
//                    case 1018:
//                            itemId = SHIMBRACER;
//                            suffixId = C_HEAL;
//                            break;
//     
//                    case 1019:
//                            itemId = SHIMBRACER;
//                            suffixId = C_EAGLE;
//                            break;
//     
//                    case 1020:
//                            itemId = SHIMBRACER;
//                            suffixId = C_OWL;
//                            break;
//     
//                    case 1021:
//                            itemId = SHIMBRACER;
//                            suffixId = C_WHALE;
//                            break;
//     
//                    case 1022:
//                            itemId = SHIMBRACER;
//                            suffixId = C_FIRE;
//                            break;
//     
//                    case 1023:
//                            itemId = SHIMBRACER;
//                            suffixId = C_FROST;
//                            break;
//     
//                    case 1024:
//                            itemId = SHIMBRACER;
//                            suffixId = C_ARC;
//                            break;
//     
//                    case 1025:
//                            itemId = SHIMBRACER;
//                            suffixId = C_SHAD;
//                            break;
//     
//                    case 1026:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_SPI;
//                            break;
//     
//                    case 1027:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_HEAL;
//                            break;
//     
//                    case 1028:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_DEF;
//                            break;
//     
//                    case 1029:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_MON;
//                            break;
//     
//                    case 1030:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_BEAR;
//                            break;
//     
//                    case 1031:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_EAGLE;
//                            break;
//     
//                    case 1032:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_FALC;
//                            break;
//     
//                    case 1033:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_TIGER;
//                            break;
//     
//                    case 1034:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_WHALE;
//                            break;
//     
//                    case 1035:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_OWL;
//                            break;
//     
//                    case 1036:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_GOR;
//                            break;
//     
//                    case 1037:
//                            itemId = SCOUTGLOVE;
//                            suffixId = D_ARC;
//                            break;
//     
//                    case 1038:
//                            itemId = EARTHGIRDLE;
//                            suffixId = E_STR;
//                            break;
//     
//                    case 1039:
//                            itemId = EARTHGIRDLE;
//                            suffixId = E_STAM;
//                            break;
//     
//                    case 1040:
//                            itemId = EARTHGIRDLE;
//                            suffixId = E_BEAR;
//                            break;
//     
//                    case 1041:
//                            itemId = VIGBELT;
//                            suffixId = F_MON;
//                            break;
//     
//                    case 1042:
//                            itemId = SCOUTTROU;
//                            suffixId = G_ARC;
//                            break;
//     
//                    case 1068:
//                            itemId = SCOUTTROU;
//                            suffixId = G_HEAL;
//                            break;
//     
//                    case 1043:
//                            itemId = SCOUTTROU;
//                            suffixId = G_POWER;
//                            break;
//     
//                    case 1044:
//                            itemId = SCOUTTROU;
//                            suffixId = G_FALCON;
//                            break;
//     
//                    case 1045:
//                            itemId = SCOUTTROU;
//                            suffixId = G_EAGLE;
//                            break;
//     
//                    case 1046:
//                            itemId = SCOUTTROU;
//                            suffixId = G_WHALE;
//                            break;
//     
//                    case 1047:
//                            itemId = GREENSAND;
//                            suffixId = H_STAM;
//                            break;
//     
//                    case 1048:
//                            itemId = ZIRCBAND;
//                            suffixId = I_FIRER;
//                            break;
//     
//                    case 1049:
//                            itemId = ZIRCBAND;
//                            suffixId = I_FROSTR;
//                            break;
//     
//                    case 1050:
//                            itemId = ZIRCBAND;
//                            suffixId = I_NATR;
//                            break;
//     
//                    case 1051:
//                            itemId = ZIRCBAND;
//                            suffixId = I_ARCR;
//                            break;
//     
//                    case 1052:
//                            itemId = ZIRCBAND;
//                            suffixId = I_SHADR;
//                            break;
//     
//                    case 1056:
//                            itemId = TUMCLOAK;
//                            suffixId = BANDIT;
//                            break;
//     
//                    case 1057:
//                            itemId = TUMCLOAK;
//                            suffixId = SOLDIER;
//                            break;
//     
//                    case 1058:
//                            itemId = TUMCLOAK;
//                            suffixId = CHAMPION;
//                            break;
//     
//                    case 1059:
//                            itemId = TUMCLOAK;
//                            suffixId = ELDER;
//                            break;
//     
//                    case 1060:
//                            itemId = TUMCLOAK;
//                            suffixId = MOON;
//                            break;
//     
//                    case 1061:
//                            itemId = TUMCLOAK;
//                            suffixId = SORCERER;
//                            break;
//     
//                    case 1062:
//                            itemId = EARTHGIRDLE;
//                            suffixId = SOLDIER;
//                            break;
//     
//                    case 1063:
//                            itemId = EARTHGIRDLE;
//                            suffixId = CHAMPION;
//                            break;
//     
//                    case 1064:
//                            itemId = EARTHGIRDLE;
//                            suffixId = SORCERER;
//                            break;
//     
//                    case 1065:
//                            itemId = VIGBELT;
//                            suffixId = BANDIT;
//                            break;
//     
//                    case 1066:
//                            itemId = VIGBELT;
//                            suffixId = VISION;
//                            break;
//     
//                    case 1067:
//                            itemId = VIGBELT;
//                            suffixId = MOON;
//                            break;
//     
//                    case 1069:
//                            itemId = ENUMWRAP;
//                            suffixId = 186;
//                            break;
//     
//                    case 1070:
//                            itemId = ENUMWRAP;
//                            suffixId = 854;
//                            break;
//     
//                    case 1071:
//                            itemId = ENUMWRAP;
//                            suffixId = SORCERER;
//                            break;
//     
//                    case 1072:
//                            itemId = ENUMWRAP;
//                            suffixId = VISION;
//                            break;
//     
//                    case 1073:
//                            itemId = GRUNTS;
//                            suffixId = BLOCK;
//                            break;
//     
//                    case 1074:
//                            itemId = BANSHEE;
//                            suffixId = SUN;
//                            break;
//     
//                    case 1075:
//                            itemId = BANSHEE;
//                            suffixId = MOON;
//                            break;
//     
//                    case 1076:
//                            itemId = FORTB;
//                            suffixId = B_EAGLE;
//                            break;
//     
//                    case 1077:
//                            itemId = DEFENDERB;
//                            suffixId = 2031;
//                            break;
//     
//                    case 1078:
//                            itemId = DEFENDERB;
//                            suffixId = 675;
//                            break;
//     
//     
//     
//                    case 1500:
//                            itemId = DEFENDG;
//                            suffixId = D_EAGLE;
//                            break;
//     
//                    case 1501:
//                            itemId = DEFENDG;
//                            suffixId = D_MON;
//                            break;
//     
//                    case 1502:
//                            itemId = GRUNTL;
//                            suffixId = 851;
//                            break;
//     
//                    case 1503:
//                            itemId = GRUNTL;
//                            suffixId = 596;
//                            break;
//                   
//                    case 1504:
//                            itemId = DEFENDERL;
//                            suffixId = 2034;
//                            break;
//                   
//                    case 1505:
//                            itemId = DEFENDERBO;
//                            suffixId = 848;
//                            break;
//     
//     
//                    }
//                    if (itemId > 0)
//                    {
//                            msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1, false);
//                            if (msg == EQUIP_ERR_OK)
//                            {
//                                    Item * newitem = player->StoreNewItem(dest, itemId, true, suffixId);
//                                    player->SendNewItem(newitem, 1, true, false);
//                                    player->CLOSE_GOSSIP_MENU();
//                            }
//                    }
//                    return true;
//            }
//    };
//
//
void AddSC_npc_suffixvendor()
{
   //     new npc_suffixvendor();
}


