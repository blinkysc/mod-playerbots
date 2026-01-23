/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_BOTVALUECACHE_H
#define _PLAYERBOT_BOTVALUECACHE_H

#include "ObjectGuid.h"

class Unit;
class AiObjectContext;
class PlayerbotAI;
template<class T> class Value;
class LastMovement;

// Cached Value* pointers for frequently accessed values
// Eliminates string-based hash lookups (was ~800 lookups/tick with string keys)
struct BotValueCache
{
    // Unit* values
    Value<Unit*>* currentTarget = nullptr;
    Value<Unit*>* mainTank = nullptr;
    Value<Unit*>* enemyPlayerTarget = nullptr;

    // GuidVector values
    Value<GuidVector>* nearestHostileNpcs = nullptr;
    Value<GuidVector>* possibleTargets = nullptr;
    Value<GuidVector>* possibleTargetsNoLos = nullptr;
    Value<GuidVector>* groupMembers = nullptr;
    Value<GuidVector>* nearestNpcs = nullptr;

    // Other common values
    Value<uint8>* attackerCount = nullptr;
    Value<float>* disperseDistance = nullptr;

    // Initialize cached pointers from context (called once)
    void Initialize(AiObjectContext* context);

    // Check if initialized
    bool IsInitialized() const { return currentTarget != nullptr; }
};

// Fast access macros - use cached Value* instead of string lookup
#define CACHE_VALUE(cache, member) ((cache).member ? (cache).member->Get() : decltype((cache).member->Get()){})

#endif
