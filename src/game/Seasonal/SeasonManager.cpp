#include "Seasonal/SeasonManager.h"
#include <cstdarg>

INSTANTIATE_SINGLETON_1(SeasonManager);

const char* SeasonManager::GetSeasonStatsDBQuery()
{
    return "SELECT attack_speed_bonus, cast_speed_bonus, damage_bonus, damage_reduction_bonus, move_speed_bonus, talent_points, talent_string, difficulty FROM season_stats WHERE character_guid = '%u'";
}

const char* SeasonManager::GetSeasonTrackingDBQuery()
{
    return "SELECT total_kills, elite_kills, quests_completed FROM season_tracking WHERE character_guid = '%u'";
}

void SeasonManager::SaveSeasonStats(SeasonId characterGuid, const SeasonCharacterStats& stats, bool manageTransaction)
{
    // need to add new fields, if actually going to use this
    /*static SqlStatementID insId;

    CharacterDatabase.BeginTransaction();

    SqlStatement stmt = CharacterDatabase.CreateStatement(insId, "INSERT INTO season_stats VALUES (?,?,?,?,?)");
    stmt.PExecute(characterGuid, stats.attackSpeedBonus, stats.castSpeedBonus, stats.damageBonus, stats.damageReductionBonus, stats.moveSpeedBonus);

    CharacterDatabase.CommitTransaction();*/
}

void SeasonManager::SaveSeasonTracking(SeasonId characterGuid, const SeasonTracking& tracking, bool manageTransaction)
{
    if (manageTransaction)
        CharacterDatabase.BeginTransaction();

    static SqlStatementID delId;
    static SqlStatementID insId;
    SqlStatement stmtDel = CharacterDatabase.CreateStatement(delId, "DELETE FROM season_tracking WHERE character_guid = ?");
    SqlStatement stmtIns = CharacterDatabase.CreateStatement(insId, "INSERT INTO season_tracking (character_guid,total_kills,elite_kills,quests_completed) VALUES (?,?,?,?)");

    stmtDel.PExecute(characterGuid);
    auto temp = tracking.totalKills + 1;
    stmtIns.PExecute(characterGuid, temp, tracking.eliteKills, tracking.questsCompleted);

    if (manageTransaction)
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
        stats.talentPoints = fields[5].GetUInt32();
        stats.talentString = fields[6].GetCppString();
        stats.difficulty = fields[7].GetUInt8();

        playerStatsMap[playerId] = stats;
        return true;
    }
    return false;
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

bool SeasonManager::InsertPlayerTracking(SeasonId playerId, std::unique_ptr<QueryResult> queryResult, uint32 timediff)
{
    if (queryResult)
    {
        Field* fields = queryResult->Fetch();
        SeasonTracking tracking;
        tracking.totalKills = fields[0].GetUInt32();
        tracking.eliteKills = fields[1].GetUInt32();
        tracking.questsCompleted = fields[2].GetUInt32();

        playerTrackingMap[playerId] = tracking;
        return true;
    }
    return false;
}

SeasonTracking* SeasonManager::GetPlayerTracking(SeasonId playerId)
{
    auto t = playerTrackingMap.find(playerId);
    return t == playerTrackingMap.end() ? nullptr : &(t->second);
}