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

#include "Maps/ScalingManager.h"

INSTANTIATE_SINGLETON_1(ScalingManager);

//ScalingManagerState::ScalingManagerState(uint32 instanceId)
//    : instanceId_(instanceId)
//{
//}
//
//ScalingManagerState::~ScalingManagerState()
//{
//}

ScalingManager::ScalingManager()
{
}

ScalingManager::~ScalingManager()
{
    /*for (auto& itr : states_)
        delete itr.second;*/
}

bool ScalingManager::Insert(uint32 instanceId)
{
    states_[instanceId] = {};
    return true;
}

bool ScalingManager::Insert(uint32 instanceId, const ScalingManagerState& state)
{
    states_[instanceId] = state;
    return true;
}

std::string ScalingManager::Print(uint32 instanceId)
{
    auto itr = states_.find(instanceId);
    if (itr == states_.end())
        return "N/A";
    return "tank: " + std::to_string(itr->second.tankFactor_)
        + " heal: " + std::to_string(itr->second.healFactor_)
        + " dps: " + std::to_string(itr->second.dpsFactor_);
}