/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARRIORFURYTABLE_H
#define _PLAYERBOT_WARRIORFURYTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Fury Warrior Action Table - Proof of Concept
 *
 * This demonstrates the bitmask FSM approach for a single spec.
 * Actions are sorted by priority (highest first) for efficient selection.
 */

namespace WarriorFuryStrategy
{
    constexpr uint16_t STRATEGY_ID = 100;

    // ========================================================================
    // ACTION DEFINITIONS - Sorted by priority (descending)
    // ========================================================================

    static const ActionDefinition rules[] = {
        // ====================================================================
        // EMERGENCY (Priority 90)
        // ====================================================================

        // Enraged Regeneration when critical health
        {
            .id = WarriorActions::EnragedRegeneration,
            .name = "enraged regeneration",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Critical)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,  // 3 min CD
            .alternatives = {WarriorActions::LastStand, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Last Stand as emergency fallback
        {
            .id = WarriorActions::LastStand,
            .name = "last stand",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // INTERRUPTS (Priority 40)
        // ====================================================================

        // Pummel when target casting (requires Berserker Stance)
        {
            .id = WarriorActions::Pummel,
            .name = "pummel",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 10000,
            .alternatives = {WarriorActions::HeroicThrow, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MOVEMENT (Priority 30)
        // ====================================================================

        // Charge when out of melee (Battle Stance)
        {
            .id = WarriorActions::Charge,
            .name = "charge",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Moving)
                .requireClass(ClassState::BattleStance)
                .enemyRange(RangeBand::Mid)  // 8-25 yards
                .build(),
            .basePriority = ActionPriority::Move,
            .cooldownMs = 15000,
            .alternatives = {WarriorActions::Intercept, WarriorActions::ReachMelee, INVALID_ACTION, INVALID_ACTION}
        },

        // Intercept when out of melee (Berserker Stance)
        {
            .id = WarriorActions::Intercept,
            .name = "intercept",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Moving)
                .requireClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Mid)
                .minRage(1)  // Costs 10 rage
                .build(),
            .basePriority = ActionPriority::Move - 1,
            .cooldownMs = 30000,
            .alternatives = {WarriorActions::ReachMelee, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Reach melee when out of range
        {
            .id = WarriorActions::ReachMelee,
            .name = "reach melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Close)  // > melee but close
                .build(),
            .basePriority = ActionPriority::Move - 5,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // BUFFS (Priority 25)
        // ====================================================================

        // Berserker Stance for Fury (switch to it)
        {
            .id = WarriorActions::BerserkerStance,
            .name = "berserker stance",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Melee)  // Only switch when in melee
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 1000,  // GCD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Battle Shout buff
        {
            .id = WarriorActions::BattleShout,
            .name = "battle shout",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                // TODO: Check if buff is missing
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 120000,  // 2 min duration
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Death Wish offensive cooldown
        {
            .id = WarriorActions::DeathWish,
            .name = "death wish",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)  // Don't use when low HP
                .minRage(1)  // Costs 10 rage
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 180000,  // 3 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Recklessness (crit cooldown)
        {
            .id = WarriorActions::Recklessness,
            .name = "recklessness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 300000,  // 5 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // PROC REACTIONS (Priority 20)
        // ====================================================================

        // Victory Rush when available (after killing blow)
        {
            .id = WarriorActions::VictoryRush,
            .name = "victory rush",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                // TODO: Check for Victory Rush proc
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // AOE (Priority 18-19)
        // ====================================================================

        // Whirlwind when 3+ targets
        {
            .id = WarriorActions::WhirlwindFury,
            .name = "whirlwind",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Melee)
                .minAttackers(3)
                .minRage(2)  // Costs 25 rage
                .build(),
            .basePriority = ActionPriority::High - 1,
            .cooldownMs = 8000,
            .alternatives = {WarriorActions::Cleave, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Cleave when 2+ targets
        {
            .id = WarriorActions::Cleave,
            .name = "cleave",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minAttackers(2)
                .minRage(2)  // Costs 20 rage
                .build(),
            .basePriority = ActionPriority::High - 2,
            .cooldownMs = 0,  // Next melee
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MAIN ROTATION (Priority 10-15)
        // ====================================================================

        // Execute when target low HP
        {
            .id = WarriorActions::Execute,
            .name = "execute",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(ClassState::BattleStance | ClassState::BerserkerStance)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // Costs 15+ rage
                // TODO: Check target health < 20%
                .build(),
            .basePriority = ActionPriority::Normal + 5,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Bloodthirst (main Fury ability)
        {
            .id = WarriorActions::Bloodthirst,
            .name = "bloodthirst",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // Costs 30 rage at 80
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 4000,
            .alternatives = {WarriorActions::HeroicStrike, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Heroic Strike (rage dump)
        {
            .id = WarriorActions::HeroicStrike,
            .name = "heroic strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(2)  // Costs 15 rage, use when high rage
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 0,  // Next melee
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Rend (maintain bleed)
        {
            .id = WarriorActions::Rend,
            .name = "rend",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BattleStance | ClassState::DefensiveStance)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // Costs 10 rage
                // TODO: Check if Rend missing from target
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 15000,  // Rend duration
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Hamstring (kite/snare)
        {
            .id = WarriorActions::Hamstring,
            .name = "hamstring",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetFleeing)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // Costs 10 rage
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DEBUFFS (Priority 8-9)
        // ====================================================================

        // Demoralizing Shout
        {
            .id = WarriorActions::DemoralizingShout,
            .name = "demoralizing shout",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .minRage(1)  // Costs 10 rage
                // TODO: Check if debuff missing
                .build(),
            .basePriority = ActionPriority::Normal - 1,
            .cooldownMs = 30000,  // Duration
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Thunder Clap (slow attack speed)
        {
            .id = WarriorActions::ThunderClap,
            .name = "thunder clap",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(ClassState::BattleStance | ClassState::DefensiveStance)
                .minRage(2)  // Costs 20 rage
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::Normal - 2,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DEFAULT (Priority 5)
        // ====================================================================

        // Basic melee attack
        {
            .id = WarriorActions::Melee,
            .name = "melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Default,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Follow when out of combat
        {
            .id = WarriorActions::Follow,
            .name = "follow",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead)
                .requireSecondary(SecondaryState::FarFromMaster)
                .build(),
            .basePriority = ActionPriority::Default - 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
    };

    // Count of rules
    constexpr uint16_t RULE_COUNT = sizeof(rules) / sizeof(ActionDefinition);

    // Action table instance
    static const ActionTable table = {
        rules,
        RULE_COUNT,
        STRATEGY_ID,
        "fury warrior"
    };

} // namespace WarriorFuryStrategy

#endif // _PLAYERBOT_WARRIORFURYTABLE_H
