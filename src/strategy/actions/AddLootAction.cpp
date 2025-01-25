/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "AddLootAction.h"
#include "CellImpl.h"
#include "Event.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "LootObjectStack.h"
#include "Playerbots.h"
#include "ServerFacade.h"

bool AddLootAction::Execute(Event event)
{
    if (ObjectGuid const& guid = event.getObject())
    {
        return AI_VALUE(LootObjectStack*, "available loot")->Add(guid);
    }
    return false;
}

bool AddAllLootAction::Execute(Event event)
{
    bool added = false;
    
    if (GuidVector const& gameObjects = context->GetValue<GuidVector>("nearest game objects")->Get(); !gameObjects.empty())
    {
        for (ObjectGuid const& guid : gameObjects)
        {
            added |= AddLoot(guid);
        }
    }

    if (GuidVector const& corpses = context->GetValue<GuidVector>("nearest corpses")->Get(); !corpses.empty())
    {
        for (ObjectGuid const& guid : corpses)
        {
            added |= AddLoot(guid);
        }
    }

    return added;
}

bool AddAllLootAction::AddLoot(ObjectGuid const& guid)
{
    return AI_VALUE(LootObjectStack*, "available loot")->Add(guid);
}

bool AddGatheringLootAction::AddLoot(ObjectGuid const& guid)
{
    LootObject loot(bot, guid);
    
    WorldObject* worldObj = loot.GetWorldObject(bot);
    if (loot.IsEmpty() || !worldObj || !bot->IsWithinLOSInMap(worldObj))
    {
        return false;
    }

    if (loot.skillId == SKILL_NONE || !loot.IsLootPossible(bot))
    {
        return false;
    }

    if (sServerFacade->IsDistanceGreaterThan(sServerFacade->GetDistance2d(bot, worldObj), INTERACTION_DISTANCE))
    {
        if (HasNearbyEnemies())
        {
            return false;
        }
    }

    return AddAllLootAction::AddLoot(guid);
}

bool AddGatheringLootAction::HasNearbyEnemies() const
{
    std::list<Unit*> targets;
    Acore::AnyUnfriendlyUnitInObjectRangeCheck unitCheck(bot, bot, sPlayerbotAIConfig->lootDistance);
    Acore::UnitListSearcher<Acore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(bot, targets, unitCheck);
    Cell::VisitAllObjects(bot, searcher, sPlayerbotAIConfig->lootDistance * 1.5f);

    if (!targets.empty())
    {
        std::ostringstream out;
        out << "Kill that " << targets.front()->GetName() << " so I can loot freely";
        botAI->TellError(out.str());
        return true;
    }

    return false;
}
