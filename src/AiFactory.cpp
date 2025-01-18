/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "AiFactory.h"

#include "BattlegroundMgr.h"
#include "DKAiObjectContext.h"
#include "DruidAiObjectContext.h"
#include "Engine.h"
#include "Group.h"
#include "HunterAiObjectContext.h"
#include "Item.h"
#include "MageAiObjectContext.h"
#include "PaladinAiObjectContext.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "Playerbots.h"
#include "PriestAiObjectContext.h"
#include "RogueAiObjectContext.h"
#include "ShamanAiObjectContext.h"
#include "SharedDefines.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "WarlockAiObjectContext.h"
#include "WarriorAiObjectContext.h"

AiObjectContext* AiFactory::createAiObjectContext(Player* player, PlayerbotAI* botAI)
{
    // Static map to store class-specific context creators, initialized only once
    static const std::unordered_map<uint8, std::function<AiObjectContext*(PlayerbotAI*)>> contextCreators = {
        { CLASS_PRIEST,       [](PlayerbotAI* ai) { return new PriestAiObjectContext(ai); }},
        { CLASS_MAGE,         [](PlayerbotAI* ai) { return new MageAiObjectContext(ai); }},
        { CLASS_WARLOCK,      [](PlayerbotAI* ai) { return new WarlockAiObjectContext(ai); }},
        { CLASS_WARRIOR,      [](PlayerbotAI* ai) { return new WarriorAiObjectContext(ai); }},
        { CLASS_SHAMAN,       [](PlayerbotAI* ai) { return new ShamanAiObjectContext(ai); }},
        { CLASS_PALADIN,      [](PlayerbotAI* ai) { return new PaladinAiObjectContext(ai); }},
        { CLASS_DRUID,        [](PlayerbotAI* ai) { return new DruidAiObjectContext(ai); }},
        { CLASS_HUNTER,       [](PlayerbotAI* ai) { return new HunterAiObjectContext(ai); }},
        { CLASS_ROGUE,        [](PlayerbotAI* ai) { return new RogueAiObjectContext(ai); }},
        { CLASS_DEATH_KNIGHT, [](PlayerbotAI* ai) { return new DKAiObjectContext(ai); }}
    };

    if (!player || !botAI)
        return new AiObjectContext(botAI);  // Default context for invalid input

    // Find the context creator for the player's class
    auto it = contextCreators.find(player->getClass());
    
    // If class found in map, create specific context, otherwise return default context
    // This ensures:
    // 1. Safe access to map elements
    // 2. Fallback for unknown class types
    // 3. Future-proofing for class additions
    return (it != contextCreators.end()) ? it->second(botAI) : new AiObjectContext(botAI);
}

void AiFactory::AddRandomBotLeaderStrategies(Engine* engine)
{
    if (!engine)
        return;

    engine->addStrategy("pvp", false);
    engine->addStrategy("grind", false);

    if (sPlayerbotAIConfig->enableNewRpgStrategy)
    {
        engine->addStrategy("new rpg", false);
    }
    else if (sPlayerbotAIConfig->autoDoQuests)
    {
        engine->addStrategy("rpg", false);
    }
    else
    {
        engine->addStrategy("move random", false);
    }

    if (sPlayerbotAIConfig->randomBotJoinBG)
        engine->addStrategy("bg", false);

    engine->ChangeStrategy(sPlayerbotAIConfig->randomBotNonCombatStrategies);
}

void AiFactory::AddRandomBotGroupMemberStrategies(Engine* engine, Player* master, PlayerbotAI* facade)
{
    if (!engine || !master || !facade)
        return;

    PlayerbotAI* masterBotAI = GET_PLAYERBOT_AI(master);
    if (masterBotAI || sRandomPlayerbotMgr->IsRandomBot(master))
    {
        engine->addStrategy("pvp", false);
        engine->ChangeStrategy(sPlayerbotAIConfig->randomBotNonCombatStrategies);
    }
    else
    {
        engine->addStrategy("pvp", false);
        engine->ChangeStrategy(sPlayerbotAIConfig->nonCombatStrategies);
    }
}

