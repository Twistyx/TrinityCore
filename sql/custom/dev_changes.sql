-- Make all glyphs BoP
UPDATE `item_template`  SET bonding=1 WHERE `name` LIKE '%Glyph of%' AND `class` = '16';

-- Give Totem of the Earthen Ring on character creation for shamans
INSERT INTO `playercreateinfo_item` (`class`, `itemid`) VALUES ('64', '46978');

-- Added Taunt to warriors and maul to druids (base spell)
DELETE FROM playercreateinfo_spell WHERE Spell IN (335, 6808, 50769, 5019);
INSERT INTO `playercreateinfo_spell` (`classmask`, `Spell`, `Note`) VALUES
('1', '355', 'Taunt'),
('1024', '6808', 'Maul'),
('1024', '50769', 'Revive'),
('400', '5019', 'Wand Skill');

-- Added talent script
UPDATE `creature_template` SET `ScriptName`='npc_talent' AND 'npcflags'=1 WHERE (`entry` IN ('500025', '500066'));

