/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ScalingManager_H
#define __ScalingManager_H

#include "Common.h"
#include "Policies/Singleton.h"

class Player;
class Group;
class Map;


struct ScalingManagerState
{
    int32 version_{ 2 }; // which version to use
    int32 difficulty_{ 1 };
    int32 style_{ 1 };

    float tankFactor_{ 1.0f };  // how many tanks out of max, 0.0 - 1.0, ie 2/2 or 1/2
    float healFactor_{ 1.0f };  // how many healers out of max, 0.0 - 1.0, ie 2/2 or 1/2
    float dpsFactor_{ 1.0f };   // how many dps out of max, 0.0 - 1.0, ie 2/2 or 1/2

    // new way, or dungeon way
    bool hasTank_{ true };
    bool hasHealer_{ true };
    int32 numDps_{ 3 };

    // full control way
    float dmgFactor_{ 1.0f };  
    float healthFactor_{ 1.0f };  

    ScalingManagerState() {}

    ScalingManagerState(float tank, float heals, float dps)
        : version_(1), tankFactor_(tank), healFactor_(heals), dpsFactor_(dps)
    {}

    ScalingManagerState(bool hasTank, bool hasHealer, int32 numDps)
        : version_(2), hasTank_(hasTank), hasHealer_(hasHealer), numDps_(numDps)
    {}

    ScalingManagerState(float health, float dmg)
        : version_(3), healthFactor_(health), dmgFactor_(dmg)
    {}

    ~ScalingManagerState() {}
};


class ScalingManager
{
public:
    ScalingManager();
    ~ScalingManager();

    bool InsertInstance(uint32 id, const ScalingManagerState& state);
    bool InserPlayerDef(uint32 id, const ScalingManagerState& state);

    typedef std::unordered_map<uint32, ScalingManagerState> ScalingStateMap;

    ScalingStateMap& GetInstanceStates() { return instanceStates_; }
    ScalingManagerState* GetInstanceState(uint32 id);
    ScalingManagerState* GetPlayerDefState(uint32 id);

    ScalingManagerState GetStateDefinition(int difficulty, int style);

    float GetHealthMod(uint32 id, ScalingManagerState* state = nullptr);
    float GetDamageMod(uint32 id, ScalingManagerState* state = nullptr);

private:
    ScalingStateMap instanceStates_;            // state of each map instance      
    ScalingStateMap playerDefinedStates_;       // each player can setup values, then the instance uses their settings to create the map
};

#define sScalingManager MaNGOS::Singleton<ScalingManager>::Instance()
#endif
