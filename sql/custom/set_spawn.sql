-- horde
UPDATE `playercreateinfo` SET
`map`='1',
`zone`='1638',
`position_x`='-1473.901733',
`position_y`='-79.326981',
`position_z`='161.101929',
`orientation`='5.776213'
WHERE `race` NOT IN ('1', '3', '4', '7', '11');

-- alliance
UPDATE `playercreateinfo` SET
`map`='1',
`zone`='1638',
`position_x`='-1048.654785',
`position_y`='-304.482635',
`position_z`='159.029907',
`orientation`='1.760865'
WHERE `race` IN ('1', '3', '4', '7', '11');
