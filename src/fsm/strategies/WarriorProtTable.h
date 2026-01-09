/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARRIORPROTTABLE_H
#define _PLAYERBOT_WARRIORPROTTABLE_H

#include "../ActionTable.h"
#include "../FSMEngine.h"

namespace WarriorProtStrategy
{
    // Strategy identifier
    constexpr uint16_t STRATEGY_ID = 103;  // 101 = Fury, 102 = Arms, 103 = Prot

    // Protection Warrior action rules - sorted by priority (descending)
    static const ActionDefinition rules[] = {
        // ========================================
        // EMERGENCY ACTIONS (Priority 90)
        // ========================================
        {
            .id = WarriorActions::LastStand,
            .name = "last stand",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Critical)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 180000,  // 3 min CD
        },
        {
            .id = WarriorActions::ShieldWall,
            .name = "shield wall",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 300000,  // 5 min CD
        },
        {
            .id = WarriorActions::EnragedRegeneration,
            .name = "enraged regeneration",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 2,
            .cooldownMs = 180000,
        },

        // ========================================
        // INTERRUPT ACTIONS (Priority 40)
        // ========================================
        {
            .id = WarriorActions::ShieldBash,
            .name = "shield bash",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 12000,
        },
        {
            .id = WarriorActions::ConcussionBlow,
            .name = "concussion blow",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Interrupt - 1,
            .cooldownMs = 30000,
        },

        // ========================================
        // TAUNT/THREAT ACTIONS (Priority 35)
        // ========================================
        {
            .id = WarriorActions::Taunt,
            .name = "taunt",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::HasAggro)
                .requireClass(ClassState::DefensiveStance)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Move + 5,
            .cooldownMs = 8000,
        },
        {
            .id = WarriorActions::ChallengingShout,
            .name = "challenging shout",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(3)  // AOE taunt for groups
                .build(),
            .basePriority = ActionPriority::Move + 4,
            .cooldownMs = 180000,  // 3 min CD
        },
        {
            .id = WarriorActions::MockingBlow,
            .name = "mocking blow",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead | PrimaryState::HasAggro)
                .requireClass(ClassState::BattleStance)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Move + 3,
            .cooldownMs = 60000,
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
            .alternatives = {WarriorActions::Intervene, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION},
        },
        {
            .id = WarriorActions::Intervene,
            .name = "intervene",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Move - 1,
            .cooldownMs = 30000,
        },

        // ========================================
        // STANCE MANAGEMENT (Priority 25)
        // ========================================
        {
            .id = WarriorActions::DefensiveStance,
            .name = "defensive stance",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::DefensiveStance)  // Not already in Defensive
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 1000,
        },

        // ========================================
        // PROC REACTIONS (Priority 22)
        // ========================================
        {
            .id = WarriorActions::Revenge,
            .name = "revenge",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance | ClassState::Revenge)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 5000,
        },
        {
            .id = WarriorActions::ShieldSlam,
            .name = "shield slam",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 20 rage
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 6000,
        },

        // ========================================
        // BUFF MAINTENANCE (Priority 20)
        // ========================================
        {
            .id = WarriorActions::ShieldBlock,
            .name = "shield block",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 5000,  // ~10 sec duration
        },
        {
            .id = WarriorActions::CommandingShout,
            .name = "commanding shout",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .build(),
            .basePriority = ActionPriority::High - 1,
            .cooldownMs = 120000,  // 2 min duration
        },

        // ========================================
        // AOE THREAT (Priority 15-17)
        // ========================================
        {
            .id = WarriorActions::Shockwave,
            .name = "shockwave",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minAttackers(2)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Normal + 7,
            .cooldownMs = 20000,
        },
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
            .basePriority = ActionPriority::Normal + 6,
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
                .minRage(2)  // Higher rage threshold
                .build(),
            .basePriority = ActionPriority::Normal + 5,
            .cooldownMs = 0,
        },

        // ========================================
        // DEBUFFS (Priority 13-14)
        // ========================================
        {
            .id = WarriorActions::DemoralizingShout,
            .name = "demoralizing shout",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 10 rage
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 30000,  // Refresh every 30s
        },
        {
            .id = WarriorActions::SunderArmor,
            .name = "sunder armor",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 3000,  // Stack to 5
        },

        // ========================================
        // CORE ROTATION (Priority 10-12)
        // ========================================
        {
            .id = WarriorActions::Devastate,
            .name = "devastate",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 0,
        },
        {
            .id = WarriorActions::HeroicStrike,
            .name = "heroic strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(2)  // Dump excess rage
                .build(),
            .basePriority = ActionPriority::Normal + 1,
            .cooldownMs = 0,
        },

        // ========================================
        // UTILITY (Priority 6-8)
        // ========================================
        {
            .id = WarriorActions::Disarm,
            .name = "disarm",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::DefensiveStance)
                .enemyRange(RangeBand::Melee)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Default + 3,
            .cooldownMs = 60000,
        },
        {
            .id = WarriorActions::SpellReflection,
            .name = "spell reflection",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .minRage(1)  // 15 rage
                .build(),
            .basePriority = ActionPriority::Default + 2,
            .cooldownMs = 10000,
        },
        {
            .id = WarriorActions::Hamstring,
            .name = "hamstring",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minRage(1)
                .build(),
            .basePriority = ActionPriority::Default + 1,
            .cooldownMs = 15000,
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
                .enemyRange(RangeBand::Close)
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
                .enemyRange(RangeBand::Spell)
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
        "protection warrior"
    };

} // namespace WarriorProtStrategy

#endif // _PLAYERBOT_WARRIORPROTTABLE_H
