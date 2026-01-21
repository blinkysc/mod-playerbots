/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_SHAMANRESTOTABLE_H
#define _PLAYERBOT_SHAMANRESTOTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Restoration Shaman Action Table (Healer)
 *
 * Restoration Shamans focus on chain healing and totems.
 * Key abilities: Chain Heal, Riptide, Earth Shield, Lesser Healing Wave
 */

namespace ShamanRestoStrategy
{
    constexpr uint16_t STRATEGY_ID = 1102;

    static const ActionDefinition rules[] = {
        // Emergency - Nature's Swiftness + Healing Wave
        {
            .id = ShamanActions::NaturesSwiftness,
            .name = "nature's swiftness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,
            .alternatives = {ShamanActions::Riptide, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Tidal Force
        {
            .id = ShamanActions::TidalForce,
            .name = "tidal force",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Water Shield
        {
            .id = ShamanActions::WaterShield,
            .name = "water shield",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 600000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Weapon - Earthliving
        {
            .id = ShamanActions::EarthlivingWeapon,
            .name = "earthliving weapon",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Earth Shield (on tank)
        {
            .id = ShamanActions::EarthShield,
            .name = "earth shield",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Totems - Mana Spring
        {
            .id = ShamanActions::ManaSpringTotem,
            .name = "mana spring totem",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 300000,
            .alternatives = {ShamanActions::HealingStreamTotem, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Totems - Wrath of Air (spell haste)
        {
            .id = ShamanActions::WrathOfAirTotem,
            .name = "wrath of air totem",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 300000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Bloodlust/Heroism
        {
            .id = ShamanActions::Bloodlust,
            .name = "bloodlust",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 600000,
            .alternatives = {ShamanActions::Heroism, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Riptide (instant HoT + boost Chain Heal)
        {
            .id = ShamanActions::Riptide,
            .name = "riptide",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Chain Heal (group heal)
        {
            .id = ShamanActions::ChainHeal,
            .name = "chain heal",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Lesser Healing Wave (fast heal)
        {
            .id = ShamanActions::LesserHealingWave,
            .name = "lesser healing wave",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Healing Wave (big heal)
        {
            .id = ShamanActions::HealingWave,
            .name = "healing wave",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Cleanse Spirit
        {
            .id = ShamanActions::CleanseSpiritAction,
            .name = "cleanse spirit",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // DPS filler - Lightning Bolt
        {
            .id = ShamanActions::LightningBolt,
            .name = "lightning bolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Follow
        {
            .id = ShamanActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "restoration shaman"};
}

#endif
