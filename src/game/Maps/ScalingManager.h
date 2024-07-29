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
    float tankFactor_{ 1.0f };  // how many tanks out of max, 0.0 - 1.0, ie 2/2 or 1/2
    float healFactor_{ 1.0f };  // how many healers out of max, 0.0 - 1.0, ie 2/2 or 1/2
    float dpsFactor_{ 1.0f };   // how many dps out of max, 0.0 - 1.0, ie 2/2 or 1/2
};



class ScalingManager
{
public:
    ScalingManager();
    ~ScalingManager();

    bool Insert(uint32 instanceId);
    bool Insert(uint32 instanceId, const ScalingManagerState& state);

    std::string Print(uint32 instanceId);

private:
    typedef std::unordered_map < uint32 /*InstanceId*/, ScalingManagerState > StatesPerInstance;

    StatesPerInstance states_;
};

#define sScalingManager MaNGOS::Singleton<ScalingManager>::Instance()
#endif
