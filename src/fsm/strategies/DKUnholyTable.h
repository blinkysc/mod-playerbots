/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_DKUNHOLYTABLE_H
#define _PLAYERBOT_DKUNHOLYTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Unholy Death Knight Action Table (DPS)
 *
 * Unholy DKs focus on disease damage and pet synergy.
 * Key abilities: Scourge Strike, Death Coil, Summon Gargoyle
 */

namespace DKUnholyStrategy
{
    constexpr uint16_t STRATEGY_ID = 502;

    static const ActionDefinition rules[] = {
        // Emergency
        {
            .id = DeathKnightActions::IceboundFortitude,
            .name = "icebound fortitude",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 120000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Buffs
        {
            .id = DeathKnightActions::UnholyPresence,
            .name = "unholy presence",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::UnholyPresence)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 1000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::RaiseDead,
            .name = "raise dead",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::HasPet)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 180000,
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
        // Cooldowns
        {
            .id = DeathKnightActions::SummonGargoyle,
            .name = "summon gargoyle",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 180000,
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
            .id = DeathKnightActions::DeathAndDecay,
            .name = "death and decay",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::ScourgeStrike,
            .name = "scourge strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::DeathCoil,
            .name = "death coil",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        {
            .id = DeathKnightActions::BloodStrike,
            .name = "blood strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Normal + 3,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "unholy dk"};
}

#endif
