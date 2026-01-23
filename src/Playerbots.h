/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_H
#define _PLAYERBOT_H

#include "AiObjectContext.h"
#include "Group.h"
#include "Pet.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "PlayerbotMgr.h"
#include "RandomPlayerbotMgr.h"
#include "SharedValueContext.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "TravelNode.h"

std::vector<std::string> split(std::string const s, char delim);
void split(std::vector<std::string>& dest, std::string const str, char const* delim);
#ifndef WIN32
int strcmpi(char const* s1, char const* s2);
#endif

#define CAST_ANGLE_IN_FRONT (2.f * static_cast<float>(M_PI) / 3.f)
#define EMOTE_ANGLE_IN_FRONT (2.f * static_cast<float>(M_PI) / 6.f)

#define GET_PLAYERBOT_AI(object) sPlayerbotsMgr->GetPlayerbotAI(object)
#define GET_PLAYERBOT_MGR(object) sPlayerbotsMgr->GetPlayerbotMgr(object)

#define AI_VALUE(type, name) context->GetValue<type>(name)->Get()
#define AI_VALUE2(type, name, param) context->GetValue<type>(name, param)->Get()

// Cached value access - uses pre-cached Value* pointers instead of string lookups
// These eliminate ~800 string hash lookups per tick with 5000 bots
#define GET_CURRENT_TARGET() (botAI->GetValueCache().currentTarget ? botAI->GetValueCache().currentTarget->Get() : nullptr)
#define GET_MAIN_TANK() (botAI->GetValueCache().mainTank ? botAI->GetValueCache().mainTank->Get() : nullptr)
#define GET_ENEMY_PLAYER_TARGET() (botAI->GetValueCache().enemyPlayerTarget ? botAI->GetValueCache().enemyPlayerTarget->Get() : nullptr)
#define GET_NEAREST_HOSTILE_NPCS() (botAI->GetValueCache().nearestHostileNpcs ? botAI->GetValueCache().nearestHostileNpcs->Get() : GuidVector{})
#define GET_POSSIBLE_TARGETS() (botAI->GetValueCache().possibleTargets ? botAI->GetValueCache().possibleTargets->Get() : GuidVector{})
#define GET_POSSIBLE_TARGETS_NO_LOS() (botAI->GetValueCache().possibleTargetsNoLos ? botAI->GetValueCache().possibleTargetsNoLos->Get() : GuidVector{})
#define GET_GROUP_MEMBERS() (botAI->GetValueCache().groupMembers ? botAI->GetValueCache().groupMembers->Get() : GuidVector{})
#define GET_NEAREST_NPCS() (botAI->GetValueCache().nearestNpcs ? botAI->GetValueCache().nearestNpcs->Get() : GuidVector{})
#define GET_ATTACKER_COUNT() (botAI->GetValueCache().attackerCount ? botAI->GetValueCache().attackerCount->Get() : 0)
#define GET_DISPERSE_DISTANCE() (botAI->GetValueCache().disperseDistance ? botAI->GetValueCache().disperseDistance->Get() : 0.0f)

#define AI_VALUE_LAZY(type, name) context->GetValue<type>(name)->LazyGet()
#define AI_VALUE2_LAZY(type, name, param) context->GetValue<type>(name, param)->LazyGet()

#define AI_VALUE_REF(type, name) context->GetValue<type>(name)->RefGet()

#define SET_AI_VALUE(type, name, value) context->GetValue<type>(name)->Set(value)
#define SET_AI_VALUE2(type, name, param, value) context->GetValue<type>(name, param)->Set(value)
#define RESET_AI_VALUE(type, name) context->GetValue<type>(name)->Reset()
#define RESET_AI_VALUE2(type, name, param) context->GetValue<type>(name, param)->Reset()

#define PAI_VALUE(type, name) sPlayerbotsMgr->GetPlayerbotAI(player)->GetAiObjectContext()->GetValue<type>(name)->Get()
#define PAI_VALUE2(type, name, param) \
    sPlayerbotsMgr->GetPlayerbotAI(player)->GetAiObjectContext()->GetValue<type>(name, param)->Get()
#define GAI_VALUE(type, name) sSharedValueContext->getGlobalValue<type>(name)->Get()
#define GAI_VALUE2(type, name, param) sSharedValueContext->getGlobalValue<type>(name, param)->Get()

#endif
