--GM level suggestion:
--GM level 1: VIP
--GM level 2: Ticket GM
--GM level 3: Admin
--GM level 4: Owner





--0 = No risk
--1 = Low risk
--2 = very risky
--just change the id to whatever account level ids you have and want

REPLACE_INTO rbac_linked_permissions (`id`,`linkedid`)
VALUES 
(0, 236), --Syntax: .arena info #TeamID
(0, 217), --Syntax: .account
(0, 374), --Syntax: .gm ingame
(0, 442), --Syntax: .lookup $subcommand
(0, 443), --Syntax: .lookup area $namepart
(0, 444), --Syntax: .lookup creature $namepart
(0, 445), --Syntax: .lookup event $name
(0, 446), --Syntax: .lookup faction $name
(0, 447), --Syntax: .lookup item $itemname
(0, 448), --Syntax: .lookup itemset $itemname
(0, 449), --Syntax: .lookup object $objname
(0, 450), --Syntax: .lookup quest $namepart
(0, 455), --Syntax: .lookup skill $$namepart
(0, 456), --Syntax: .lookup spell $namepart
(0, 457), --Syntax: .lookup spell id #spellid
(0, 458), --Syntax: .lookup taxinode $substring
(0, 459), --Syntax: .lookup tele $substring
(0, 460), --Syntax: .lookup title $$namepart
(0, 461), --Syntax: .lookup map $namepart
(0, 237), --Syntax: .arena lookup $name
(0, 505), --Syntax: .gps [$name|$shift-link]
(0, 502), --Syntax: .distance
(2, 726), --Syntax: .server plimit [#num|-1|-2|-3|reset|player|moderator|gamemaster|administrator]
(0, 718), --Syntax: .server $subcommand
(0, 725), --Syntax: .server info
(0, 742), --Syntax: .ticket $subcommand
(0, 743), --Usage: .ticket assign $ticketid $gmname.
(0, 744), --Usage: .ticket close $ticketid.
(0, 745), --Displays a list of closed GM tickets.
(0, 746), --Usage: .ticket comment $ticketid $comment.
(0, 748), --Usage: .ticket delete $ticketid.
(0, 751), --Displays a list of open GM tickets.
(0, 752), --Displays a list of open GM tickets whose owner is online.
(0, 753), --Syntax: .ticket reset
(0, 758), --Syntax: .ticket unassign $ticketid
(0, 759), --Usage: .ticket viewid $ticketid.
(0, 760), --Usage: .ticket viewname $creatorname. 
(0, 600), --Syntax: .npc yell $message
(0, 599), --Syntax: .npc whisper #playerguid #text
(0, 597), --Syntax: .npc say $message
(0, 593), --Syntax: .npc info
(0, 507), --Syntax: .help [$command]
(0, 490), --Syntax: .appear [$charactername]
(0, 485), --Syntax: .send mail #playername "#subject" "#text"
(0, 486), --Syntax: .send message $playername $message
(0, 472), --Syntax: .group $subcommand
(0, 473), --Syntax: .group leader [$characterName]
(0, 476), --Syntax: .group join $AnyCharacterNameFromGroup [$CharacterName] 
(0, 477), --Syntax: .group list [$CharacterName] 
(0, 462), --Syntax: .announce $MessageToBroadcast
(0, 466), --Syntax: .gmannounce $announcement
(0, 467), --Syntax: .gmnameannounce $announcement.
(0, 468), --Syntax: .gmnotify $notification
(0, 469), --Syntax: .nameannounce $announcement.
(0, 441), --Syntax: .list mail $character
(0, 440), --Syntax: .list auras
(0, 439), --Syntax: .list object #gameobject_id [#max_count]
(0, 438), --Syntax: .list item #item_id [#max_count]
(0, 437), --Syntax: .list creature #creature_id [#max_count]
(0, 436), --Syntax: .list $subcommand
(0, 371), --Syntax: .gm [on/off]
(0, 372), --Syntax: .gm chat [on/off]
(0, 374), --Syntax: .gm ingame
(0, 375), --Syntax: .gm list
(0, 376), --Syntax: .gm visible on/off
(0, 274), --Syntax: .character customize [$name]
(0, 275), --Syntax: .character changefaction $name
(0, 276), --Syntax: .character changerace $name
(0, 248), --Syntax: .banlist $subcommand
(0, 249), --Syntax: .banlist account [$Name]
(0, 250), --Syntax: .banlist character $Name
(0, 251), --Syntax: .banlist ip [$Ip]
(0, 284), --Syntax: .character rename [$name] [$newName] 
(0, 286), --Syntax: .character titles [$player_name]
(0, 496), --Syntax: .commands
(0, 368), --Syntax: .event activelist
(0, 367), --Syntax: .event #event_id
(1, 474), --Syntax: .group disband [$characterName]
(1, 475), --Syntax: .group remove [$characterName]
(1, 517), --Syntax: .pinfo [$player_name/#GUID]
(1, 373), --Syntax: .gm fly [on/off]
(1, 369), --Syntax: .event start #event_id
(1, 370), --Syntax: .event stop #event_id
(1, 279), --Syntax: .character deleted list [#guid|$name]
(1, 294), --Syntax: .cheat explore #flag
(1, 544), --Syntax: .modify $subcommand
(1, 547), --Syntax: .modify drunk #value
(1, 546), --Syntax: .modify bit #field #bit
(1, 549), --Syntax: .modify faction #factionid #flagid #npcflagid #dynamicflagid
(1, 550), --Syntax: .modify gender male/female
(1, 552), --Syntax: .modify hp #newhp
(1, 553), --Syntax: .modify hp #newhp
(1, 554), --Syntax: .modify money #money
(1, 556), --Syntax: .modify phase #phasemask
(1, 557), --Syntax: .modify rage #newrage
(1, 559), --Syntax: .modify runicpower #newrunicpower
(1, 560), --.modify scale #scale
(1, 561), --Syntax: .modify speed $speedtype #rate
(1, 562), --Syntax: .modify aspeed #rate
(1, 563), --Syntax: .modify speed backwalk #rate
(1, 564), --.modify speed fly #rate
(1, 565), --Syntax: .modify speed bwalk #rate
(1, 566), --Syntax: .modify speed swim #rate
(1, 568), --Syntax: .modify standstate #emoteid
(2, 571), --Syntax: .npc add #creature_id
(2, 569), --Syntax: .modify talentpoints #amount
(2, 558), --Syntax: .modify reputation #repId (#repvalue | $rankname [#delta])
(2, 545), --Syntax: .modify arenapoints #value
(2, 551), --Syntax: .modify honor $amount
(2, 555), --Syntax: .modify mount #id #speed