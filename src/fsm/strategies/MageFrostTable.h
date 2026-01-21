/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_MAGEFROSTTABLE_H
#define _PLAYERBOT_MAGEFROSTTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Frost Mage Action Table (DPS)
 *
 * Frost Mages focus on control and sustained damage.
 * Key abilities: Frostbolt, Ice Lance, Deep Freeze, Water Elemental
 */

namespace MageFrostStrategy
{
    constexpr uint16_t STRATEGY_ID = 601;

    static const ActionDefinition rules[] = {
        // Emergency
        {
            .id = MageActions::IceBlock,
            .name = "ice block",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 300000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = MageActions::ColdSnap,
            .name = "cold snap",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 480000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Interrupts
        {
            .id = MageActions::Counterspell,
            .name = "counterspell",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 24000,
            .alternatives = {MageActions::DeepFreeze, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs
        {
            .id = MageActions::IceArmor,
            .name = "ice armor",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 1800000,
            .alternatives = {MageActions::FrostArmor, MageActions::MageArmor, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = MageActions::ArcaneIntellect,
            .name = "arcane intellect",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = MageActions::IceBarrier,
            .name = "ice barrier",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Pet
        {
            .id = MageActions::SummonWaterElemental,
            .name = "summon water elemental",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::HasPet)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Cooldowns
        {
            .id = MageActions::IcyVeins,
            .name = "icy veins",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = MageActions::MirrorImage,
            .name = "mirror image",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana management
        {
            .id = MageActions::Evocation,
            .name = "evocation",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 240000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Control
        {
            .id = MageActions::DeepFreeze,
            .name = "deep freeze",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // AoE
        {
            .id = MageActions::Blizzard,
            .name = "blizzard",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Ice Lance (instant, use with procs)
        {
            .id = MageActions::IceLance,
            .name = "ice lance",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Main filler
        {
            .id = MageActions::Frostbolt,
            .name = "frostbolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Frostfire Bolt (for FFB spec)
        {
            .id = MageActions::FrostfireBolt,
            .name = "frostfire bolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Default
        {
            .id = MageActions::Melee,
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
        {
            .id = MageActions::Follow,
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

    constexpr uint16_t RULE_COUNT = sizeof(rules) / sizeof(ActionDefinition);
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "frost mage"};
}

#endif
