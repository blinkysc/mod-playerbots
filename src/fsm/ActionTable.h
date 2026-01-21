/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_ACTIONTABLE_H
#define _PLAYERBOT_ACTIONTABLE_H

#include "BotStateMask.h"
#include <cstdint>
#include <vector>

class PlayerbotAI;
class Event;

// ============================================================================
// ACTION IDENTIFIERS
// ============================================================================

using ActionId = uint16_t;
constexpr ActionId INVALID_ACTION = 0xFFFF;

// Priority tiers (matching existing Strategy.h values)
namespace ActionPriority
{
    constexpr float Idle        = 0.0f;
    constexpr float Default     = 5.0f;
    constexpr float Normal      = 10.0f;
    constexpr float High        = 20.0f;
    constexpr float Move        = 30.0f;
    constexpr float Interrupt   = 40.0f;
    constexpr float Dispel      = 50.0f;
    constexpr float Raid        = 60.0f;
    constexpr float Emergency   = 90.0f;
}

// ============================================================================
// STATE CONDITION - For matching state to action
// ============================================================================

/**
 * Condition for checking if bot state matches requirements.
 * All non-zero masks must pass for condition to match.
 */
struct StateCondition
{
    // Primary layer conditions
    uint64_t primaryRequired;   // These flags must ALL be set
    uint64_t primaryForbidden;  // These flags must ALL be clear
    uint64_t primaryAnyOf;      // At least ONE of these must be set (0 = ignore)

    // Secondary layer conditions
    uint64_t secondaryRequired;
    uint64_t secondaryForbidden;

    // Class-specific conditions
    uint64_t classRequired;
    uint64_t classForbidden;

    // Numeric threshold checks (using getField)
    uint8_t minHealthLevel;     // 0-7, 0 means no check
    uint8_t maxHealthLevel;     // 0-7, 7 means no check
    uint8_t minManaLevel;       // 0-7
    uint8_t maxManaLevel;       // 0-7
    uint8_t minAttackers;       // 0-7
    uint8_t maxAttackers;       // 0-7
    uint8_t minComboPoints;     // 0-5
    uint8_t minRageLevel;       // 0-3
    uint8_t minEnergyLevel;     // 0-3
    uint8_t enemyRangeBand;     // Specific range band required (0xFF = any)

    // Default constructor - matches all states
    StateCondition()
        : primaryRequired(0), primaryForbidden(0), primaryAnyOf(0)
        , secondaryRequired(0), secondaryForbidden(0)
        , classRequired(0), classForbidden(0)
        , minHealthLevel(0), maxHealthLevel(7)
        , minManaLevel(0), maxManaLevel(7)
        , minAttackers(0), maxAttackers(7)
        , minComboPoints(0)
        , minRageLevel(0), minEnergyLevel(0)
        , enemyRangeBand(0xFF)
    {}

    /**
     * Evaluate if state matches this condition.
     * Fast path: most checks are single bitmask operations.
     */
    bool matches(const BotStateVector& state) const
    {
        // Primary layer checks (most common)
        if (primaryRequired && (state.primary & primaryRequired) != primaryRequired)
            return false;
        if (primaryForbidden && (state.primary & primaryForbidden) != 0)
            return false;
        if (primaryAnyOf && (state.primary & primaryAnyOf) == 0)
            return false;

        // Secondary layer checks
        if (secondaryRequired && (state.secondary & secondaryRequired) != secondaryRequired)
            return false;
        if (secondaryForbidden && (state.secondary & secondaryForbidden) != 0)
            return false;

        // Class-specific checks
        if (classRequired && (state.classFlags & classRequired) != classRequired)
            return false;
        if (classForbidden && (state.classFlags & classForbidden) != 0)
            return false;

        // Numeric threshold checks (slightly slower, skip if default)
        uint8_t healthLevel = FSM_HEALTH_LEVEL(state);
        if (healthLevel < minHealthLevel || healthLevel > maxHealthLevel)
            return false;

        uint8_t manaLevel = FSM_MANA_LEVEL(state);
        if (manaLevel < minManaLevel || manaLevel > maxManaLevel)
            return false;

        uint8_t attackers = FSM_ATTACKER_COUNT(state);
        if (attackers < minAttackers || attackers > maxAttackers)
            return false;

        if (minComboPoints > 0)
        {
            uint8_t combo = FSM_COMBO_POINTS(state);
            if (combo < minComboPoints)
                return false;
        }

        if (minRageLevel > 0)
        {
            uint8_t rage = state.getField(PrimaryState::RageLevelMask, PrimaryState::RageLevelShift);
            if (rage < minRageLevel)
                return false;
        }

        if (minEnergyLevel > 0)
        {
            uint8_t energy = state.getField(PrimaryState::EnergyLevelMask, PrimaryState::EnergyLevelShift);
            if (energy < minEnergyLevel)
                return false;
        }

        if (enemyRangeBand != 0xFF)
        {
            uint8_t range = FSM_ENEMY_RANGE(state);
            if (range != enemyRangeBand)
                return false;
        }

        return true;
    }
};

