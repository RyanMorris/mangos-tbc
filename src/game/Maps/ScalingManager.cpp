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
    //sLog.outString("[DEVLOG] ScalingManager::InserPlayerDef state in: diff %d  style %d", state.difficulty_, state.style_);
    //sLog.outString("[DEVLOG] ScalingManager::InserPlayerDef playerDefinedStates_[id]: diff %d  style %d", playerDefinedStates_[id].difficulty_, playerDefinedStates_[id].style_);
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

ScalingManagerState ScalingManager::GetStateDefinition(int difficulty, int style)
{
    // sLog.outString("[DEVLOG] ScalingManager::GetStateDefinition state in: diff %d  style %d", difficulty, style);
    ScalingManagerState state;
    if (difficulty == 1)
    {
        switch (style)
        {
            case 1: state = ScalingManagerState(0.35f, 0.06f); break;
            case 2: state = ScalingManagerState(0.28f, 0.10f); break;
            case 3: state = ScalingManagerState(0.22f, 0.16f); break;
            case 4: state = ScalingManagerState(0.17f, 0.23f); break;
            case 5: state = ScalingManagerState(0.13f, 0.31f); break;
            case 6: state = ScalingManagerState(0.10f, 0.40f); break;
            case 7: state = ScalingManagerState(0.08f, 0.50f); break;
            default: state = ScalingManagerState(1.0f, 1.0f);
        }
    }
    else if (difficulty == 2)
    {
        switch (style)
        {
            case 1: state = ScalingManagerState(0.42f, 0.13f); break;
            case 2: state = ScalingManagerState(0.35f, 0.17f); break;
            case 3: state = ScalingManagerState(0.29f, 0.23f); break;
            case 4: state = ScalingManagerState(0.24f, 0.30f); break;
            case 5: state = ScalingManagerState(0.20f, 0.38f); break;
            case 6: state = ScalingManagerState(0.17f, 0.47f); break;
            case 7: state = ScalingManagerState(0.15f, 0.57f); break;
            default: state = ScalingManagerState(1.0f, 1.0f);
        }
    }
    else
    {
        switch (style)
        {
            case 1: state = ScalingManagerState(0.49f, 0.20f); break;
            case 2: state = ScalingManagerState(0.42f, 0.24f); break;
            case 3: state = ScalingManagerState(0.36f, 0.30f); break;
            case 4: state = ScalingManagerState(0.31f, 0.37f); break;
            case 5: state = ScalingManagerState(0.27f, 0.45f); break;
            case 6: state = ScalingManagerState(0.24f, 0.54f); break;
            case 7: state = ScalingManagerState(0.22f, 0.64f); break;
            default: state = ScalingManagerState(1.0f, 1.0f);
        }
    }
    state.difficulty_ = difficulty;
    state.style_ = style;
    //sLog.outString("[DEVLOG] ScalingManager::GetStateDefinition state set: diff %d  style %d", state.difficulty_, state.style_);
    return state;
}

float ScalingManager::GetHealthMod(uint32 id, ScalingManagerState* state /*= nullptr*/)
{
    if (state == nullptr)
    {
        auto itr = instanceStates_.find(id);
        if (itr == instanceStates_.end())
            return 1.0f;
        state = &(itr->second);
    }
    switch (state->version_)
    {
        case 1:
        {
            return state->dpsFactor_;
        }
        case 2:
        {
            // solo healer
            if (state->hasHealer_ && !state->hasTank_ && state->numDps_ == 0)
            {
                return 0.1f;
            }
            float mod = state->hasTank_ ? 0.1f : 0.0f;
            for (auto i = 0; i < state->numDps_; i++)
                mod += 0.3f;
            return mod;
        }
        case 3:
        {
            return state->healthFactor_;
        }
        default: return 1.0f;
    }
}

float ScalingManager::GetDamageMod(uint32 id, ScalingManagerState* state /*= nullptr*/)
{
    if (state == nullptr)
    {
        auto itr = instanceStates_.find(id);
        if (itr == instanceStates_.end())
            return 1.0f;
        state = &(itr->second);
    }
    switch (state->version_)
    {
        case 1:
        {
            return state->tankFactor_ * state->healFactor_;
        }
        case 2:
        {
            float mod = state->hasTank_ ? 0.4f : 0.0f;
            mod += state->hasHealer_ ? 0.3f : 0.0f;
            for (auto i = 0; i < state->numDps_; i++)
                mod += 0.1f;
            return mod;
        }
        case 3:
        {
            return state->dmgFactor_;
        }
        default: return 1.0f;
    }
}