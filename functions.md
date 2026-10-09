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
