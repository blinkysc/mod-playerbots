/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

/**
 * @file AddLootAction.cpp
 * Implements loot collection actions for playerbots
 */

#include "AddLootAction.h"
#include "CellImpl.h"
#include "Event.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "LootObjectStack.h"
#include "Playerbots.h"
#include "ServerFacade.h"

/**
 * @brief Attempts to add a single loot object to the bot's available loot
 * @param event Event containing the object GUID to loot
 * @return true if loot was successfully added, false otherwise
 */
bool AddLootAction::Execute(Event const& event)
{
    if (ObjectGuid const& guid = event.getObject())
    {
        return AI_VALUE(LootObjectStack*, "available loot")->Add(guid);
    }
    return false;
}

/**
 * @brief Attempts to add all nearby lootable objects and corpses
 * @param event Unused event parameter
 * @return true if any loot was added, false if none was added
 */
bool AddAllLootAction::Execute(Event const& /*event*/)
{
    bool added = false;
    
    // Process nearby game objects
    if (GuidVector const& gameObjects = context->GetValue<GuidVector>("nearest game objects")->Get(); !gameObjects.empty())
    {
        for (ObjectGuid const& guid : gameObjects)
        {
            added |= AddLoot(guid);
        }
    }

    // Process nearby corpses
    if (GuidVector const& corpses = context->GetValue<GuidVector>("nearest corpses")->Get(); !corpses.empty())
    {
        for (ObjectGuid const& guid : corpses)
        {
            added |= AddLoot(guid);
        }
    }

    return added;
}

/**
 * @brief Specialized loot addition for gathering professions
 * @param guid GUID of the object to gather from
 * @return true if gathering loot was added, false if conditions weren't met
 */
bool AddGatheringLootAction::AddLoot(ObjectGuid const& guid)
{
    LootObject loot(bot, guid);
    
    // Basic validity checks
    WorldObject* worldObj = loot.GetWorldObject(bot);
    if (loot.IsEmpty() || !worldObj || !bot->IsWithinLOSInMap(worldObj))
    {
        return false;
    }

    // Verify bot can gather this object
    if (loot.skillId == SKILL_NONE || !loot.IsLootPossible(bot))
    {
        return false;
    }

    // Check for nearby enemies if not in interaction range
    if (sServerFacade->IsDistanceGreaterThan(sServerFacade->GetDistance2d(bot, worldObj), INTERACTION_DISTANCE))
    {
        if (HasNearbyEnemies())
        {
            return false;
        }
    }

    return AddAllLootAction::AddLoot(guid);
}

/**
 * @brief Checks for hostile units near the bot
 * @return true if enemies are nearby, false if area is clear
 */
bool AddGatheringLootAction::HasNearbyEnemies() const
{
    std::list<Unit*> targets;
    // Set up range check for hostile units
    Acore::AnyUnfriendlyUnitInObjectRangeCheck unitCheck(bot, bot, sPlayerbotAIConfig->lootDistance);
    Acore::UnitListSearcher<Acore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(bot, targets, unitCheck);
    
    // Search for hostile units in slightly larger range than loot distance
    Cell::VisitAllObjects(bot, searcher, sPlayerbotAIConfig->lootDistance * 1.5f);

    if (!targets.empty())
    {
        // Notify bot about nearby enemy preventing looting
        std::ostringstream out;
        out << "Kill that " << targets.front()->GetName() << " so I can loot freely";
        botAI->TellError(out.str());
        return true;
    }

    return false;
}
