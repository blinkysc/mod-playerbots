/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "BotState.h"
#include "AiObjectContext.h"
#include "Value.h"

void BotValueCache::Initialize(AiObjectContext* context)
{
    if (!context)
        return;

    // Cache Value* pointers for frequently accessed values
    // This is done once per bot, eliminating repeated string lookups

    // Unit* values
    currentTarget = context->GetValue<Unit*>("current target");
    mainTank = context->GetValue<Unit*>("main tank");
    enemyPlayerTarget = context->GetValue<Unit*>("enemy player target");

    // GuidVector values
    nearestHostileNpcs = context->GetValue<GuidVector>("nearest hostile npcs");
    possibleTargets = context->GetValue<GuidVector>("possible targets");
    possibleTargetsNoLos = context->GetValue<GuidVector>("possible targets no los");
    groupMembers = context->GetValue<GuidVector>("group members");
    nearestNpcs = context->GetValue<GuidVector>("nearest npcs");

    // Other common values
    attackerCount = context->GetValue<uint8>("attacker count");
    disperseDistance = context->GetValue<float>("disperse distance");
}
