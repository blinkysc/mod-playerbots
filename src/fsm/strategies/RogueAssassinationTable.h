/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_ROGUEASSASSINATIONTABLE_H
#define _PLAYERBOT_ROGUEASSASSINATIONTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Assassination Rogue Action Table
 *
 * Assassination rogues focus on poison damage and sustained DoTs.
 * Key abilities: Mutilate, Envenom, Hunger for Blood, Cold Blood
 * Priority: Hunger for Blood > Slice and Dice > Rupture > Envenom
 */

namespace RogueAssassinationStrategy
{
    constexpr uint16_t STRATEGY_ID = 201;

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
            .cooldownMs = 180000,
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
            .cooldownMs = 90000,
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
            .cooldownMs = 180000,
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
                .minEnergy(1)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 10000,
            .alternatives = {RogueActions::Gouge, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // STEALTH OPENER (Priority 35)
        // ====================================================================

        // Garrote opener (bleed + silence) - Assassination prefers this
        {
            .id = RogueActions::Garrote,
            .name = "garrote",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget | PrimaryState::BehindTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)
                .build(),
            .basePriority = ActionPriority::Move + 5,
            .cooldownMs = 1000,
            .alternatives = {RogueActions::CheapShot, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Cheap Shot opener (stun + combo points)
        {
            .id = RogueActions::CheapShot,
            .name = "cheap shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)
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

        // Cold Blood (guaranteed crit) - use before Envenom
        {
            .id = RogueActions::ColdBlood,
            .name = "cold blood",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minCombo(5)  // Use with 5 CP for max Envenom
                .minEnergy(1)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 180000,
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
                .minEnergy(1)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // BUFFS (Priority 24)
        // ====================================================================

        // Hunger for Blood - maintain 3 stacks
        {
            .id = RogueActions::HungerForBlood,
            .name = "hunger for blood",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .minEnergy(1)  // Costs 15 energy
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 30000,  // Refresh before it falls off
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
                .minCombo(1)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Rupture (bleed DoT) - maintain for Hunger for Blood
        {
            .id = RogueActions::Rupture,
            .name = "rupture",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(4)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 16000,
            .alternatives = {RogueActions::Envenom, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Envenom (poison finisher) - main damage finisher for Assassination
        {
            .id = RogueActions::Envenom,
            .name = "envenom",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(4)
                .minEnergy(1)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {RogueActions::Eviscerate, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Eviscerate (fallback finisher if no poisons)
        {
            .id = RogueActions::Eviscerate,
            .name = "eviscerate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minCombo(5)
                .minEnergy(1)
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
                .minEnergy(1)
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
                .minEnergy(2)
                .build(),
            .basePriority = ActionPriority::High - 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // COMBO BUILDERS (Priority 10-15)
        // ====================================================================

        // Mutilate (main builder for Assassination)
        {
            .id = RogueActions::MutilateMainHand,
            .name = "mutilate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 60 energy
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 0,
            .alternatives = {RogueActions::Backstab, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Backstab (fallback if no daggers in offhand)
        {
            .id = RogueActions::Backstab,
            .name = "backstab",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::BehindTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)  // Costs 60 energy
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 0,
            .alternatives = {RogueActions::SinisterStrike, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Sinister Strike (if can't use Mutilate/Backstab)
        {
            .id = RogueActions::SinisterStrike,
            .name = "sinister strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidClass(ClassState::InStealth)
                .enemyRange(RangeBand::Melee)
                .minEnergy(2)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // STEALTH (Priority 8)
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
        "assassination rogue"
    };

} // namespace RogueAssassinationStrategy

#endif // _PLAYERBOT_ROGUEASSASSINATIONTABLE_H
