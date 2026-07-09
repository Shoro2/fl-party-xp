/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "Player.h"
#include "Object.h"
#include "DataMap.h"
#include "Formulas.h"

class FLPartyXPPlayer : public PlayerScript
{
public:
    FLPartyXPPlayer() : PlayerScript("MyPlayer") { }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* victim, uint8 /*xpSource*/) override
{
    if (Group* group = player->GetGroup())
    {
        uint32 groupSize = group->GetMembersCount();
        if (groupSize > 1)
        {
            if (player->IsWithinRange(victim, 100.0f))
            {
                //uint32 oldAmount = amount;
                bool isRaid = group->isRaidGroup();
                float bonusRate = Acore::XP::xp_in_group_rate(groupSize, isRaid);

                float adjusted = (float(amount) * groupSize) / bonusRate;

                amount = uint32(adjusted);
            }
            

            //ChatHandler(player->GetSession()).PSendSysMessage("[XP] Normalized to solo: %u (was %u)", amount, oldAmount);
        }
    }
}
};

void AddFLPartyXPScripts()
{
    new FLPartyXPPlayer();
}
