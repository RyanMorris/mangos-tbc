#include "Seasonal/SeasonManager.h"
#include <cstdarg>

INSTANTIATE_SINGLETON_1(SeasonManager);

bool SeasonManager::InsertPlayerStats(ObjectGuid playerId, const SeasonCharacterStats& stats)
{
    playerStatsMap[playerId] = stats;
    return true;
}

SeasonCharacterStats* SeasonManager::GetPlayerStats(ObjectGuid playerId)
{
    auto stats = playerStatsMap.find(playerId);
    return stats == playerStatsMap.end() ? nullptr : &(stats->second);
}