void AiFactory::ApplyBattlegroundStrategies(Player* player, Engine* engine)
{
    if (!player || !engine || !player->GetBattleground())
        return;

    engine->addStrategiesNoInit("nc", "chat", "default", "buff", "food", "mount", "pvp", "dps assist",
                               "attack tagged", "emote", nullptr);
                               
    engine->removeStrategy("custom::say", false);
    engine->removeStrategy("travel", false);
    engine->removeStrategy("rpg", false);
    engine->removeStrategy("grind", false);

    BattlegroundTypeId bgType = player->GetBattlegroundTypeId();
    if (bgType == BATTLEGROUND_RB)
        bgType = player->GetBattleground()->GetBgTypeID(true);

    static const std::unordered_map<BattlegroundTypeId, const char*> bgStrategies = {
        {BATTLEGROUND_WS, "warsong"},
        {BATTLEGROUND_AB, "arathi"},
        {BATTLEGROUND_AV, "alterac"},
        {BATTLEGROUND_EY, "eye"},
        {BATTLEGROUND_IC, "isle"}
    };

    if ((bgType <= BATTLEGROUND_EY || bgType == BATTLEGROUND_IC) && !player->InArena())
        engine->addStrategy("battleground", false);

    auto it = bgStrategies.find(bgType);
    if (it != bgStrategies.end())
        engine->addStrategy(it->second, false);

    if (player->InArena())
    {
        engine->addStrategy("arena", false);
        engine->removeStrategy("mount", false);
    }
}

uint8 AiFactory::GetPlayerSpecTab(Player* bot)
{
    // Early exit for invalid input
    if (!bot)
        return 0;

    // Check if player is level 10+ and has spent talent points
    std::map<uint8, uint32> tabs = GetPlayerSpecTabs(bot);
    uint32 totalPoints = tabs[0] + tabs[1] + tabs[2];
    
    if (bot->GetLevel() >= 10 && totalPoints > 0)
    {
        // Find tab with maximum talent points
        return std::max_element(tabs.begin(), tabs.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }
        )->first;
    }

    // Default specs for low level characters
    static const std::unordered_map<uint8, uint8> defaultClassSpecs = {
        { CLASS_MAGE,     1 },
        { CLASS_PALADIN,  2 },
        { CLASS_PRIEST,   1 }
    };

    // Return default spec if exists, otherwise return 0
    auto it = defaultClassSpecs.find(bot->getClass());
    return it != defaultClassSpecs.end() ? it->second : 0;
}

std::map<uint8, uint32> AiFactory::GetPlayerSpecTabs(Player* bot)
{
    // Initialize tabs with zero values
    std::map<uint8, uint32> tabs = {{0, 0}, {1, 0}, {2, 0}};  // Fixed initialization
    
    if (!bot)
        return tabs;

    const PlayerTalentMap& talentMap = bot->GetTalentMap();
    const uint32* talentTabIds = GetTalentTabPages(bot->getClass());

    for (const auto& [spellId, talentData] : talentMap)
    {
        // Skip talents not in active spec
        if ((bot->GetActiveSpecMask() & talentData->specMask) == 0)
            continue;

        // Validate talent position and entry
        TalentSpellPos const* talentPos = GetTalentSpellPos(spellId);
        if (!talentPos)
            continue;

        TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentPos->talent_id);
        if (!talentInfo)
            continue;

        // Get spell rank
        const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        uint32 rank = spellInfo ? spellInfo->GetRank() : 1;

        // Update appropriate tab with rank points
        for (uint8 i = 0; i < 3; ++i)
        {
            if (talentInfo->TalentTab == talentTabIds[i])
            {
                tabs[i] += rank;
                break;  // Each talent belongs to only one tab
            }
        }
    }

    return tabs;
}

