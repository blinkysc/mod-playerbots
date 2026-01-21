/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_PRIESTHOLYTABLE_H
#define _PLAYERBOT_PRIESTHOLYTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Holy Priest Action Table (Healer)
 *
 * Holy Priests focus on direct heals and AoE healing.
 * Key abilities: Circle of Healing, Prayer of Healing, Guardian Spirit
 */

namespace PriestHolyStrategy
{
    constexpr uint16_t STRATEGY_ID = 802;

    static const ActionDefinition rules[] = {
        // Emergency - Guardian Spirit
        {
            .id = PriestActions::GuardianSpirit,
            .name = "guardian spirit",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,
            .alternatives = {PriestActions::FlashHeal, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Divine Hymn
        {
            .id = PriestActions::DivineHymn,
            .name = "divine hymn",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 480000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Psychic Scream
        {
            .id = PriestActions::PsychicScream,
            .name = "psychic scream",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::Emergency - 2,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Inner Fire
        {
            .id = PriestActions::InnerFire,
            .name = "inner fire",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Power Word: Fortitude
        {
            .id = PriestActions::PowerWordFortitude,
            .name = "power word: fortitude",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Divine Spirit
        {
            .id = PriestActions::DivineSpirit,
            .name = "divine spirit",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 1800000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana - Shadowfiend
        {
            .id = PriestActions::Shadowfiend,
            .name = "shadowfiend",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 300000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Power Word: Shield (prevent damage)
        {
            .id = PriestActions::PowerWordShield,
            .name = "power word: shield",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 4000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Fade - drop aggro
        {
            .id = PriestActions::Fade,
            .name = "fade",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAggro)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Circle of Healing (group heal)
        {
            .id = PriestActions::CircleOfHealing,
            .name = "circle of healing",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 6000,
            .alternatives = {PriestActions::PrayerOfHealing, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Prayer of Mending (proactive)
        {
            .id = PriestActions::PrayerOfMending,
            .name = "prayer of mending",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Prayer of Healing (AoE heal)
        {
            .id = PriestActions::PrayerOfHealing,
            .name = "prayer of healing",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Flash Heal (fast heal)
        {
            .id = PriestActions::FlashHeal,
            .name = "flash heal",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Greater Heal
        {
            .id = PriestActions::GreaterHeal,
            .name = "greater heal",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Binding Heal (heal self and target)
        {
            .id = PriestActions::BindingHeal,
            .name = "binding heal",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Renew (HoT)
        {
            .id = PriestActions::Renew,
            .name = "renew",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Holy Nova (group heal + damage)
        {
            .id = PriestActions::HolyNova,
            .name = "holy nova",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Smite (DPS filler)
        {
            .id = PriestActions::Smite,
            .name = "smite",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Default heal
        {
            .id = PriestActions::Heal,
            .name = "heal",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Follow
        {
            .id = PriestActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "holy priest"};
}

#endif
