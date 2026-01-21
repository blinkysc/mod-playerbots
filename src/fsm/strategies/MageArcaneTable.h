/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_MAGEARCANETABLE_H
#define _PLAYERBOT_MAGEARCANETABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Arcane Mage Action Table (DPS)
 *
 * Arcane Mages focus on mana management and burst windows.
 * Key abilities: Arcane Blast, Arcane Missiles, Arcane Barrage, Arcane Power
 */

namespace MageArcaneStrategy
{
    constexpr uint16_t STRATEGY_ID = 602;

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
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs
        {
            .id = MageActions::MageArmor,
            .name = "mage armor",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 1800000,
            .alternatives = {MageActions::MoltenArmor, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
        // Cooldowns - Arcane Power + Presence of Mind combo
        {
            .id = MageActions::ArcanePower,
            .name = "arcane power",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .minMana(ManaLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 120000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = MageActions::PresenceOfMind,
            .name = "presence of mind",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 120000,
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
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana management - Critical for Arcane
        {
            .id = MageActions::Evocation,
            .name = "evocation",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 240000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // AoE
        {
            .id = MageActions::ArcaneExplosion,
            .name = "arcane explosion",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Arcane Barrage - use to clear Arcane Blast debuff when low mana
        {
            .id = MageActions::ArcaneBarrage,
            .name = "arcane barrage",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 3000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Arcane Missiles - proc based, use when available
        {
            .id = MageActions::ArcaneMissiles,
            .name = "arcane missiles",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Main filler - Arcane Blast
        {
            .id = MageActions::ArcaneBlast,
            .name = "arcane blast",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {MageActions::Fireball, MageActions::Frostbolt, INVALID_ACTION, INVALID_ACTION}
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "arcane mage"};
}

#endif