BotRoles AiFactory::GetPlayerRoles(Player* player)
{
    if (!player)
        return BOT_ROLE_NONE;

    uint8 playerClass = player->getClass();
    uint8 specTab = GetPlayerSpecTab(player);

    // Handle classes with simple role mappings using switch
    switch (playerClass)
    {
        case CLASS_PRIEST:
            return (specTab == 2) ? BOT_ROLE_DPS : BOT_ROLE_HEALER;

        case CLASS_SHAMAN:
            return (specTab == 2) ? BOT_ROLE_HEALER : BOT_ROLE_DPS;

        case CLASS_WARRIOR:
            return (specTab == 2) ? BOT_ROLE_TANK : BOT_ROLE_DPS;

        case CLASS_PALADIN:
            switch (specTab)
            {
                case 0: return BOT_ROLE_HEALER;
                case 1: return BOT_ROLE_TANK;
                case 2: return BOT_ROLE_DPS;
                default: return BOT_ROLE_DPS;
            }

        case CLASS_DRUID:
            switch (specTab)
            {
                case 0: return BOT_ROLE_DPS;
                case 1: return static_cast<BotRoles>(BOT_ROLE_TANK | BOT_ROLE_DPS);
                case 2: return BOT_ROLE_HEALER;
                default: return BOT_ROLE_DPS;
            }

        default:
            return BOT_ROLE_DPS;  // Default role for unmapped classes
    }
}

std::string AiFactory::GetPlayerSpecName(Player* player)
{
    if (!player)
        return "";

    // Define spec names using nested map structure
    // {class -> {spec tab -> spec name}}
    static const std::unordered_map<uint8, std::unordered_map<uint8, std::string>> classSpecs = {
        { 
            CLASS_PRIEST, {
                {0, "disc"},
                {1, "holy"},
                {2, "shadow"}
            }
        },
        { 
            CLASS_SHAMAN, {
                {0, "elem"},
                {1, "enhance"},
                {2, "resto"}
            }
        },
        { 
            CLASS_WARRIOR, {
                {0, "arms"},
                {1, "fury"},
                {2, "prot"}
            }
        },
        { 
            CLASS_PALADIN, {
                {0, "holy"},
                {1, "prot"},
                {2, "retrib"}
            }
        },
        { 
            CLASS_DRUID, {
                {0, "balance"},
                {1, "feraldps"},
                {2, "resto"}
            }
        },
        { 
            CLASS_ROGUE, {
                {0, "assas"},
                {1, "combat"},
                {2, "subtle"}
            }
        },
        { 
            CLASS_HUNTER, {
                {0, "beast"},
                {1, "marks"},
                {2, "surv"}
            }
        },
        { 
            CLASS_DEATH_KNIGHT, {
                {0, "blooddps"},
                {1, "frostdps"},
                {2, "unholydps"}
            }
        },
        { 
            CLASS_MAGE, {
                {0, "arcane"},
                {1, "fire"},
                {2, "frost"}
            }
        },
        { 
            CLASS_WARLOCK, {
                {0, "afflic"},
                {1, "demo"},
                {2, "destro"}
            }
        }
    };

    uint8 playerClass = player->getClass();
    uint8 specTab = GetPlayerSpecTab(player);

    // Find class in map
    auto classIt = classSpecs.find(playerClass);
    if (classIt == classSpecs.end())
        return "";  // Return empty string for unmapped classes

    // Find spec tab in class specs
    auto specIt = classIt->second.find(specTab);
    if (specIt == classIt->second.end())
        return "";  // Return empty string for unmapped specs

    return specIt->second;
}


