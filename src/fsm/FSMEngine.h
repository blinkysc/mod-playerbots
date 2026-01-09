/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_FSMENGINE_H
#define _PLAYERBOT_FSMENGINE_H

#include "BotStateMask.h"
#include "ActionTable.h"
#include <vector>
#include <unordered_map>

class PlayerbotAI;
class Player;
class Unit;
class AiObjectContext;

// ============================================================================
// FSM ENGINE - Bitmask-based state machine for bot AI
// ============================================================================

/**
 * Finite State Machine engine that replaces the priority queue-based Engine.
 *
 * Key differences from old Engine:
 * - State is computed once per tick into a bitmask (256 bits)
 * - Action selection is a table lookup, not a queue scan
 * - No string-based lookups at runtime
 * - ~5x faster per tick
 */
class FSMEngine
{
public:
    FSMEngine(PlayerbotAI* ai);
    ~FSMEngine();

    /**
     * Main tick function - replaces Engine::DoNextAction()
     * 1. Updates state bitmask
     * 2. Selects best action from active strategies
     * 3. Executes the action
     * Returns true if an action was executed successfully.
     */
    bool DoNextAction(Unit* target = nullptr, int depth = 0, bool minimal = false);

    /**
     * Update state bitmask from current game state.
     * Called automatically by DoNextAction, but can be called manually
     * to inspect state without executing actions.
     */
    void UpdateState();

    /**
     * Get current state (read-only).
     */
    const BotStateVector& GetState() const { return currentState; }

    /**
     * Add an action table (strategy) to active strategies.
     */
    void AddStrategy(const ActionTable* table);

    /**
     * Remove an action table from active strategies.
     */
    void RemoveStrategy(uint16_t strategyId);

    /**
     * Check if a strategy is active.
     */
    bool HasStrategy(uint16_t strategyId) const;

    /**
     * Clear all active strategies.
     */
    void ClearStrategies();

    /**
     * Register an action executor.
     * Must be called for each action before it can be executed.
     */
    void RegisterAction(const ActionEntry& entry);

    /**
     * Get action entry by ID.
     */
    const ActionEntry* GetActionEntry(ActionId id) const;

    /**
     * Record action cooldown.
     */
    void RecordCooldown(ActionId id, uint32_t durationMs);

    /**
     * Check if action is on cooldown.
     */
    bool IsOnCooldown(ActionId id) const;

    /**
     * Force execute a specific action (for commands).
     */
    bool ExecuteAction(ActionId id);

    /**
     * Get last executed action ID.
     */
    ActionId GetLastAction() const { return lastAction; }

    /**
     * Get previous state (for detecting transitions).
     */
    const BotStateVector& GetPreviousState() const { return previousState; }

private:
    PlayerbotAI* botAI;
    Player* bot;
    AiObjectContext* context;

    // State tracking
    BotStateVector currentState;
    BotStateVector previousState;

    // Active strategy tables
    std::vector<const ActionTable*> activeStrategies;

    // Action registry (ActionId -> ActionEntry)
    std::unordered_map<ActionId, ActionEntry> actionRegistry;

    // Cooldown tracking (ActionId -> expiry time)
    std::unordered_map<ActionId, uint32_t> cooldowns;

    // Last executed action
    ActionId lastAction;
    uint32_t lastActionTime;
    uint32_t lastTimerTick;  // For per-second timer events

    // State update helpers
    void UpdatePrimaryState();
    void UpdateSecondaryState();
    void UpdateClassState();
    void UpdateEventState();

    // Numeric state calculations
    uint8_t CalculateHealthLevel(uint8_t healthPct) const;
    uint8_t CalculateManaLevel(uint8_t manaPct) const;
    uint8_t CalculateRangeBand(float distance) const;

    // Action selection
    ActionId SelectBestAction() const;

    // Action execution
    bool ExecuteActionInternal(ActionId id, const ActionDefinition* def);
};

// ============================================================================
// WARRIOR ACTION IDS (Proof of Concept)
// ============================================================================

namespace WarriorActions
{
    // Generic actions
    constexpr ActionId Melee                = 1;
    constexpr ActionId Shoot                = 2;
    constexpr ActionId Follow               = 3;
    constexpr ActionId ReachMelee           = 4;

    // Stance management
    constexpr ActionId BattleStance         = 10;
    constexpr ActionId DefensiveStance      = 11;
    constexpr ActionId BerserkerStance      = 12;

    // Arms abilities
    constexpr ActionId Charge               = 20;
    constexpr ActionId MortalStrike         = 21;
    constexpr ActionId Overpower            = 22;
    constexpr ActionId Rend                 = 23;
    constexpr ActionId Slam                 = 24;
    constexpr ActionId Execute              = 25;
    constexpr ActionId Bladestorm           = 26;

    // Fury abilities
    constexpr ActionId Bloodthirst          = 30;
    constexpr ActionId WhirlwindFury        = 31;
    constexpr ActionId RagingBlow           = 32;
    constexpr ActionId HeroicStrike         = 33;
    constexpr ActionId Cleave               = 34;
    constexpr ActionId Rampage              = 35;

    // Protection abilities
    constexpr ActionId ShieldSlam           = 40;
    constexpr ActionId Revenge              = 41;
    constexpr ActionId Devastate            = 42;
    constexpr ActionId ShieldBlock          = 43;
    constexpr ActionId Shockwave            = 44;
    constexpr ActionId ConcussionBlow       = 45;

    // Shouts
    constexpr ActionId BattleShout          = 50;
    constexpr ActionId CommandingShout      = 51;
    constexpr ActionId DemoralizingShout    = 52;

    // Defensive cooldowns
    constexpr ActionId ShieldWall           = 60;
    constexpr ActionId LastStand            = 61;
    constexpr ActionId EnragedRegeneration  = 62;

    // Interrupts
    constexpr ActionId Pummel               = 70;
    constexpr ActionId ShieldBash           = 71;
    constexpr ActionId HeroicThrow          = 72;

    // Utility
    constexpr ActionId Taunt                = 80;
    constexpr ActionId MockingBlow          = 81;
    constexpr ActionId Intervene            = 82;
    constexpr ActionId Intercept            = 83;
    constexpr ActionId Hamstring            = 84;
    constexpr ActionId SunderArmor          = 85;
    constexpr ActionId ThunderClap          = 86;
    constexpr ActionId VictoryRush          = 87;
    constexpr ActionId Berserker            = 88;
    constexpr ActionId Recklessness         = 89;
    constexpr ActionId DeathWish            = 90;

    // Additional Arms/Prot abilities
    constexpr ActionId SweepingStrikes      = 91;
    constexpr ActionId ChallengingShout     = 93;
    constexpr ActionId Disarm               = 94;
    constexpr ActionId SpellReflection      = 95;
}

#endif // _PLAYERBOT_FSMENGINE_H
