#ifndef __ScalingManager_H
#define __ScalingManager_H

#include "Common.h"
#include "Policies/Singleton.h"

typedef uint32 SeasonId;

struct SeasonCharacterStats
{
    float attackSpeedBonus {0.0f};
    float castSpeedBonus {0.0f};
    float damageBonus {0.0f};
    float damageReductionBonus {0.0f};
    float moveSpeedBonus {0.0f};
};

class SeasonManager
{
public:
    SeasonManager() = default;
    ~SeasonManager() {};

    // database
    const char* GetDBQuery();
    void UpdateCharacterSeasonStats(SeasonId characterGuid, const SeasonCharacterStats& stats);

    // cache
    bool InsertPlayerStats(SeasonId playerId, std::unique_ptr<QueryResult> queryResult, uint32 timediff);
    bool InsertPlayerStats(SeasonId playerId, const SeasonCharacterStats& stats);
    SeasonCharacterStats* GetPlayerStats(SeasonId playerId);

private:
    std::unordered_map<SeasonId, SeasonCharacterStats> playerStatsMap;
};

#define sSeasonManager MaNGOS::Singleton<SeasonManager>::Instance()
#endif