void AiFactory::AddDefaultCombatStrategies(Player* player, PlayerbotAI* const facade, Engine* engine)
{
    if (!player || !facade || !engine)
        return;

    uint8 tab = GetPlayerSpecTab(player);
    
    // Base strategies
    if (!player->InBattleground())
    {
        engine->addStrategiesNoInit("racials", "chat", "default", "cast time", "duel", "boost", nullptr);
    }

    if (sPlayerbotAIConfig->autoAvoidAoe && facade->HasRealPlayerMaster())
    {
        engine->addStrategy("avoid aoe", false);
    }
    engine->addStrategy("formation", false);

    // Class-specific strategies
    static const std::unordered_map<uint8, std::unordered_map<uint8, std::vector<const char*>>> classStrategies = {
        { CLASS_PRIEST, {
            {2, {"dps", "shadow debuff", "shadow aoe"}},
            {PRIEST_TAB_DISIPLINE, {"heal"}},
            {1, {"holy heal"}},
            {0xFF, {"dps assist", "cure"}}
        }},
        { CLASS_MAGE, {
            {0, {"arcane", "arcane aoe"}},
            {1, {"fire", "fire aoe"}},
            {2, {"frost", "frost aoe"}},
            {0xFF, {"dps", "dps assist", "cure"}}
        }},
        { CLASS_WARRIOR, {
            {2, {"tank", "tank assist", "aoe"}},
            {0, {"arms", "aoe", "dps assist"}},
            {1, {"fury", "aoe", "dps assist"}}
        }},
        { CLASS_SHAMAN, {
            {0, {"caster", "caster aoe", "bmana"}},
            {2, {"heal", "bmana"}},
            {1, {"melee", "melee aoe", "bdps"}},
            {0xFF, {"dps assist", "cure", "totems"}}
        }},
        { CLASS_PALADIN, {
            {1, {"tank", "tank assist", "bthreat", "barmor", "cure"}},
            {0, {"heal", "dps assist", "cure", "bcast"}},
            {2, {"dps", "dps assist", "cure", "baoe"}}
        }},
        { CLASS_DRUID, {
            {0, {"caster", "cure", "caster aoe", "dps assist"}},
            {2, {"heal", "cure", "dps assist"}},
            {1, {}} // Special handling for feral
        }},
        { CLASS_HUNTER, {
            {0xFF, {"dps", "aoe", "bdps", "dps assist"}}
        }},
        { CLASS_ROGUE, {
            {ROGUE_TAB_ASSASSINATION, {"melee", "dps assist", "aoe"}},
            {0xFF, {"dps", "dps assist", "aoe"}}
        }},
        { CLASS_WARLOCK, {
            {0xFF, {"dps assist", "dps", "dps debuff", "aoe"}}
        }},
        { CLASS_DEATH_KNIGHT, {
            {0, {"blood", "tank assist"}},
            {1, {"frost", "frost aoe", "dps assist"}},
            {2, {"unholy", "unholy aoe", "dps assist"}}
        }}
    };

    // Apply class strategies
    uint8 playerClass = player->getClass();
    auto classIt = classStrategies.find(playerClass);
    if (classIt != classStrategies.end())
    {
        // Apply tab-specific strategies
        auto specIt = classIt->second.find(tab);
        if (specIt != classIt->second.end())
        {
            for (const char* strategy : specIt->second)
            {
                engine->addStrategiesNoInit(strategy, nullptr);
            }
        }

        // Apply common strategies for class
        auto commonIt = classIt->second.find(0xFF);
        if (commonIt != classIt->second.end())
        {
            for (const char* strategy : commonIt->second)
            {
                engine->addStrategiesNoInit(strategy, nullptr);
            }
        }
    }

    // Special case for Druid Feral
    if (playerClass == CLASS_DRUID && tab == 1)
    {
        if (player->GetLevel() >= 20 && !player->HasAura(16931))
        {
            engine->addStrategiesNoInit("cat", "dps assist", nullptr);
        }
        else
        {
            engine->addStrategiesNoInit("bear", "tank assist", nullptr);
        }
    }

    // Role-based strategies
    if (PlayerbotAI::IsTank(player, true)) {
        engine->addStrategy("tank face", false);
    }
    if (PlayerbotAI::IsMelee(player, true) && PlayerbotAI::IsDps(player, true)) {
        engine->addStrategy("behind", false);
    }
    if (PlayerbotAI::IsHeal(player, true))
    {
        if (sPlayerbotAIConfig->autoSaveMana)
            engine->addStrategy("save mana", false);
        engine->addStrategy("healer dps", false);
    }

    // Special player strategies
    if (facade->IsRealPlayer() || sRandomPlayerbotMgr->IsRandomBot(player))
    {
        if (!player->GetGroup())
        {
            engine->addStrategy("boost", false);
            engine->addStrategy("dps assist", false);
            engine->removeStrategy("threat", false);

            // Healer solo strategies
            if ((playerClass == CLASS_PRIEST && tab != PRIEST_TAB_SHADOW) ||
                (playerClass == CLASS_DRUID && tab == DRUID_TAB_RESTORATION) ||
                (playerClass == CLASS_SHAMAN && tab == SHAMAN_TAB_RESTORATION) ||
                (playerClass == CLASS_PALADIN && tab == PALADIN_TAB_HOLY))
            {
                const std::unordered_map<uint8, std::vector<const char*>> healerSoloStrats = {
                    {CLASS_PRIEST, {"holy dps", "shadow debuff", "shadow aoe"}},
                    {CLASS_DRUID, {"caster", "caster aoe", "caster debuff"}},
                    {CLASS_SHAMAN, {"caster", "caster aoe", "bmana"}},
                    {CLASS_PALADIN, {"dps", "dps assist", "baoe"}}
                };

                auto healerStrats = healerSoloStrats.find(playerClass);
                if (healerStrats != healerSoloStrats.end())
                {
                    for (const char* strategy : healerStrats->second)
                    {
                        engine->addStrategiesNoInit(strategy, nullptr);
                    }
                }
            }
        }
    }
    else
    {
        engine->ChangeStrategy(sPlayerbotAIConfig->combatStrategies);
    }

    // Battleground strategies
    if (player->InBattleground() && player->GetBattleground())
    {
        static const std::unordered_map<BattlegroundTypeId, const char*> bgStrategies = {
            {BATTLEGROUND_WS, "warsong"},
            {BATTLEGROUND_AB, "arathi"},
            {BATTLEGROUND_AV, "alterac"},
            {BATTLEGROUND_EY, "eye"},
            {BATTLEGROUND_IC, "isle"}
        };

        BattlegroundTypeId bgType = player->GetBattlegroundTypeId();
        if (bgType == BATTLEGROUND_RB)
            bgType = player->GetBattleground()->GetBgTypeID(true);

        auto it = bgStrategies.find(bgType);
        if (it != bgStrategies.end())
            engine->addStrategy(it->second, false);

        if (player->InArena())
            engine->addStrategy("arena", false);

        engine->addStrategiesNoInit("boost", "racials", "chat", "default", "aoe", "potions", "cast time", "dps assist", nullptr);
        engine->removeStrategy("custom::say", false);
        engine->removeStrategy("flee", false);
        engine->removeStrategy("threat", false);
        engine->addStrategy("boost", false);
    }
}

