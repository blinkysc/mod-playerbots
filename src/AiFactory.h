/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_AIFACTORY_H
#define _PLAYERBOT_AIFACTORY_H

#include "Common.h"
#include <map>

// Forward declarations to reduce header dependencies
class AiObjectContext;
class Engine;
class Player;
class PlayerbotAI;
enum BotRoles : uint8;

/**
 * @class AiFactory
 * @brief Factory class for creating and configuring AI components for playerbots
 * 
 * This class provides static methods for creating and initializing different types
 * of AI engines (combat, non-combat, dead state) and managing player bot strategies.
 */
class AiFactory
{
public:
    // Engine creation methods
    /**
     * @brief Creates the main AI context for a player bot
     * @return Owned pointer to the created context
     */
    static AiObjectContext* createAiObjectContext(Player* player, PlayerbotAI* botAI);
    
    /**
     * @brief Creates engines for different bot states
     * @return Owned pointer to the created engine
     */
    static Engine* createCombatEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* aiObjectContext);
    static Engine* createNonCombatEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* aiObjectContext);
    static Engine* createDeadEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* aiObjectContext);

    // Strategy configuration methods
    /**
     * @brief Adds default strategies to different engine types
     */
    static void AddDefaultCombatStrategies(Player* player, PlayerbotAI* const facade, Engine* engine);
    static void AddDefaultNonCombatStrategies(Player* player, PlayerbotAI* const facade, Engine* nonCombatEngine);
    static void AddDefaultDeadStrategies(Player* player, PlayerbotAI* const facade, Engine* deadEngine);

    // Player analysis methods
    /**
     * @brief Methods for analyzing player specs and roles
     */
    static uint8 GetPlayerSpecTab(Player* player);
    static std::map<uint8, uint32> GetPlayerSpecTabs(Player* player);
    static BotRoles GetPlayerRoles(Player* player);
    static std::string GetPlayerSpecName(Player* player);

private:
    // Prevent instantiation of static-only class
    AiFactory() = delete;
    ~AiFactory() = delete;
    AiFactory(const AiFactory&) = delete;
    AiFactory& operator=(const AiFactory&) = delete;
};

#endif // _PLAYERBOT_AIFACTORY_H
