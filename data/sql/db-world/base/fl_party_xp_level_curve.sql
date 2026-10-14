-- fl-party-xp: the old Forgotten Land levelling curve.
-- XP per level of the old FL server (its player_xp_for_level, world dump 2025-07-29).
-- XP earned on the FL maps is scaled by player_xp_for_level / fl_xp_for_level for the
-- player's level, so the FL quest line keeps the pace it was built for.
CREATE TABLE IF NOT EXISTS `fl_xp_for_level` (
    `Level` TINYINT UNSIGNED NOT NULL,
    `Experience` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`Level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

DELETE FROM `fl_xp_for_level` WHERE `Level` BETWEEN 1 AND 255;
INSERT INTO `fl_xp_for_level` (`Level`, `Experience`) VALUES
    (1,100), (2,200), (3,300), (4,380), (5,500), (6,680), (7,800), (8,950),
    (9,1150), (10,1300), (11,1480), (12,1630), (13,1800), (14,1940), (15,2100), (16,2250),
    (17,2400), (18,2560), (19,2650), (20,2800), (21,2910), (22,3120), (23,3240), (24,3370),
    (25,3500), (26,3650), (27,3780), (28,3900), (29,4050), (30,4300), (31,4720), (32,5110),
    (33,5560), (34,9005), (35,9930), (36,10850), (37,11430), (38,12000), (39,12870), (40,13320),
    (41,14120), (42,15009), (43,15860), (44,16760), (45,17420), (46,18032), (47,18750), (48,19230),
    (49,114800), (50,120900), (51,126100), (52,131500), (53,137000), (54,142500), (55,148200), (56,154000),
    (57,159900), (58,165800), (59,172000), (60,190000), (61,212000), (62,234000), (63,256000), (64,271000),
    (65,292000), (66,315000), (67,332000), (68,354000), (69,376100), (70,392000), (71,414500), (72,438500),
    (73,462000), (74,485400), (75,501200), (76,524300), (77,525000), (78,300000), (79,315000);