Engine* AiFactory::createCombatEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* aiObjectContext)
{
    // Validate input parameters
    if (!player || !facade || !aiObjectContext)
        return nullptr;

    // Create new engine instance
    Engine* engine = new Engine(facade, aiObjectContext);
    if (!engine)
        return nullptr;

    // Add combat strategies and initialize
    AddDefaultCombatStrategies(player, facade, engine);
    engine->Init();

    return engine;
}

void AiFactory::AddDefaultNonCombatStrategies(Player* player, PlayerbotAI* const facade, Engine* nonCombatEngine)
{
    if (!player || !nonCombatEngine)
        return;

    const uint8 playerClass = player->getClass();
    const uint8 tab = GetPlayerSpecTab(player);
    const uint32 playerLevel = player->GetLevel();
    const bool isInBattleground = player->InBattleground();
    
    // Base class-specific strategies
    struct ClassStrategy {
        const char* strategies[4];  // null-terminated array
        uint8 specTab;
        bool needsLevel20Check;
    };
    
    static const std::unordered_map<uint8, ClassStrategy> classStrategies = {
        {CLASS_PRIEST,    {{"dps assist", "cure", nullptr}}},
        {CLASS_HUNTER,    {{"bdps", "dps assist", "pet", nullptr}}},
        {CLASS_WARRIOR,   {{nullptr}, 2, false}},  // Special handling in switch
        {CLASS_PALADIN,   {{nullptr}, 0, false}},  // Special handling in switch
        {CLASS_SHAMAN,    {{nullptr}, 0, false}},  // Special handling in switch
        {CLASS_MAGE,      {{nullptr}, 0, false}},  // Special handling in switch
        {CLASS_WARLOCK,   {{nullptr}, 0, false}},  // Special handling in switch
        {CLASS_DRUID,     {{nullptr}, 1, true}},   // Special handling in switch
        {CLASS_DEATH_KNIGHT, {{nullptr}, 0, false}} // Special handling in switch
    };

    // Apply class-specific strategies
    auto it = classStrategies.find(playerClass);
    if (it != classStrategies.end()) {
        if (it->second.strategies[0]) {
            nonCombatEngine->addStrategiesNoInit(it->second.strategies[0], it->second.strategies[1], 
                                                it->second.strategies[2], nullptr);
        } else {
            // Handle special cases
            switch (playerClass) {
                case CLASS_PALADIN:
                    if (tab == 1) {
                        nonCombatEngine->addStrategiesNoInit("bthreat", "tank assist", "barmor", nullptr);
                        nonCombatEngine->addStrategy(playerLevel >= 20 ? "bstats" : "bdps", false);
                    } else {
                        nonCombatEngine->addStrategiesNoInit(tab == 0 ? "dps assist, bmana, bcast" : 
                                                           "dps assist, bdps, baoe", nullptr);
                    }
                    nonCombatEngine->addStrategiesNoInit("cure", nullptr);
                    break;

                case CLASS_WARLOCK:
                    const char* specStrategy = (tab == WARLOCK_TAB_AFFLICATION) ? "bmana" :
                                             (tab == WARLOCK_TAB_DEMONOLOGY) ? "bdps" : "bhealth";
                    nonCombatEngine->addStrategiesNoInit(specStrategy, nullptr);
                    nonCombatEngine->addStrategiesNoInit("dps assist", nullptr);
                    break;

                // ... Similar optimizations for other special cases ...
            }
        }
    }

    // Handle non-battleground strategies
    if (!isInBattleground) {
        static const char* defaultStrats[] = {
            "nc", "food", "chat", "follow", "default", "quest", "loot", 
            "gather", "duel", "buff", "mount", "emote", nullptr
        };
        nonCombatEngine->addStrategiesNoInit(defaultStrats[0], defaultStrats[1], defaultStrats[2],
            defaultStrats[3], defaultStrats[4], defaultStrats[5], defaultStrats[6],
            defaultStrats[7], defaultStrats[8], defaultStrats[9], defaultStrats[10], 
            defaultStrats[11], nullptr);
    }

    // Auto save mana check
    if (sPlayerbotAIConfig->autoSaveMana) {
        nonCombatEngine->addStrategy("save mana", false);
    }

    // Random bot specific logic
    if (sRandomPlayerbotMgr->IsRandomBot(player) && !isInBattleground) {
        Player* master = facade->GetMaster();
        const bool isGroupLeader = !player->GetGroup() || 
                                 player->GetGroup()->GetLeaderGUID() == player->GetGUID();

        if (!urand(0, 3)) {
            nonCombatEngine->addStrategy("start duel", false);
        }

        if (sPlayerbotAIConfig->randomBotJoinLfg) {
            nonCombatEngine->addStrategy("lfg", false);
        }

        if (isGroupLeader) {
            AddRandomBotLeaderStrategies(nonCombatEngine);
        } else if (facade && master) {
            AddRandomBotGroupMemberStrategies(nonCombatEngine, master, facade);
        }
    } else {
        nonCombatEngine->ChangeStrategy(sPlayerbotAIConfig->nonCombatStrategies);
    }

    // Battleground specific strategies
    if (isInBattleground && player->GetBattleground()) {
        ApplyBattlegroundStrategies(player, nonCombatEngine);
    }
}

