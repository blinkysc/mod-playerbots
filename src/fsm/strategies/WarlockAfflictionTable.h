/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARLOCKAFFLICTIONTABLE_H
#define _PLAYERBOT_WARLOCKAFFLICTIONTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Affliction Warlock Action Table (DoT DPS)
 *
 * Affliction Warlocks focus on damage-over-time effects.
 * Key abilities: Haunt, Unstable Affliction, Corruption, Curse of Agony
 */

namespace WarlockAfflictionStrategy
{
    constexpr uint16_t STRATEGY_ID = 900;

    static const ActionDefinition rules[] = {
        // Emergency - Death Coil (heal + horror)
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
        // Pet - Summon Felhunter (best for Affliction)
        {
            .id = WarlockActions::SummonFelhunter,
            .name = "summon felhunter",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 10000,
            .alternatives = {WarlockActions::SummonImp, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Mana - Dark Pact (drain pet mana)
        {
            .id = WarlockActions::DarkPact,
            .name = "dark pact",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(ManaLevel::Low)
                .minHealth(HealthLevel::MedHigh)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 0,
            .alternatives = {WarlockActions::LifeTap, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Haunt (priority DoT - returns to heal)
        {
            .id = WarlockActions::Haunt,
            .name = "haunt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Unstable Affliction (high priority DoT)
        {
            .id = WarlockActions::UnstableAffliction,
            .name = "unstable affliction",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
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
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 18000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Curse of Agony
        {
            .id = WarlockActions::CurseOfAgony,
            .name = "curse of agony",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 24000,
            .alternatives = {WarlockActions::CurseOfElements, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Drain Soul (execute phase + soul shard)
        {
            .id = WarlockActions::DrainSoul,
            .name = "drain soul",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {WarlockActions::ShadowBolt, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Seed of Corruption (AoE)
        {
            .id = WarlockActions::SeedOfCorruption,
            .name = "seed of corruption",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Drain Life (self-heal filler)
        {
            .id = WarlockActions::DrainLife,
            .name = "drain life",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shadow Bolt (filler)
        {
            .id = WarlockActions::ShadowBolt,
            .name = "shadow bolt",
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "affliction warlock"};
}

#endif
