/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// The Forgotten Land levelling curve.
//
// The FL quest line (Kalu'thar Haven 1-48, then Port Ocean up to the
// Zul'Gorok Ruins) was built for the old FL server's own XP table, which
// asked 4-9 times less XP for levels 1-48 and 1.5-5.5 times less for 60-79.
// FL 2.0 keeps the stock table for the rest of Azeroth, so XP earned on the
// FL maps is scaled by player_xp_for_level / fl_xp_for_level for the
// player's current level: the FL content keeps the pace it was built for,
// everything else stays stock.

#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QuestDef.h"
#include "ScriptMgr.h"
#include "StringConvert.h"
#include "Tokenize.h"
#include <unordered_map>
#include <unordered_set>

namespace
{
    bool _curveEnabled = false;
    std::unordered_set<uint32> _curveMaps;
    std::unordered_map<uint8, uint32> _oldXpForLevel;

    void LoadCurveConfig()
    {
        _curveEnabled = sConfigMgr->GetOption<bool>(
            "FLPX.LevelCurve.Enable", true);

        _curveMaps.clear();
        std::string const maps = sConfigMgr->GetOption<std::string>(
            "FLPX.LevelCurve.Maps", "727,751,738,739,740,741,742,743,744,"
            "745,746,747,748,749,750,752");
        for (std::string_view token : Acore::Tokenize(maps, ',', false))
            if (Optional<uint32> mapId = Acore::StringTo<uint32>(token))
                _curveMaps.insert(*mapId);
    }

    void LoadCurveTable()
    {
        _oldXpForLevel.clear();
        QueryResult result = WorldDatabase.Query(
            "SELECT `Level`, `Experience` FROM `fl_xp_for_level`");
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 const xp = fields[1].Get<uint32>();
                if (xp)
                    _oldXpForLevel[fields[0].Get<uint8>()] = xp;
            } while (result->NextRow());
        }

        LOG_INFO("module", ">> FL levelling curve: {}, {} levels, {} maps",
            _curveEnabled ? "on" : "off", _oldXpForLevel.size(),
            _curveMaps.size());
    }

    // XP needed for the player's level today / on the old FL server; 1 off
    // the FL maps or for a level the old table does not list.
    float CurveFactor(Player const* player)
    {
        if (!_curveEnabled || !player
            || !_curveMaps.count(player->GetMapId()))
            return 1.0f;

        uint8 const level = player->GetLevel();
        auto itr = _oldXpForLevel.find(level);
        if (itr == _oldXpForLevel.end())
            return 1.0f;

        uint32 const stockXp = sObjectMgr->GetXPForLevel(level);
        if (!stockXp)
            return 1.0f;

        return float(stockXp) / float(itr->second);
    }

    uint32 Scale(Player const* player, uint32 xp)
    {
        float const factor = CurveFactor(player);
        if (factor == 1.0f || !xp)
            return xp;

        return uint32(float(xp) * factor + 0.5f);
    }
}

class FLLevelCurveWorld : public WorldScript
{
public:
    FLLevelCurveWorld() : WorldScript("FLLevelCurveWorld", {
        WORLDHOOK_ON_AFTER_CONFIG_LOAD,
        WORLDHOOK_ON_STARTUP
    }) { }

    // The first config load runs before the world database is open, so the
    // table loads at startup; a later `.reload config` reloads it too.
    void OnAfterConfigLoad(bool reload) override
    {
        LoadCurveConfig();
        if (reload)
            LoadCurveTable();
    }

    void OnStartup() override
    {
        LoadCurveTable();
    }
};

class FLLevelCurvePlayer : public PlayerScript
{
public:
    FLLevelCurvePlayer() : PlayerScript("FLLevelCurvePlayer", {
        PLAYERHOOK_ON_GIVE_EXP,
        PLAYERHOOK_ON_QUEST_COMPUTE_EXP
    }) { }

    // Quest XP is scaled here: the quest window and the reward both read
    // this hook, so the window shows the XP the player really gets.
    void OnPlayerQuestComputeXP(Player* player, Quest const* /*quest*/,
        uint32& xpValue) override
    {
        xpValue = Scale(player, xpValue);
    }

    // Kill and exploration XP; quest XP was already scaled above.
    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* /*victim*/,
        uint8 xpSource) override
    {
        if (xpSource == XPSOURCE_KILL || xpSource == XPSOURCE_EXPLORE)
            amount = Scale(player, amount);
    }
};

void AddFLLevelCurveScripts()
{
    new FLLevelCurveWorld();
    new FLLevelCurvePlayer();
}
