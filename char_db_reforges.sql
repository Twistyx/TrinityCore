CREATE TABLE `custom_reforges` (
	`GUID` INT(10) UNSIGNED NOT NULL COMMENT 'item guid low',
	`increase` INT(10) UNSIGNED NOT NULL COMMENT 'stat_type',
	`decrease` INT(10) UNSIGNED NOT NULL COMMENT 'stat_type',
	`stat_value` INT(10) NOT NULL DEFAULT '0',
	PRIMARY KEY (`GUID`)
)
COMMENT='version 0.1'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
ROW_FORMAT=COMPACT;
