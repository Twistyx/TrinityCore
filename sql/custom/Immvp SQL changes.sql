--Immvp SQL changes.

--Loots for Chest Event.
INSERT INTO `gameobject_loot_template` VALUES 
(700000, 3345, 0, 1, 1, 1, 1),
(700000, 14246, 0, 1, 1, 1, 1),
(700000, 25681, 0, 1, 1, 1, 1),
(700000, 25683, 0, 1, 1, 1, 1),
(700000, 32235, 0, 1, 1, 1, 1),
(700000, 32241, 0, 1, 1, 1, 1),
(700000, 33453, 0, 1, 1, 1, 1),
(700000, 33677, 0, 1, 1, 1, 1),
(700000, 33701, 0, 1, 1, 1, 1),
(700000, 34333, 0, 1, 1, 1, 1),
(700000, 34404, 0, 1, 1, 1, 1),
(700000, 34405, 0, 1, 1, 1, 1),
(700000, 35033, 0, 1, 1, 1, 1),
(700000, 38506, 0, 1, 1, 1, 1),
(700000, 45497, 0, 1, 1, 1, 1),
(700000, 45998, 0, 1, 1, 1, 1),
(700000, 46140, 0, 1, 1, 1, 1),
(700000, 50807, 0, 1, 1, 1, 1),
(700000, 51470, 0, 1, 1, 1, 1),
(700000, 51543, 0.5, 1, 2, 1, 1),
(700000, 51837, 0, 1, 1, 1, 1);


--Loots updated for World Boss "The Awakened Chaos"
REPLACE INTO `creature_loot_template` (`entry`, `item`, `chanceorquestchance`, `lootmode`, `groupid`, `mincountorref`, `maxcount`)
VALUES (700000, 14246, 0, 1, 0, 1, 1);

--Prepare for Hat transmog.
UPDATE item_template
SET WDBVerified=StatsCount, StatsCount=0, bonding=1, Quality=3, Flags=4096, FlagsExtra=0, BuyCount=1,
    maxcount=0, requiredhonorrank=0, RequiredReputationFaction=0, RequiredCityRank=0, RequiredSkill=0, itemset=0,
    RequiredSkillRank=0, armor=15, RequiredLevel=19, ItemLevel=25, holy_res=0, arcane_res=0, frost_res=0, shadow_res=0, nature_res=0, fire_res=0,
    duration=0, area=0, startquest=0, RandomSuffix=0, RandomProperty=0, ArmorDamageModifier=0, ScriptName='', HolidayId=0, subclass=1,
    socketBonus=0, socketContent_1=0, socketContent_2=0, socketContent_3=0, socketColor_3=0, socketColor_2=0, socketColor_1=0, GemProperties=0,
    spellid_1=0, spellid_2=0, spellid_3=0, spellid_4=0, spellid_5=0 
WHERE InventoryType=1 AND entry NOT IN (19972, 4373, 4385);

--Removed Class Requirements on all Hats.
UPDATE item_template SET AllowableClass = -1 WHERE InventoryType = 1;

--All mounts requires level 19.
UPDATE item_template SET RequiredSkill=0, RequiredLevel=19, RequiredSkillRank=0, RequiredReputationRank=0, RequiredReputationFaction=0, Bonding=1, FlagsExtra=1, AllowableClass=-1, AllowableRace=-1 WHERE RequiredSkill=762;


