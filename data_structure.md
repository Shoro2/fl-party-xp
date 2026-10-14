# Data structure

## Files

| Path | What |
|---|---|
| `src/FLPartyXPPlayer.cpp` | `FLPartyXPPlayer` (PlayerScript "MyPlayer"), `AddFLPartyXPScripts()` |
| `src/FLLevelCurve.cpp` | the FL levelling curve: `FLLevelCurveWorld` (config + table load), `FLLevelCurvePlayer` (the XP hooks), `AddFLLevelCurveScripts()` |
| `src/FLPartyXP_loader.cpp` | `Addfl_party_xpScripts()` (calls both `Add...Scripts()`) |
| `conf/fl_party_xp.conf.dist` | `FLPX.Enable`, `FLPX.LevelCurve.Enable`, `FLPX.LevelCurve.Maps` |
| `data/sql/db-world/base/fl_party_xp_level_curve.sql` | creates and fills `fl_xp_for_level` (idempotent: `CREATE TABLE IF NOT EXISTS`, then DELETE + INSERT) |
| `data/sql/db-{auth,characters,world}/.gitkeep` | placeholders |
| `tests/fl_level_curve.tbs` | test bot scenario (mod-fl-testbots): quest and kill XP on map 727 and in Northshire |
| `apps/`, `include.sh`, `pull_request_template.md` | AzerothCore skeleton leftovers (`apps/ci/ci-codestyle.sh` = the skeleton's codestyle check) |
| `.github/README.md`, `.github/README_ES.md`, `.github/ISSUE_TEMPLATE/` | skeleton template texts, not a module description |
| `.github/workflows/core-build.yml`, `core_codestyle.yml` | CI: AzerothCore's reusable module build + codestyle |

## Tables

### `acore_world.fl_xp_for_level`

| Column | Type | What |
|---|---|---|
| `Level` | TINYINT UNSIGNED, PK | player level 1-79 |
| `Experience` | INT UNSIGNED | XP the old FL server asked for that level (its `player_xp_for_level`, world dump `F:\Backups\fl1-originals\dumps\Dump20250729`) |

Levels 1-48 are 4-9x cheaper than the stock `player_xp_for_level`, 49-59 equal it, 60-79 are
1.5-5.5x cheaper (78 and 79 cheaper than 77, as on the old server). A level without a row, or a
row with 0, scales by 1.

## Config

| Key | Default | What |
|---|---|---|
| `FLPX.Enable` | 1 | no effect; the party hook always runs |
| `FLPX.LevelCurve.Enable` | 1 | 0 = stock XP on the FL maps too |
| `FLPX.LevelCurve.Maps` | `727,751,738,739,740,741,742,743,744,745,746,747,748,749,750,752` | the maps that count as Forgotten Land (the player's map when the XP is given) |
