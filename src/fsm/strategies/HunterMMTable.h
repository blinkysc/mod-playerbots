/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_HUNTERMMTABLE_H
#define _PLAYERBOT_HUNTERMMTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Marksmanship Hunter Action Table (DPS)
 *
 * MM Hunters focus on raw ranged damage and steady shot weaving.
 * Key abilities: Chimera Shot, Aimed Shot, Steady Shot, Silencing Shot
 */

namespace HunterMMStrategy
{
    constexpr uint16_t STRATEGY_ID = 701;

    static const ActionDefinition rules[] = {
        // Emergency - Feign Death
        {
            .id = HunterActions::FeignDeath,
            .name = "feign death",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 30000,
            .alternatives = {HunterActions::Deterrence, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Emergency - Deterrence
        {
            .id = HunterActions::Deterrence,
            .name = "deterrence",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 90000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Interrupt - Silencing Shot (MM signature)
        {
            .id = HunterActions::SilencingShot,
            .name = "silencing shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 20000,
            .alternatives = {HunterActions::ScatterShot, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Pet management - Call Pet
        {
            .id = HunterActions::CallPet,
            .name = "call pet",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::HasPet)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 5000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Pet management - Revive Pet
        {
            .id = HunterActions::RevivePet,
            .name = "revive pet",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::InCombat | PrimaryState::HasPet)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Pet management - Mend Pet
        {
            .id = HunterActions::MendPet,
            .name = "mend pet",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasPet)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 3000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Aspect - Dragonhawk
        {
            .id = HunterActions::AspectOfTheDragonhawk,
            .name = "aspect of the dragonhawk",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .minMana(ManaLevel::MedLow)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 1800000,
            .alternatives = {HunterActions::AspectOfTheHawk, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Aspect - Viper (low mana)
        {
            .id = HunterActions::AspectOfTheViper,
            .name = "aspect of the viper",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .maxMana(ManaLevel::Low)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Hunter's Mark
        {
            .id = HunterActions::HuntersMark,
            .name = "hunter's mark",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 120000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // MM Cooldown - Rapid Fire
        {
            .id = HunterActions::RapidFire,
            .name = "rapid fire",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 300000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // MM Cooldown - Readiness
        {
            .id = HunterActions::Readiness,
            .name = "readiness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Kill Shot (execute - health check done in executor)
        {
            .id = HunterActions::KillShot,
            .name = "kill shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Chimera Shot (MM signature)
        {
            .id = HunterActions::ChimeraShot,
            .name = "chimera shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Aimed Shot
        {
            .id = HunterActions::AimedShot,
            .name = "aimed shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Serpent Sting (maintain for Chimera Shot)
        {
            .id = HunterActions::SerpentSting,
            .name = "serpent sting",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Multi-Shot (AoE)
        {
            .id = HunterActions::MultiShot,
            .name = "multi-shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Arcane Shot
        {
            .id = HunterActions::ArcaneShot,
            .name = "arcane shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Steady Shot (filler - core MM ability)
        {
            .id = HunterActions::SteadyShot,
            .name = "steady shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Auto Shot
        {
            .id = HunterActions::AutoShot,
            .name = "auto shot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Raptor Strike (melee emergency)
        {
            .id = HunterActions::RaptorStrike,
            .name = "raptor strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Default + 1,
            .cooldownMs = 6000,
            .alternatives = {HunterActions::MongooseBite, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Wing Clip (escape melee)
        {
            .id = HunterActions::WingClip,
            .name = "wing clip",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Default,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },
        // Follow
        {
            .id = HunterActions::Follow,
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
    static const ActionTable table = {rules, RULE_COUNT, STRATEGY_ID, "marksmanship hunter"};
}

#endif
