/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_ROGUECOMBATTABLE_H
#define _PLAYERBOT_ROGUECOMBATTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Combat Rogue Action Table
 *
 * Combat rogues focus on sustained damage with Sinister Strike as primary builder.
 * Key abilities: Blade Flurry, Adrenaline Rush, Killing Spree
 * Finisher priority: Slice and Dice > Rupture > Eviscerate
 */

namespace RogueCombatStrategy
{
    constexpr uint16_t STRATEGY_ID = 200;

    // ========================================================================
    // ACTION DEFINITIONS - Sorted by priority (descending)
    // ========================================================================

    static const ActionDefinition rules[] = {
        // ====================================================================
        // EMERGENCY (Priority 90)
        // ====================================================================

        // Evasion when taking heavy damage
        {
            .id = RogueActions::Evasion,
            .name = "evasion",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,  // 3 min CD
            .alternatives = {RogueActions::CloakOfShadows, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Cloak of Shadows for magic damage/debuffs
        {
            .id = RogueActions::CloakOfShadows,
            .name = "cloak of shadows",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 90000,  // 1.5 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Vanish for emergency escape
        {
            .id = RogueActions::Vanish,
            .name = "vanish",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 2,
            .cooldownMs = 180000,  // 3 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // INTERRUPTS (Priority 40)
        // ====================================================================

        // Kick to interrupt
        {
            .id = RogueActions::Kick,
            .name = "kick",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(1)  // Costs 25 energy
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 10000,
            .alternatives = {RogueActions::Gouge, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Gouge as backup interrupt
        {
            .id = RogueActions::Gouge,
            .name = "gouge",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 45 energy
                .build(),
            .basePriority = ActionPriority::Interrupt - 1,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // STEALTH OPENER (Priority 35)
        // ====================================================================

        // Cheap Shot opener (stun + combo points)
        {
            .id = RogueActions::CheapShot,
            .name = "cheap shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 60 energy
                .build(),
            .basePriority = ActionPriority::Move + 5,
            .cooldownMs = 1000,  // GCD
            .alternatives = {RogueActions::Garrote, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Garrote opener (bleed + silence)
        {
            .id = RogueActions::Garrote,
            .name = "garrote",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget | PrimaryState::BehindTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 50 energy
                .build(),
            .basePriority = ActionPriority::Move + 4,
            .cooldownMs = 1000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MOVEMENT (Priority 30)
        // ====================================================================

        // Sprint to close gap
        {
            .id = RogueActions::Sprint,
            .name = "sprint",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Far)
                .build(),
            .basePriority = ActionPriority::Move,
            .cooldownMs = 180000,
            .alternatives = {RogueActions::ReachMelee, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Reach melee range
        {
            .id = RogueActions::ReachMelee,
            .name = "reach melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Close)
                .build(),
            .basePriority = ActionPriority::Move - 5,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // COOLDOWNS (Priority 25-28)
        // ====================================================================

        // Killing Spree (Combat 51-point talent)
        {
            .id = RogueActions::KillingSpree,
            .name = "killing spree",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minHealth(HealthLevel::Medium)  // Don't use when low HP
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 120000,  // 2 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Adrenaline Rush (energy regen cooldown)
        {
            .id = RogueActions::AdrenalineRush,
            .name = "adrenaline rush",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 180000,  // 3 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Blade Flurry (cleave damage)
        {
            .id = RogueActions::BladeFlurry,
            .name = "blade flurry",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minAttackers(2)  // Only use with multiple targets
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 120000,  // 2 min CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Tricks of the Trade
        {
            .id = RogueActions::TricksOfTheTrade,
            .name = "tricks of the trade",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minEnergy(1)  // Costs 15 energy
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 30000,  // 30 sec CD
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // FINISHERS (Priority 18-22)
        // ====================================================================

        // Slice and Dice (attack speed buff) - maintain always
        {
            .id = RogueActions::SliceAndDice,
            .name = "slice and dice",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(1)  // Use at 1+ for initial, refresh at 5
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,  // Refresh as needed
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Rupture (bleed DoT) - use at 5 combo points
        {
            .id = RogueActions::Rupture,
            .name = "rupture",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(4)  // Use at 4-5 CP
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 16000,  // Rupture duration at 5 CP
            .alternatives = {RogueActions::Eviscerate, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Eviscerate (direct damage finisher)
        {
            .id = RogueActions::Eviscerate,
            .name = "eviscerate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(5)  // Use at 5 CP
                .minEnergy(1)  // Costs 35 energy
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Kidney Shot (stun finisher) - for control
        {
            .id = RogueActions::KidneyShot,
            .name = "kidney shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(4)
                .minEnergy(1)  // Costs 25 energy
                .build(),
            .basePriority = ActionPriority::High - 1,
            .cooldownMs = 20000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // AOE (Priority 17)
        // ====================================================================

        // Fan of Knives for 3+ targets
        {
            .id = RogueActions::FanOfKnives,
            .name = "fan of knives",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minAttackers(3)
                .minEnergy(2)  // Costs 50 energy
                .build(),
            .basePriority = ActionPriority::High - 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // COMBO BUILDERS (Priority 10-15)
        // ====================================================================

        // Sinister Strike (main builder for Combat)
        {
            .id = RogueActions::SinisterStrike,
            .name = "sinister strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 40 energy
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // STEALTH (Priority 8) - Use before pull if not in combat
        // ====================================================================

        // Stealth before combat
        {
            .id = RogueActions::Stealth,
            .name = "stealth",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead | PrimaryState::Mounted)
                .requirePrimary(PrimaryState::HasTarget)
                .forbidClass(ClassState::InStealth)
                .build(),
            .basePriority = ActionPriority::Normal - 2,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DEFAULT (Priority 5)
        // ====================================================================

        // Basic melee attack
        {
            .id = RogueActions::Melee,
            .name = "melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Default,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Follow when out of combat
        {
            .id = RogueActions::Follow,
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
        "combat rogue"
    };

} // namespace RogueCombatStrategy

#endif // _PLAYERBOT_ROGUECOMBATTABLE_H
