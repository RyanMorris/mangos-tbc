#ifndef __ScalingManager_H
#define __ScalingManager_H

#include "Common.h"
#include "Policies/Singleton.h"

typedef uint32 SeasonId;

struct SeasonCharacterStats
{
    float attackSpeedBonus{ 0.0f };
    float castSpeedBonus{ 0.0f };
    float damageBonus{ 0.0f };
    float damageReductionBonus{ 0.0f };
    float moveSpeedBonus{ 0.0f };
    uint32 talentPoints{ 0 };
    std::string talentString{ "" };
    uint8 difficulty{ 0 };
};

struct SeasonTracking
{
    uint32 totalKills{ 0 };
    uint32 eliteKills{ 0 };
    uint32 questsCompleted{ 0 };
};

class SeasonManager
{
public:
    SeasonManager() = default;
    ~SeasonManager() {};

    // database
    const char* GetSeasonStatsDBQuery();
    const char* GetSeasonTrackingDBQuery();
    void SaveSeasonStats(SeasonId characterGuid, const SeasonCharacterStats& stats, bool manageTransaction = false);
    void SaveSeasonTracking(SeasonId characterGuid, const SeasonTracking& tracking, bool manageTransaction = false);

    // cache
    bool InsertPlayerStats(SeasonId playerId, std::unique_ptr<QueryResult> queryResult, uint32 timediff);
    bool InsertPlayerStats(SeasonId playerId, const SeasonCharacterStats& stats);
    SeasonCharacterStats* GetPlayerStats(SeasonId playerId);

    bool InsertPlayerTracking(SeasonId playerId, std::unique_ptr<QueryResult> queryResult, uint32 timediff);
    SeasonTracking* GetPlayerTracking(SeasonId playerId);

private:
    std::unordered_map<SeasonId, SeasonCharacterStats> playerStatsMap;
    std::unordered_map<SeasonId, SeasonTracking> playerTrackingMap;
};

#define sSeasonManager MaNGOS::Singleton<SeasonManager>::Instance()
#endif