// ============================================================================
// ACTION DEFINITION - Single action with state requirements
// ============================================================================

/**
 * Defines when an action can execute and its priority.
 * Note: This is an aggregate type to allow designated initializers.
 */
struct ActionDefinition
{
    ActionId id = INVALID_ACTION;       // Unique action identifier
    const char* name = nullptr;         // Action name (for debugging/logging)
    StateCondition condition = {};      // When this action is valid
    float basePriority = 0.0f;          // Priority (higher = more important)
    uint32_t cooldownMs = 0;            // Minimum time between executions (supports up to ~49 days)
    ActionId alternatives[4] = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION};
};

// ============================================================================
// ACTION TABLE - Collection of actions for a strategy
// ============================================================================

/**
 * Pre-sorted table of actions for a specific strategy/role.
 * Actions are sorted by priority (descending) for fast selection.
 */
struct ActionTable
{
    const ActionDefinition* rules;      // Array of action definitions
    uint16_t ruleCount;                 // Number of rules
    uint16_t strategyId;                // Strategy identifier
    const char* strategyName;           // For debugging

    ActionTable()
        : rules(nullptr), ruleCount(0), strategyId(0), strategyName(nullptr) {}

    ActionTable(const ActionDefinition* r, uint16_t count, uint16_t id, const char* name)
        : rules(r), ruleCount(count), strategyId(id), strategyName(name) {}
};

// ============================================================================
// ACTION EXECUTOR - Function pointer type for action execution
// ============================================================================

// Execute the action, returns true on success
using ActionExecutor = bool(*)(PlayerbotAI* ai, const BotStateVector& state);

// Check if action is possible (resources, range, etc.)
using ActionPossibleCheck = bool(*)(PlayerbotAI* ai, const BotStateVector& state);

// Check if action is useful (tactical decision)
using ActionUsefulCheck = bool(*)(PlayerbotAI* ai, const BotStateVector& state);

// ============================================================================
// ACTION ENTRY - Runtime action registration
// ============================================================================

/**
 * Links ActionId to execution functions.
 * Allows hybrid approach: data-driven selection, code-based execution.
 */
struct ActionEntry
{
    ActionId id;
    const char* name;                   // For debugging
    ActionExecutor executor;            // Main execute function
    ActionPossibleCheck isPossible;     // nullptr = always possible
    ActionUsefulCheck isUseful;         // nullptr = always useful
    uint32_t spellId;                   // For spell-based actions (0 = not a spell)

    ActionEntry()
        : id(INVALID_ACTION), name(nullptr)
        , executor(nullptr), isPossible(nullptr), isUseful(nullptr)
        , spellId(0) {}

    ActionEntry(ActionId id_, const char* name_, ActionExecutor exec,
                ActionPossibleCheck possible = nullptr,
                ActionUsefulCheck useful = nullptr,
                uint32_t spell = 0)
        : id(id_), name(name_)
        , executor(exec), isPossible(possible), isUseful(useful)
        , spellId(spell) {}
};

// ============================================================================
// BUILDER HELPERS - For creating conditions fluently
// ============================================================================

