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
#include <cstdarg>

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
    //instanceStates_[0] = {};    // default
}

ScalingManager::~ScalingManager()
{
    /*for (auto& itr : instanceStates_)
        delete itr.second;*/
}

bool ScalingManager::InsertInstance(uint32 id, const ScalingManagerState& state)
{
    instanceStates_[id] = state;
    return true;
}

bool ScalingManager::InserPlayerDef(uint32 id, const ScalingManagerState& state)
{
    playerDefinedStates_[id] = state;
    return true;
}

ScalingManagerState* ScalingManager::GetInstanceState(uint32 id)
{
    auto itr = instanceStates_.find(id);
    return itr == instanceStates_.end() ? nullptr : &(itr->second);
}

ScalingManagerState* ScalingManager::GetPlayerDefState(uint32 id)
{
    auto itr = playerDefinedStates_.find(id);
    return itr == playerDefinedStates_.end() ? nullptr : &(itr->second);
}