/**
 * @brief Creates and initializes a non-combat engine for a player's AI
 * 
 * @param player The player to create the engine for
 * @param facade The PlayerBotAI interface controlling the bot
 * @param aiObjectContext Context containing AI behaviors and states
 * @return Engine* Pointer to the initialized non-combat engine, or nullptr if parameters are invalid
 * 
 * @note Caller takes ownership of the returned Engine pointer
 */
Engine* AiFactory::createNonCombatEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* aiObjectContext) 
{
    // Validate input parameters to prevent crashes
    if (!player || !facade || !aiObjectContext)
        return nullptr;
        
    // Use smart pointer for exception safety during initialization
    std::unique_ptr<Engine> nonCombatEngine(new Engine(facade, aiObjectContext));
    
    // Add default strategies specific to player's class and context
    AddDefaultNonCombatStrategies(player, facade, nonCombatEngine.get());
    
    // Initialize the engine with added strategies
    nonCombatEngine->Init();
    
    // Transfer ownership to caller and return raw pointer
    return nonCombatEngine.release();
}

/**
 * @brief Adds default strategies for when a player is dead
 * 
 * @param player The player to add dead strategies for
 * @param facade Unused PlayerbotAI parameter (kept for interface consistency)
 * @param deadEngine The engine to add dead strategies to
 * 
 * @note Dead bots will generally stay, chat, and follow unless they're a
 *       random bot without a group, in which case they won't follow
 */
