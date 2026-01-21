/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_DRUIDRESTOTABLE_H
#define _PLAYERBOT_DRUIDRESTOTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Restoration Druid Action Table (Healer)
 *
 * Restoration Druids focus on HoT-based healing.
 * Key abilities: Rejuvenation, Regrowth, Lifebloom, Wild Growth, Swiftmend
 */

namespace DruidRestoStrategy
{
    constexpr uint16_t STRATEGY_ID = 1002;

    static const ActionDefinition rules[] = {
        // Emergency - Nature's Swiftness + Healing Touch
        {
            .id = DruidActions::NaturesSwiftness,
            .name = "nature's swiftness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,
            .alternatives = {DruidActions::Swiftmend, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Barkskin
        {
            .id = DruidActions::Barkskin,
            .name = "barkskin",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Tranquility
        {
            .id = DruidActions::Tranquility,
            .name = "tranquility",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Emergency - 2,
            .cooldownMs = 480000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Mark of the Wild
        {
            .id = DruidActions::MarkOfTheWild,
            .name = "mark of the wild",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Thorns
        {
            .id = DruidActions::Thorns,
            .name = "thorns",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 600000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shapeshift - Tree of Life (optional, can heal in caster form too)
        {
            .id = DruidActions::TreeOfLifeForm,
            .name = "tree of life",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana - Innervate
        {
            .id = DruidActions::Innervate,
            .name = "innervate",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Wild Growth (group HoT)
        {
            .id = DruidActions::WildGrowth,
            .name = "wild growth",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Swiftmend (instant, consumes HoT)
        {
            .id = DruidActions::Swiftmend,
            .name = "swiftmend",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 15000,
            .alternatives = {DruidActions::Regrowth, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Lifebloom (stack on tank)
        {
            .id = DruidActions::Lifebloom,
            .name = "lifebloom",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 7000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Rejuvenation (HoT)
        {
            .id = DruidActions::Rejuvenation,
            .name = "rejuvenation",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Regrowth (direct heal + HoT)
        {
            .id = DruidActions::Regrowth,
            .name = "regrowth",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Nourish (direct heal, boosted by HoTs)
        {
            .id = DruidActions::Nourish,
            .name = "nourish",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Healing Touch (big heal)
        {
            .id = DruidActions::HealingTouch,
            .name = "healing touch",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // DPS filler - Wrath
        {
            .id = DruidActions::Wrath,
            .name = "wrath",
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
            .id = DruidActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "restoration druid"};
}

#endif