--Added World Boss "The Awakened Chaos"
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`)
VALUES (700000, 0, 0, 0, 0, 0, 21322, 0, 0, 0, 'The Awakened Chaos', '', '', 0, 19, 19, 0, 14, 14, 0, 1, 1.1, 0.2, 3, 260, 540, 0, 0, 1, 1600, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 700000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 1, 1, 0, 389.755, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 938442751, 20, 'npc_immvp_boss', 0),


--Added NPC "Cairne Bloodhoof"
(700002, 0, 0, 0, 0, 0, 4307, 0, 0, 0, 'Cairne Bloodhoof', 'High Chieftain', '', 0, 19, 19, 2, 35, 35, 3, 1, 1.14286, 1, 3, 0, 0, 0, 0, 1, 0, 0, 0, 164614, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 23050, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),


--Added World Boss Quartermaster NPC "Giorgio Adorno"
(700001, 0, 0, 0, 0, 0, 2347, 0, 0, 0, 'Giorgio Adorno', 'World Boss Quartermaster', '', 0, 19, 19, 0, 35, 35, 129, 1, 1.14286, 1.5, 2, 0, 0, 0, 0, 1, 0, 0, 0, 164614, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1);


--Added World Boss Daily Quest
DELETE FROM `creature_queststarter` WHERE `quest` = 700000;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (700002, 700000);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 700002;
DELETE FROM `creature_questender` WHERE `quest` = 700000;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (700002, 700000);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=700002;
REPLACE INTO `quest_template` (`Id`, `Method`, `Level`, `MinLevel`, `MaxLevel`, `ZoneOrSort`, `Type`, `SuggestedPlayers`, `LimitTime`, `RequiredClasses`, `RequiredRaces`, `RequiredSkillId`, `RequiredSkillPoints`, `RequiredFactionId1`, `RequiredFactionId2`, `RequiredFactionValue1`, `RequiredFactionValue2`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `PrevQuestId`, `NextQuestId`, `ExclusiveGroup`, `NextQuestIdChain`, `RewardXPId`, `RewardOrRequiredMoney`, `RewardMoneyMaxLevel`, `RewardSpell`, `RewardSpellCast`, `RewardHonor`, `RewardHonorMultiplier`, `RewardMailTemplateId`, `RewardMailDelay`, `SourceItemId`, `SourceItemCount`, `SourceSpellId`, `Flags`, `SpecialFlags`, `RewardTitleId`, `RequiredPlayerKills`, `RewardTalents`, `RewardArenaPoints`, `RewardItemId1`, `RewardItemId2`, `RewardItemId3`, `RewardItemId4`, `RewardItemCount1`, `RewardItemCount2`, `RewardItemCount3`, `RewardItemCount4`, `RewardChoiceItemId1`, `RewardChoiceItemId2`, `RewardChoiceItemId3`, `RewardChoiceItemId4`, `RewardChoiceItemId5`, `RewardChoiceItemId6`, `RewardChoiceItemCount1`, `RewardChoiceItemCount2`, `RewardChoiceItemCount3`, `RewardChoiceItemCount4`, `RewardChoiceItemCount5`, `RewardChoiceItemCount6`, `RewardFactionId1`, `RewardFactionId2`, `RewardFactionId3`, `RewardFactionId4`, `RewardFactionId5`, `RewardFactionValueId1`, `RewardFactionValueId2`, `RewardFactionValueId3`, `RewardFactionValueId4`, `RewardFactionValueId5`, `RewardFactionValueIdOverride1`, `RewardFactionValueIdOverride2`, `RewardFactionValueIdOverride3`, `RewardFactionValueIdOverride4`, `RewardFactionValueIdOverride5`, `PointMapId`, `PointX`, `PointY`, `PointOption`, `Title`, `Objectives`, `Details`, `EndText`, `OfferRewardText`, `RequestItemsText`, `CompletedText`, `RequiredNpcOrGo1`, `RequiredNpcOrGo2`, `RequiredNpcOrGo3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount1`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGoCount3`, `RequiredNpcOrGoCount4`, `RequiredSourceItemId1`, `RequiredSourceItemId2`, `RequiredSourceItemId3`, `RequiredSourceItemId4`, `RequiredSourceItemCount1`, `RequiredSourceItemCount2`, `RequiredSourceItemCount3`, `RequiredSourceItemCount4`, `RequiredItemId1`, `RequiredItemId2`, `RequiredItemId3`, `RequiredItemId4`, `RequiredItemId5`, `RequiredItemId6`, `RequiredItemCount1`, `RequiredItemCount2`, `RequiredItemCount3`, `RequiredItemCount4`, `RequiredItemCount5`, `RequiredItemCount6`, `Unknown0`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`, `DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4`, `EmoteOnIncomplete`, `EmoteOnComplete`, `OfferRewardEmote1`, `OfferRewardEmote2`, `OfferRewardEmote3`, `OfferRewardEmote4`, `OfferRewardEmoteDelay1`, `OfferRewardEmoteDelay2`, `OfferRewardEmoteDelay3`, `OfferRewardEmoteDelay4`, `WDBVerified`) 
VALUES (700000, 2, 19, 19, 19, 0, 62, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 1, 0, 0, 0, 0, 0, 4096, 0, 0, 0, 0, 1500, 23247, 41596, 40752, 0, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'It has Awakened..', 'Go to the Fire place outside of The Elder Rise and The Hunter Rise and kill The Awakened Chaos.', 'Welcome $n. The Chaos has awakened once again, and we need your help. $B$BIt\'s been a while since last time he was here. But the great Pirate Warlocks have somehow summoned him back into our world. You to destroy him $n!$BGather your friends, allies and foes. If we do not destroy The Chaos, the whole world will be gone!', '', 'Thank you great Warrior. I hope you\'ll be back to save us, once again.', 'Go now $n! Save us all!', 'Return to Cairne Bloodhoof.', 700000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'The Awakened Chaos Slain', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
