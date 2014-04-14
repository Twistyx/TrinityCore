REPLACE INTO origins_world.gameobject
(SELECT * FROM dev_world.gameobject AS dev
WHERE id IN (190445,
184949,
191540,
187333));

REPLACE INTO origins_world.pool_gameobject
(SELECT * FROM dev_world.pool_gameobject AS dev
WHERE `description` LIKE '%Custom%');

REPLACE INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES ('170000', '25', 'Max Berry Bush');

REPLACE INTO origins_world.creature_template
(SELECT * FROM dev_world.creature_template AS dev
WHERE entry IN ( 1215, 4609));

DELETE FROM `origins_world.creature_queststarter` WHERE `quest` = 50009;
DELETE FROM `origins_world.gameobject_queststarter` WHERE `quest` = 50009;
UPDATE `origins_world.item_template` SET `StartQuest`=0 WHERE `StartQuest` = 50009;
INSERT INTO `origins_world.creature_queststarter` (`id`, `quest`) VALUES (1215, 50009);
UPDATE `origins_world.creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 1215;
INSERT INTO `origins_world.creature_queststarter` (`id`, `quest`) VALUES (4609, 50009);
UPDATE `origins_world.creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 4609;
DELETE FROM `origins_world.creature_questender` WHERE `quest` = 50009;
DELETE FROM `origins_world.gameobject_questender` WHERE `quest` = 50009;
INSERT INTO `origins_world.creature_questender` (`id`, `quest`) VALUES (1215, 50009);
UPDATE `origins_world.creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=1215;
INSERT INTO `origins_world.creature_questender` (`id`, `quest`) VALUES (4609, 50009);
UPDATE `origins_world.creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=4609;

REPLACE INTO `origins_world.quest_template` (`Id`, `Method`, `Level`, `MinLevel`, `MaxLevel`, `ZoneOrSort`, `Type`, `SuggestedPlayers`, `LimitTime`, `RequiredClasses`, `RequiredRaces`, `RequiredSkillId`, `RequiredSkillPoints`, `RequiredFactionId1`, `RequiredFactionId2`, `RequiredFactionValue1`, `RequiredFactionValue2`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `PrevQuestId`, `NextQuestId`, `ExclusiveGroup`, `NextQuestIdChain`, `RewardXPId`, `RewardOrRequiredMoney`, `RewardMoneyMaxLevel`, `RewardSpell`, `RewardSpellCast`, `RewardHonor`, `RewardHonorMultiplier`, `RewardMailTemplateId`, `RewardMailDelay`, `SourceItemId`, `SourceItemCount`, `SourceSpellId`, `Flags`, `SpecialFlags`, `RewardTitleId`, `RequiredPlayerKills`, `RewardTalents`, `RewardArenaPoints`, `RewardItemId1`, `RewardItemId2`, `RewardItemId3`, `RewardItemId4`, `RewardItemCount1`, `RewardItemCount2`, `RewardItemCount3`, `RewardItemCount4`, `RewardChoiceItemId1`, `RewardChoiceItemId2`, `RewardChoiceItemId3`, `RewardChoiceItemId4`, `RewardChoiceItemId5`, `RewardChoiceItemId6`, `RewardChoiceItemCount1`, `RewardChoiceItemCount2`, `RewardChoiceItemCount3`, `RewardChoiceItemCount4`, `RewardChoiceItemCount5`, `RewardChoiceItemCount6`, `RewardFactionId1`, `RewardFactionId2`, `RewardFactionId3`, `RewardFactionId4`, `RewardFactionId5`, `RewardFactionValueId1`, `RewardFactionValueId2`, `RewardFactionValueId3`, `RewardFactionValueId4`, `RewardFactionValueId5`, `RewardFactionValueIdOverride1`, `RewardFactionValueIdOverride2`, `RewardFactionValueIdOverride3`, `RewardFactionValueIdOverride4`, `RewardFactionValueIdOverride5`, `PointMapId`, `PointX`, `PointY`, `PointOption`, `Title`, `Objectives`, `Details`, `EndText`, `OfferRewardText`, `RequestItemsText`, `CompletedText`, `RequiredNpcOrGo1`, `RequiredNpcOrGo2`, `RequiredNpcOrGo3`, `RequiredNpcOrGo4`, `RequiredNpcOrGoCount1`, `RequiredNpcOrGoCount2`, `RequiredNpcOrGoCount3`, `RequiredNpcOrGoCount4`, `RequiredSourceItemId1`, `RequiredSourceItemId2`, `RequiredSourceItemId3`, `RequiredSourceItemId4`, `RequiredSourceItemCount1`, `RequiredSourceItemCount2`, `RequiredSourceItemCount3`, `RequiredSourceItemCount4`, `RequiredItemId1`, `RequiredItemId2`, `RequiredItemId3`, `RequiredItemId4`, `RequiredItemId5`, `RequiredItemId6`, `RequiredItemCount1`, `RequiredItemCount2`, `RequiredItemCount3`, `RequiredItemCount4`, `RequiredItemCount5`, `RequiredItemCount6`, `Unknown0`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`, `DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4`, `EmoteOnIncomplete`, `EmoteOnComplete`, `OfferRewardEmote1`, `OfferRewardEmote2`, `OfferRewardEmote3`, `OfferRewardEmote4`, `OfferRewardEmoteDelay1`, `OfferRewardEmoteDelay2`, `OfferRewardEmoteDelay3`, `OfferRewardEmoteDelay4`, `WDBVerified`) VALUES (50009, 2, 19, 19, 19, 215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4232, 1, 0, 0, 0, 0, 20558, 0, 0, 0, 2, 0, 0, 0, 6373, 3390, 6048, 30615, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Berries... Berries Everywhere!', 'Doctor Marsh in Thunder Bluff wants you to gather 10 Bloodberries from the bushes found throughout the city.', 'Ahh, $n, you look like you\'re willing to get your hands a little dirty to pick me some Blood Berries! The bloodberry grows wild in many regions of Kalimdor and has long been the number 1 ingredient to top secret pvp potions. These berries take on special properties that can be distilled and imbibed to magnify one\'s abilites. If you can bring me some samples of these berries from the bushes throughout Thunder Bluff i would reward you greatly!', '', 'Excellent, $n I hope this will aid you in your efforts here at Cyclone.', 'Have you gathered those berries yet?', 'Return to Doctor Marsh in Thunder Bluff.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34502, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12340);
