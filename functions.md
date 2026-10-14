# Functions

## `FLPartyXPPlayer::OnPlayerGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource)`

PlayerScript registered as "MyPlayer" with the bare constructor, so every PlayerScript hook is
enabled; only this one is overridden.

1. Leaves `amount` alone unless the player is in a group with more than one member.
2. Leaves it alone unless `player->IsWithinRange(victim, 100.0f)` (3D distance). `victim` is null
   for quest, exploration and battleground XP, and `Unit::IsWithinRange` returns false for null,
   so only kill XP is changed.
3. `amount = amount * groupSize / Acore::XP::xp_in_group_rate(groupSize, isRaid)` with
   `groupSize = group->GetMembersCount()`.

`xp_in_group_rate`: raid 1.0; party of 1-2 1.0, 3 1.166, 4 1.3, 5 1.4.

## How it relates to the core

`KillRewarder` gives each member `xp * groupRate * level / sumLevel`, where `groupRate` uses the
number of alive members within reward distance (`_count`). With every member present, alive and
at the same level, the module's factor `groupSize / rate(groupSize)` cancels that split exactly,
i.e. solo XP. When members are away, dead or at other levels, the two counts differ and the
result is not solo XP (e.g. 2 of 5 present: about 1.8 x solo for each).

Order in `KillRewarder::_RewardXP`: the group split and the XP-percentage auras are applied
first, then this hook, then `Player::GiveXP` (which still adds the rested or Recruit-a-Friend
bonus and the level cap checks).

## The FL levelling curve (`src/FLLevelCurve.cpp`)

### `FLLevelCurveWorld` (WorldScript)

- `OnAfterConfigLoad(reload)`: reads `FLPX.LevelCurve.Enable` and `FLPX.LevelCurve.Maps`
  (comma-separated map ids); on a `.reload config` it also reloads the table.
- `OnStartup()`: loads `fl_xp_for_level` (the first config load runs before the world database
  is open) and logs `>> FL levelling curve: on|off, <n> levels, <n> maps`.

### `CurveFactor(player)`

`player_xp_for_level[level] / fl_xp_for_level[level]` (`sObjectMgr->GetXPForLevel`) for the
player's current level, when the curve is on and the player stands on one of the configured
maps; 1 otherwise (no row, a 0 row, another map). `Scale(player, xp)` = `uint32(xp * factor +
0.5)`.

### `FLLevelCurvePlayer` (PlayerScript, hooks `ON_GIVE_EXP` and `ON_QUEST_COMPUTE_EXP`)

- `OnPlayerQuestComputeXP(player, quest, xpValue)`: scales the quest's XP. The core reads this
  hook both for the quest giver windows (`PlayerMenu::SendQuestGiverQuestDetails` and
  `SendQuestGiverOfferReward`) and in `Player::RewardQuest`, so the window shows the XP the
  player gets. The quest log's own XP line is computed by the client (QuestXP.dbc and the tier,
  without any server rate) and stays unscaled.
- `OnPlayerGiveXP(player, amount, victim, xpSource)`: scales `XPSOURCE_KILL` and
  `XPSOURCE_EXPLORE`. Quest XP (`XPSOURCE_QUEST`) passes untouched here: `RewardQuest` hands
  over the value the quest hook already scaled. Battleground and dungeon-finder XP stay stock.

With the party hook: both scripts change kill XP (order = registration order; both are
factors, so only the rounding differs). The pet gets the scaled kill XP too (`KillRewarder` passes
the hooked amount to `Pet::GivePetXP`). Rested XP is added by `Player::GiveXP` on top of the
scaled kill XP, as everywhere.

Numbers (stock / old FL XP for the level): 1: 4.0, 10: 5.85, 20: 7.43, 28: 8.69, 34: 5.71,
48: 5.76, 49-59: 1.0, 60: 1.53, 69: 1.91, 70: 3.89, 75: 3.20, 78: 5.51, 79: 5.30.
