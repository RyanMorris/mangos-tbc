#ifndef __ScalingManager_H
#define __ScalingManager_H

#include "Common.h"
#include "Policies/Singleton.h"

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

    bool InsertPlayerStats(ObjectGuid playerId, const SeasonCharacterStats& stats);
    SeasonCharacterStats* GetPlayerStats(ObjectGuid playerId);

private:
    std::unordered_map<ObjectGuid, SeasonCharacterStats> playerStatsMap;
};

#define sSeasonManager MaNGOS::Singleton<SeasonManager>::Instance()
#endif