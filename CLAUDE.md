# fl-party-xp

Forgotten Land's XP rules, two PlayerScripts:

1. **Party XP** (from the old fork): a grouped player who kills a creature within 100 yd gets
   roughly the XP a solo kill would give, so playing together costs no levelling speed. Kill XP only.
2. **The FL levelling curve** (2026-10-14): XP earned on the Forgotten Land maps (quests, kills,
   exploration) is scaled by `player_xp_for_level / fl_xp_for_level` for the player's level, so
   the FL quest line (Kalu'thar Haven 1-48, then Port Ocean up to the Zul'Gorok Ruins) keeps the
   pace it was built for: the old FL server's own XP table asked 4-9x less XP for levels 1-48 and
   1.5-5.5x less for 60-79 (49-59 = stock). Everywhere else XP stays stock. Operator decision
   2026-10-14, option "A"; analysis in share-public
   `docs/World of Warcraft/forgotten-land/server/25-fl-quest-xp-curve.md`.

There is no module README (`.github/README.md` is the AzerothCore skeleton text).

## Ids and tables

| What | Value |
|---|---|
| World DB | `fl_xp_for_level` (`Level`, `Experience`): the old FL table, 79 rows, from its world dump 2025-07-29 (`data/sql/db-world/base/fl_party_xp_level_curve.sql`, applied by the updater) |
| Config | `FLPX.LevelCurve.Enable` (1), `FLPX.LevelCurve.Maps` (727, 751, 738-750, 752: the FL continent, Port Ocean and the FL dungeons); `FLPX.Enable` exists but is never read |
| Scripts | `FLPartyXPPlayer` ("MyPlayer", every hook enabled), `FLLevelCurveWorld`, `FLLevelCurvePlayer` |
| Quest data | the 13 quests whose `RewardXPDifficulty` was raised for the curve live in share-public `python_scripts/fl_content/sql/50_fl_quest_xp_curve.sql` |

## Status and progress

- **Party XP**: workbench and host since the first deploy (2026-07-13), T2 (2026-07-13).
- **Levelling curve**: built 2026-10-14 (branch `claude/fl-quest-xp-e2ed7f04`); evidence tier
  and host state in share-public `forgotten-land/server/25-fl-quest-xp-curve.md` and the MIG
  ledger. Test: `tests/fl_level_curve.tbs` (quest and kill XP on map 727 vs Northshire).

## Next steps

1. (suggestion) Party XP: use the core's eligible-member count instead of the full group size
   (see [functions.md](functions.md)).
2. (suggestion) Wire `FLPX.Enable` or drop it.

Open points in full: [todo.md](todo.md).

## Working here

- Branch `claude/<topic>-<sessionId>`, merge into `master` and push (project rule: no pull requests).
- Rebuild worldserver (`C:\wowstuff\dcore_bin`; a new source file needs a CMake re-configure) and
  restart the workbench with `C:\wowstuff\ForgottenLand2.0\scripts\worldserver_restart.ps1`.
  Group checks need two players in a party: use the test bots (share-public
  `docs/World of Warcraft/testing/01-test-bots.md`); `tests/` must be in `TestBots.ScenarioDirs`.
- A change to `fl_xp_for_level` changes the pace of every FL zone: walk the chain again with the
  model in `25-fl-quest-xp-curve.md` before shipping it.
- Host-relevant changes need a MIG entry in share-public
  `docs/World of Warcraft/forgotten-land/15-host-migration-log.md`.
- Vault: `forgotten-land/06-fl-modules.md`.
- Doc set: [INDEX.md](INDEX.md), CLAUDE.md, [data_structure.md](data_structure.md),
  [functions.md](functions.md), [log.md](log.md) (newest first), [todo.md](todo.md).
