/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARLOCKDESTROTABLE_H
#define _PLAYERBOT_WARLOCKDESTROTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Destruction Warlock Action Table (Direct Damage DPS)
 *
 * Destruction Warlocks focus on burst fire damage.
 * Key abilities: Chaos Bolt, Conflagrate, Immolate, Incinerate
 */

namespace WarlockDestroStrategy
{
    constexpr uint16_t STRATEGY_ID = 902;

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
        // Emergency - Shadowfury (stun)
        {
            .id = WarlockActions::Shadowfury,
            .name = "shadowfury",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 20000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
            .basePriority = ActionPriority::Emergency - 2,
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
        // Pet - Summon Imp (best for Destruction - Fire Bolt)
        {
            .id = WarlockActions::SummonImp,
            .name = "summon imp",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::InCombat | PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 10000,
            .alternatives = {WarlockActions::SummonSuccubus, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Chaos Bolt (signature ability - ignores resist)
        {
            .id = WarlockActions::ChaosBolt,
            .name = "chaos bolt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 12000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Conflagrate (instant, consumes Immolate)
        {
            .id = WarlockActions::Conflagrate,
            .name = "conflagrate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Immolate (keep up for Conflagrate)
        {
            .id = WarlockActions::Immolate,
            .name = "immolate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Curse of Elements (group utility) or Curse of Doom
        {
            .id = WarlockActions::CurseOfElements,
            .name = "curse of the elements",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 300000,
            .alternatives = {WarlockActions::CurseOfDoom, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shadowburn (execute)
        {
            .id = WarlockActions::Shadowburn,
            .name = "shadowburn",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Rain of Fire (AoE)
        {
            .id = WarlockActions::RainOfFire,
            .name = "rain of fire",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shadowflame (close range AoE)
        {
            .id = WarlockActions::Shadowflame,
            .name = "shadowflame",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Incinerate (main filler)
        {
            .id = WarlockActions::Incinerate,
            .name = "incinerate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {WarlockActions::ShadowBolt, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Shadow Bolt (backup filler)
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "destruction warlock"};
}

#endif
