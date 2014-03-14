-- Make all glyphs BoP
UPDATE `item_template`  SET bonding=1 WHERE `name` LIKE '%Glyph of%' AND `class` = '16';

-- Give Totem of the Earthen Ring on character creation for shamans
INSERT INTO `playercreateinfo_item` (`class`, `itemid`) VALUES ('64', '46978');
