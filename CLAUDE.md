# fl-party-xp

Removes the group XP split on Forgotten Land: a grouped player who kills a creature within
100 yd gets roughly the XP a solo kill would give, so playing together costs no levelling speed.
Only kill XP changes; quest, exploration and battleground XP pass untouched. One small
PlayerScript from the old fork; there is no module README (`.github/README.md` is the AzerothCore
skeleton text).

## Ids and tables

None: no SQL (the skeleton SQL was removed in `06268cf`), no ids, no tables. The script is a
PlayerScript registered under the name "MyPlayer" (class `FLPartyXPPlayer`). Config key
`FLPX.Enable` exists in the conf but is never read.

## Status and progress

- **Where it runs**: workbench (built from `azerothcore-wotlk/modules/fl-party-xp`) and host
  since the first deploy (2026-07-13), both at `5dbcd59`; no host window has moved it since.
- **Evidence**: module test pass incl. party-xp T2 (user-confirmed 2026-07-13).
- **Done**: port to the current AzerothCore API (2026-07-09). No work since.

## Next steps

No open work known; last change 2026-07-09.

1. (suggestion) Decide whether the multiplier should use the core's count: it uses the full
   group size, while the core splits kill XP among the alive members within reward distance and
   by level share (`KillRewarder`), so with members away or at other levels the result is not
   solo XP (more than solo when members are away).
2. (suggestion) Wire `FLPX.Enable` or drop it.

Open points in full: [todo.md](todo.md).

## Working here

- Branch `claude/<topic>-<sessionId>`, merge into `master` and push (project rule: no pull requests).
- C++ only: rebuild worldserver (`C:\wowstuff\dcore_bin`) and restart the workbench with
  `C:\wowstuff\ForgottenLand2.0\scripts\worldserver_restart.ps1`. A group XP check needs two
  players in a party: use the test bots (share-public `docs/World of Warcraft/testing/01-test-bots.md`).
- Host-relevant changes need a MIG entry in share-public
  `docs/World of Warcraft/forgotten-land/15-host-migration-log.md`.
- Vault: `forgotten-land/06-fl-modules.md`.
- Doc set: [INDEX.md](INDEX.md), CLAUDE.md, [data_structure.md](data_structure.md),
  [functions.md](functions.md), [log.md](log.md) (newest first), [todo.md](todo.md).
