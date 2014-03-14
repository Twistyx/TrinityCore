-- Make all glyphs BoP
UPDATE `item_template`  SET bonding=1 WHERE `name` LIKE '%Glyph of%' AND `class` = '16';

-- Give Totem of the Earthen Ring on character creation for shamans
INSERT INTO `playercreateinfo_item` (`class`, `itemid`) VALUES ('64', '46978');

-- Added Taunt to warriors and maul to druids (base spell)
INSERT INTO `playercreateinfo_spell` (`classmask`, `Spell`, `Note`) VALUES ('1', '355', 'Taunt'), ('1024', '6808', 'Maul');