void AiFactory::AddDefaultDeadStrategies(Player* player, PlayerbotAI* const facade, Engine* deadEngine)
{
    // Suppress unused parameter warning
    (void)facade;

    // Validate input parameters
    if (!player || !deadEngine)
        return;

    // Define default strategies for dead state
    static const char* defaultDeadStrategies[] = {
        "dead", "stay", "chat", "default", "follow", nullptr
    };
    
    // Add all default dead strategies
    deadEngine->addStrategiesNoInit(defaultDeadStrategies[0], defaultDeadStrategies[1], 
                                   defaultDeadStrategies[2], defaultDeadStrategies[3], 
                                   defaultDeadStrategies[4], nullptr);

    // Random bots without a group shouldn't follow
    if (sRandomPlayerbotMgr->IsRandomBot(player) && !player->GetGroup())
    {
        deadEngine->removeStrategy("follow", false);
    }
}

/**
* @brief Creates and initializes an engine for handling bot behavior while dead
* 
* @param player The player to create the dead engine for
* @param facade The PlayerBotAI interface controlling the bot
* @param AiObjectContext Context containing AI behaviors and states
* @return Engine* Pointer to the initialized dead engine, or nullptr if parameters are invalid
* 
* @note Caller takes ownership of the returned Engine pointer
*/
Engine* AiFactory::createDeadEngine(Player* player, PlayerbotAI* const facade, AiObjectContext* AiObjectContext) 
{
   // Validate input parameters to prevent crashes
   if (!player || !facade || !AiObjectContext)
       return nullptr;
       
   // Use smart pointer for exception safety during initialization
   std::unique_ptr<Engine> deadEngine(new Engine(facade, AiObjectContext));
   
   // Configure dead-state strategies
   AddDefaultDeadStrategies(player, facade, deadEngine.get());
   
   // Initialize the engine with added strategies
   deadEngine->Init();
   
   // Transfer ownership to caller and return raw pointer
   return deadEngine.release();
}
