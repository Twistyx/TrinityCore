Modifier	 			 DPS Multiplier
Two Hand	 			 1.305 // inventoryType=17
Ranged (bow, gun etc.)	 1.275 // inventoryType IN (15, 26) AND subclass IN (2, 3)
Wand	 	 			 1.522 // inventoryType=26 AND subclass=19
Thrown					 1.3   // inventoryType=25
All 1Hands               1     // inventoryType IN (13, 14, 21)

ilvl 01 = 04
ilvl 02 = 04,5
ilvl 03 = 05
ilvl 04 = 05,5
ilvl 05 = 06
ilvl 06 = 06,5
ilvl 07 = 07
ilvl 08 = 07,5
ilvl 09 = 08
ilvl 10 = 08,5
ilvl 11 = 09
ilvl 12 = 09,5
ilvl 13 = 10
ilvl 14 = 10,5
ilvl 15 = 11
ilvl 16 = 11,5
ilvl 17 = 12
ilvl 18 = 12,5
ilvl 19 = 13
ilvl 20 = 13,5
ilvl 21 = 14
ilvl 22 = 14,5
ilvl 23 = 15
ilvl 24 = 15,5
ilvl 25 = 16
ilvl 26 = 16,5
ilvl 27 = 17
ilvl 28 = 17,5

if (ilvl < 28)
	(4 + (ilvl - 1 * 0.5))

min - max delay dps
 50 - 100 2500 (30)


SELECT AVG(((((dmg_min1 + dmg_max1)) / (delay / 1000))/2)) AS dmgdmgdmgdmddmg FROM item_template AS i
WHERE Quality=3 AND itemLevel=24 AND inventoryType IN (17) AND delay>0

