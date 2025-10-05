#include "Seasonal/SeasonManager.h"
#include <cstdarg>

INSTANTIATE_SINGLETON_1(SeasonManager);

const char* SeasonManager::GetDBQuery()
{
    return "SELECT attack_speed_bonus, cast_speed_bonus, damage_bonus, damage_reduction_bonus, move_speed_bonus FROM season_stats WHERE character_guid = '%u'";
}

void SeasonManager::UpdateCharacterSeasonStats(SeasonId characterGuid, const SeasonCharacterStats& stats)
{
    static SqlStatementID insId;

    CharacterDatabase.BeginTransaction();

    SqlStatement stmt = CharacterDatabase.CreateStatement(insId, "INSERT INTO season_stats VALUES (?,?,?,?,?)");
    stmt.PExecute(characterGuid, stats.attackSpeedBonus, stats.castSpeedBonus, stats.damageBonus, stats.damageReductionBonus, stats.moveSpeedBonus);

    CharacterDatabase.CommitTransaction();
}

bool SeasonManager::InsertPlayerStats(SeasonId playerId, std::unique_ptr<QueryResult> queryResult, uint32 timediff)
{
    if (queryResult)
    {
        Field* fields = queryResult->Fetch();
        SeasonCharacterStats stats;
        stats.attackSpeedBonus = fields[0].GetFloat();
        stats.castSpeedBonus = fields[1].GetFloat();
        stats.damageBonus = fields[2].GetFloat();
        stats.damageReductionBonus = fields[3].GetFloat();
        stats.moveSpeedBonus = fields[4].GetFloat();

        playerStatsMap[playerId] = stats;
    }
}

bool SeasonManager::InsertPlayerStats(SeasonId playerId, const SeasonCharacterStats& stats)
{
    playerStatsMap[playerId] = stats;
    return true;
}

SeasonCharacterStats* SeasonManager::GetPlayerStats(SeasonId playerId)
{
    auto stats = playerStatsMap.find(playerId);
    return stats == playerStatsMap.end() ? nullptr : &(stats->second);
}