/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARLOCKDEMOTABLE_H
#define _PLAYERBOT_WARLOCKDEMOTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Demonology Warlock Action Table (Pet-focused DPS)
 *
 * Demonology Warlocks focus on demon empowerment and Metamorphosis.
 * Key abilities: Metamorphosis, Demonic Empowerment, Felguard
 */

namespace WarlockDemoStrategy
{
    constexpr uint16_t STRATEGY_ID = 901;

    static const ActionDefinition rules[] = {
        // Emergency - Death Coil
        {
            .id = WarlockActions::DeathCoil,
            .name = "death coil",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 120000,
            .alternatives = {WarlockActions::Fear, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Howl of Terror
        {
            .id = WarlockActions::HowlOfTerror,
            .name = "howl of terror",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 40000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs - Fel Armor
        {
            .id = WarlockActions::FelArmor,
            .name = "fel armor",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 1800000,
            .alternatives = {WarlockActions::DemonArmor, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Pet - Summon Felguard (Demonology exclusive)
        {
            .id = WarlockActions::SummonFelguard,
            .name = "summon felguard",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 10000,
            .alternatives = {WarlockActions::SummonImp, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Metamorphosis (major cooldown)
        {
            .id = WarlockActions::Metamorphosis,
            .name = "metamorphosis",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Demonic Empowerment (pet buff)
        {
            .id = WarlockActions::DemonicEmpowerment,
            .name = "demonic empowerment",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana - Life Tap
        {
            .id = WarlockActions::LifeTap,
            .name = "life tap",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::MedLow)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Immolation Aura (Metamorphosis ability)
        {
            .id = WarlockActions::ImmolationAura,
            .name = "immolation aura",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Immolate
        {
            .id = WarlockActions::Immolate,
            .name = "immolate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Corruption
        {
            .id = WarlockActions::Corruption,
            .name = "corruption",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 18000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Curse of Doom (long fights) / Curse of Agony
        {
            .id = WarlockActions::CurseOfAgony,
            .name = "curse of agony",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 24000,
            .alternatives = {WarlockActions::CurseOfDoom, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Soul Fire (Decimation proc filler)
        {
            .id = WarlockActions::SoulFire,
            .name = "soul fire",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {WarlockActions::ShadowBolt, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shadow Bolt (filler)
        {
            .id = WarlockActions::ShadowBolt,
            .name = "shadow bolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Health Funnel (heal pet)
        {
            .id = WarlockActions::HealthFunnel,
            .name = "health funnel",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Incinerate (alternative filler)
        {
            .id = WarlockActions::Incinerate,
            .name = "incinerate",
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
            .id = WarlockActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "demonology warlock"};
}

#endif