/**
 * Fluent builder for StateCondition.
 * Example:
 *   auto cond = ConditionBuilder()
 *       .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
 *       .forbidPrimary(PrimaryState::Dead)
 *       .minHealth(HealthLevel::Low)
 *       .build();
 */
class ConditionBuilder
{
public:
    ConditionBuilder& requirePrimary(uint64_t flags)
    {
        cond.primaryRequired |= flags;
        return *this;
    }

    ConditionBuilder& forbidPrimary(uint64_t flags)
    {
        cond.primaryForbidden |= flags;
        return *this;
    }

    ConditionBuilder& anyOfPrimary(uint64_t flags)
    {
        cond.primaryAnyOf |= flags;
        return *this;
    }

    ConditionBuilder& requireSecondary(uint64_t flags)
    {
        cond.secondaryRequired |= flags;
        return *this;
    }

    ConditionBuilder& forbidSecondary(uint64_t flags)
    {
        cond.secondaryForbidden |= flags;
        return *this;
    }

    ConditionBuilder& requireClass(uint64_t flags)
    {
        cond.classRequired |= flags;
        return *this;
    }

    ConditionBuilder& forbidClass(uint64_t flags)
    {
        cond.classForbidden |= flags;
        return *this;
    }

    ConditionBuilder& healthRange(uint8_t min, uint8_t max)
    {
        cond.minHealthLevel = min;
        cond.maxHealthLevel = max;
        return *this;
    }

    ConditionBuilder& minHealth(uint8_t level)
    {
        cond.minHealthLevel = level;
        return *this;
    }

    ConditionBuilder& maxHealth(uint8_t level)
    {
        cond.maxHealthLevel = level;
        return *this;
    }

    ConditionBuilder& manaRange(uint8_t min, uint8_t max)
    {
        cond.minManaLevel = min;
        cond.maxManaLevel = max;
        return *this;
    }

    ConditionBuilder& minMana(uint8_t level)
    {
        cond.minManaLevel = level;
        return *this;
    }

    ConditionBuilder& maxMana(uint8_t level)
    {
        cond.maxManaLevel = level;
        return *this;
    }

    ConditionBuilder& minAttackers(uint8_t count)
    {
        cond.minAttackers = count;
        return *this;
    }

    ConditionBuilder& maxAttackers(uint8_t count)
    {
        cond.maxAttackers = count;
        return *this;
    }

    ConditionBuilder& minCombo(uint8_t points)
    {
        cond.minComboPoints = points;
        return *this;
    }

    ConditionBuilder& minRage(uint8_t level)
    {
        cond.minRageLevel = level;
        return *this;
    }

    ConditionBuilder& minEnergy(uint8_t level)
    {
        cond.minEnergyLevel = level;
        return *this;
    }

    ConditionBuilder& enemyRange(uint8_t band)
    {
        cond.enemyRangeBand = band;
        return *this;
    }

    StateCondition build() const { return cond; }

private:
    StateCondition cond;
};

// Shorthand for common conditions
namespace CommonConditions
{
    // In combat with valid target
    inline StateCondition InCombatWithTarget()
    {
        return ConditionBuilder()
            .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
            .forbidPrimary(PrimaryState::Dead | PrimaryState::InvalidTarget)
            .build();
    }

    // Low health emergency
    inline StateCondition CriticalHealth()
    {
        return ConditionBuilder()
            .forbidPrimary(PrimaryState::Dead)
            .maxHealth(HealthLevel::Critical)
            .build();
    }

    // Out of combat
    inline StateCondition OutOfCombat()
    {
        return ConditionBuilder()
            .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead)
            .build();
    }

    // Target in melee range
    inline StateCondition TargetInMelee()
    {
        return ConditionBuilder()
            .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
            .forbidPrimary(PrimaryState::Dead)
            .enemyRange(RangeBand::Melee)
            .build();
    }

    // Target casting (interruptable)
    inline StateCondition TargetCasting()
    {
        return ConditionBuilder()
            .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
            .forbidPrimary(PrimaryState::Dead)
            .build();
    }
}

#endif // _PLAYERBOT_ACTIONTABLE_H
