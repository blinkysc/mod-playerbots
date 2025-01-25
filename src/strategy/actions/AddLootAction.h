/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

/**
 * @file AddLootAction.h
 * Defines actions for collecting loot in the playerbot system
 */

#ifndef _PLAYERBOT_ADDLOOTACTION_H
#define _PLAYERBOT_ADDLOOTACTION_H

#include "Action.h"

class ObjectGuid;
class PlayerbotAI;

/**
 * @brief Action for adding a single loot object
 */
class AddLootAction : public Action
{
    public:
        AddLootAction(PlayerbotAI* botAI) : Action(botAI, "add loot") {}
        bool Execute(Event const& event) override;
        bool isUseful() override { return true; }
};

/**
 * @brief Action for adding all nearby lootable objects
 */
class AddAllLootAction : public Action
{
    public:
        AddAllLootAction(PlayerbotAI* botAI, std::string const& name = "add all loot") 
            : Action(botAI, name) {}
        bool Execute(Event const& event) override;
        bool isUseful() override { return true; }

    protected:
        /**
         * @brief Adds a specific object to loot stack
         * @param guid Object to be looted
         * @return true if successfully added
         */
        virtual bool AddLoot(ObjectGuid const& guid)
        {
            return AI_VALUE(LootObjectStack*, "available loot")->Add(guid);
        }
};

/**
 * @brief Specialized action for gathering profession resources
 */
class AddGatheringLootAction : public AddAllLootAction
{
    public:
        AddGatheringLootAction(PlayerbotAI* botAI) 
            : AddAllLootAction(botAI, "add gathering loot") {}

    protected:
        bool AddLoot(ObjectGuid const& guid) override;
        bool HasNearbyEnemies() const;
};

#endif
