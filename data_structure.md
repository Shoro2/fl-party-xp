# Data structure

## Files

| Path | What |
|---|---|
| `src/FLPartyXPPlayer.cpp` | `FLPartyXPPlayer` (PlayerScript "MyPlayer"), `AddFLPartyXPScripts()` |
| `src/FLPartyXP_loader.cpp` | `Addfl_party_xpScripts()` |
| `conf/fl_party_xp.conf.dist` | `FLPX.Enable = 1` (never read) |
| `data/sql/db-{auth,characters,world}/.gitkeep` | empty; the module has no SQL |
| `apps/`, `include.sh`, `pull_request_template.md` | AzerothCore skeleton leftovers (`apps/ci/ci-codestyle.sh` = the skeleton's codestyle check) |
| `.github/README.md`, `.github/README_ES.md`, `.github/ISSUE_TEMPLATE/` | skeleton template texts, not a module description |
| `.github/workflows/core-build.yml`, `core_codestyle.yml` | CI: AzerothCore's reusable module build + codestyle |

## Tables

None.

## Config

`FLPX.Enable` (dist and workbench: 1) has no effect; the hook always runs.
