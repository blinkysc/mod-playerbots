/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_DKBLOODTABLE_H
#define _PLAYERBOT_DKBLOODTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Blood Death Knight Action Table (Tank)
 *
 * Blood DKs are tanks focusing on self-healing and threat.
 * Key abilities: Heart Strike, Death Strike, Rune Strike, Dancing Rune Weapon
 */

namespace DKBloodStrategy
{
    constexpr uint16_t STRATEGY_ID = 500;

    static const ActionDefinition rules[] = {
        // Emergency
        {
            .id = DeathKnightActions::VampiricBlood,
            .name = "vampiric blood",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 60000,
            .alternatives = {DeathKnightActions::IceboundFortitude, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::IceboundFortitude,
            .name = "icebound fortitude",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 120000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Taunt
        {
            .id = DeathKnightActions::DarkCommand,
            .name = "dark command",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::LoseAggro)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Interrupt + 5,
            .cooldownMs = 8000,
            .alternatives = {DeathKnightActions::DeathGrip, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Interrupts
        {
            .id = DeathKnightActions::MindFreeze,
            .name = "mind freeze",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 10000,
            .alternatives = {DeathKnightActions::Strangulate, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs
        {
            .id = DeathKnightActions::BloodPresence,
            .name = "blood presence",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::BloodPresence)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 1000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::HornOfWinter,
            .name = "horn of winter",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 120000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Diseases
        {
            .id = DeathKnightActions::IcyTouch,
            .name = "icy touch",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::PlagueStrike,
            .name = "plague strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Main rotation
        {
            .id = DeathKnightActions::DeathStrike,
            .name = "death strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::MedHigh)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::HeartStrike,
            .name = "heart strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {DeathKnightActions::BloodStrike, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::RuneStrike,
            .name = "rune strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Default
        {
            .id = DeathKnightActions::Melee,
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
            .id = DeathKnightActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "blood dk"};
}

#endif
