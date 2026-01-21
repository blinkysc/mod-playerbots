/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_SHAMANELEMENTALTABLE_H
#define _PLAYERBOT_SHAMANELEMENTALTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Elemental Shaman Action Table (Caster DPS)
 *
 * Elemental Shamans focus on ranged spell damage.
 * Key abilities: Flame Shock, Lava Burst, Lightning Bolt, Thunderstorm
 */

namespace ShamanElementalStrategy
{
    constexpr uint16_t STRATEGY_ID = 1100;

    static const ActionDefinition rules[] = {
        // Emergency - Thunderstorm (knockback + mana)
        {
            .id = ShamanActions::Thunderstorm,
            .name = "thunderstorm",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 45000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Lightning Shield
        {
            .id = ShamanActions::LightningShield,
            .name = "lightning shield",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 600000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Totems - Totem of Wrath
        {
            .id = ShamanActions::TotemOfWrath,
            .name = "totem of wrath",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 300000,
            .alternatives = {ShamanActions::SearingTotem, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Totems - Wrath of Air
        {
            .id = ShamanActions::WrathOfAirTotem,
            .name = "wrath of air totem",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 300000,
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
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Bloodlust/Heroism
        {
            .id = ShamanActions::Bloodlust,
            .name = "bloodlust",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 600000,
            .alternatives = {ShamanActions::Heroism, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Flame Shock (apply DoT for Lava Burst)
        {
            .id = ShamanActions::FlameShock,
            .name = "flame shock",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 18000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Lava Burst (guaranteed crit with Flame Shock)
        {
            .id = ShamanActions::LavaBurst,
            .name = "lava burst",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Chain Lightning (AoE)
        {
            .id = ShamanActions::ChainLightning,
            .name = "chain lightning",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Fire Nova (AoE with fire totem)
        {
            .id = ShamanActions::FireNova,
            .name = "fire nova",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Wind Shear (interrupt)
        {
            .id = ShamanActions::WindShear,
            .name = "wind shear",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Lightning Bolt (filler)
        {
            .id = ShamanActions::LightningBolt,
            .name = "lightning bolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "elemental shaman"};
}

#endif
