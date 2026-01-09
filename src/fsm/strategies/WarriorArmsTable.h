/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARRIORARMSTABLE_H
#define _PLAYERBOT_WARRIORARMSTABLE_H

#include "../ActionTable.h"
#include "../FSMEngine.h"

namespace WarriorArmsStrategy
{
    // Strategy identifier
    constexpr uint16_t STRATEGY_ID = 102;  // 101 = Fury, 102 = Arms, 103 = Prot

    // Arms Warrior action rules - sorted by priority (descending)
    static const ActionDefinition rules[] = {
        // ========================================
        // EMERGENCY ACTIONS (Priority 90)
        // ========================================
        {
            .id = WarriorActions::EnragedRegeneration,
            .name = "enraged regeneration",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Critical)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,  // 3 min CD
        },
        {
            .id = WarriorActions::LastStand,
            .name = "last stand",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 180000,
        },

        // ========================================
        // INTERRUPT ACTIONS (Priority 40)
        // ========================================
        {
            .id = WarriorActions::Pummel,
            .name = "pummel",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 10000,
            .alternatives = {WarriorActions::ShieldBash, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION},
        },
        {
            .id = WarriorActions::ShieldBash,
            .name = "shield bash",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(0)  // Any stance with shield
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt - 1,
            .cooldownMs = 12000,
        },

        // ========================================
        // MOVEMENT ACTIONS (Priority 30)
        // ========================================
        {
            .id = WarriorActions::Charge,
            .name = "charge",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget | PrimaryState::TargetAttackable)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::InCombat)
                .requireClass(ClassState::BattleStance)
                .enemyRange(RangeBand::Far)  // 8-25 yards
                .build(),
            .basePriority = ActionPriority::Move,
            .cooldownMs = 15000,
            .alternatives = {WarriorActions::Intercept, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION},
        },
        {
            .id = WarriorActions::Intercept,
            .name = "intercept",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget | PrimaryState::TargetAttackable | PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .enemyRange(RangeBand::Far)  // 8-25 yards
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Move - 1,
            .cooldownMs = 30000,
        },

        // ========================================
        // STANCE MANAGEMENT (Priority 25)
        // ========================================
        {
            .id = WarriorActions::BattleStance,
            .name = "battle stance",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::BattleStance)  // Not already in Battle Stance
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 1000,
        },

        // ========================================
        // COOLDOWN ABILITIES (Priority 21-23)
        // ========================================
        {
            .id = WarriorActions::Bladestorm,
            .name = "bladestorm",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)  // AOE threshold
                .minRage(1)  // 25 rage
                .build(),
            .basePriority = ActionPriority::High + 3,
            .cooldownMs = 90000,  // 1.5 min CD
        },
        {
            .id = WarriorActions::Recklessness,
            .name = "recklessness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BerserkerStance)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 300000,  // 5 min CD
        },
        {
            .id = WarriorActions::SweepingStrikes,
            .name = "sweeping strikes",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(0)  // Battle or Berserker stance
                .minAttackers(2)
                .minRage(1)  // 30 rage
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 30000,  // 30 sec CD
        },

        // ========================================
        // BUFF MAINTENANCE (Priority 20)
        // ========================================
        {
            .id = WarriorActions::BattleShout,
            .name = "battle shout",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 120000,  // 2 min duration
        },

        // ========================================
        // PROC REACTIONS (Priority 18)
        // ========================================
        {
            .id = WarriorActions::Overpower,
            .name = "overpower",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::BattleStance | ClassState::Overpower)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High - 2,
            .cooldownMs = 5000,
        },
        {
            .id = WarriorActions::Execute,
            .name = "execute",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage base (with talent)
                .build(),
            .basePriority = ActionPriority::High - 3,
            .cooldownMs = 0,
        },

        // ========================================
        // AOE ATTACKS (Priority 15)
        // ========================================
        {
            .id = WarriorActions::ThunderClap,
            .name = "thunder clap",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(0)  // Battle or Defensive stance
                .minAttackers(2)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 20 rage
                .build(),
            .basePriority = ActionPriority::Normal + 5,
            .cooldownMs = 6000,
        },
        {
            .id = WarriorActions::Cleave,
            .name = "cleave",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .enemyRange(RangeBand::Melee)
                .minRage(2)  // 40 rage threshold for cleave spam
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 0,
        },

        // ========================================
        // CORE ROTATION (Priority 10-12)
        // ========================================
        {
            .id = WarriorActions::Rend,
            .name = "rend",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .anyOfPrimary(0)  // Battle or Defensive stance
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 15000,  // Rend duration
        },
        {
            .id = WarriorActions::MortalStrike,
            .name = "mortal strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 30 rage
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 6000,
        },
        {
            .id = WarriorActions::Slam,
            .name = "slam",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Moving)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 0,
        },

        // ========================================
        // RAGE GENERATORS (Priority 8-9)
        // ========================================
        {
            .id = WarriorActions::HeroicStrike,
            .name = "heroic strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(2)  // Dump excess rage
                .build(),
            .basePriority = ActionPriority::Normal - 1,
            .cooldownMs = 0,
        },

        // ========================================
        // UTILITY (Priority 6)
        // ========================================
        {
            .id = WarriorActions::VictoryRush,
            .name = "victory rush",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Default + 1,
            .cooldownMs = 0,
        },
        {
            .id = WarriorActions::Hamstring,
            .name = "hamstring",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Default,
            .cooldownMs = 15000,  // Apply once then wait
        },

        // ========================================
        // BASIC ATTACKS (Priority 1-2)
        // ========================================
        {
            .id = WarriorActions::ReachMelee,
            .name = "reach melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Close)  // Too far for melee
                .build(),
            .basePriority = ActionPriority::Idle + 2,
            .cooldownMs = 0,
        },
        {
            .id = WarriorActions::Melee,
            .name = "melee attack",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Idle + 1,
            .cooldownMs = 0,
        },
        {
            .id = WarriorActions::Shoot,
            .name = "shoot",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::InCombat)
                .enemyRange(RangeBand::Spell)  // Pulling from range
                .build(),
            .basePriority = ActionPriority::Idle,
            .cooldownMs = 0,
        },
    };

    // Count of rules
    constexpr uint16_t RULE_COUNT = sizeof(rules) / sizeof(ActionDefinition);

    // Action table instance
    static const ActionTable table = {
        rules,
        RULE_COUNT,
        STRATEGY_ID,
        "arms warrior"
    };

} // namespace WarriorArmsStrategy

#endif // _PLAYERBOT_WARRIORARMSTABLE_H
