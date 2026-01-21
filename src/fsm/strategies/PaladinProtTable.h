/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_PALADINPROTTABLE_H
#define _PLAYERBOT_PALADINPROTTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Protection Paladin Action Table
 *
 * Protection paladins are tanks focusing on threat and mitigation.
 * Key abilities: Hammer of the Righteous, Shield of Righteousness, Holy Shield
 * Priority: Taunt > Holy Shield > 969 rotation (HotR/ShoR/Consecration)
 */

namespace PaladinProtectionStrategy
{
    constexpr uint16_t STRATEGY_ID = 301;

    static const ActionDefinition rules[] = {
        // ====================================================================
        // EMERGENCY (Priority 90)
        // ====================================================================

        // Lay on Hands when critical
        {
            .id = PaladinActions::LayOnHands,
            .name = "lay on hands",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Critical)
                .build(),
            .basePriority = ActionPriority::Emergency,
            .cooldownMs = 1200000,
            .alternatives = {PaladinActions::DivineProtection, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Divine Protection (50% damage reduction)
        {
            .id = PaladinActions::DivineProtection,
            .name = "divine protection",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // TAUNT (Priority 45)
        // ====================================================================

        // Hand of Reckoning (taunt)
        {
            .id = PaladinActions::HandOfReckoning,
            .name = "hand of reckoning",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::LoseAggro)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Interrupt + 5,
            .cooldownMs = 8000,
            .alternatives = {PaladinActions::RighteousDefense, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Righteous Defense (mass taunt off ally)
        {
            .id = PaladinActions::RighteousDefense,
            .name = "righteous defense",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::InGroup)
                .requireSecondary(SecondaryState::ProtectNeeded)
                .forbidPrimary(PrimaryState::Dead)
                .build(),
            .basePriority = ActionPriority::Interrupt + 4,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // INTERRUPTS (Priority 40)
        // ====================================================================

        // Hammer of Justice to interrupt
        {
            .id = PaladinActions::HammerOfJustice,
            .name = "hammer of justice",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .build(),
            .basePriority = ActionPriority::Interrupt,
            .cooldownMs = 40000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Avenger's Shield as ranged pull/silence
        {
            .id = PaladinActions::AvengersShield,
            .name = "avenger's shield",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::HasTarget | PrimaryState::TargetCasting)
                .forbidPrimary(PrimaryState::Dead)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Interrupt - 1,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MOVEMENT (Priority 30)
        // ====================================================================

        // Reach melee range
        {
            .id = PaladinActions::ReachMelee,
            .name = "reach melee",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Close)
                .build(),
            .basePriority = ActionPriority::Move - 5,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // BUFFS (Priority 25-28)
        // ====================================================================

        // Righteous Fury (tank mode)
        {
            .id = PaladinActions::RighteousFury,
            .name = "righteous fury",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                // TODO: Check if RF not active
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Holy Shield (block + damage)
        {
            .id = PaladinActions::HolyShield,
            .name = "holy shield",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAttackers)
                .forbidPrimary(PrimaryState::Dead)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 7,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Seal of Vengeance for threat
        {
            .id = PaladinActions::SealOfVengeance,
            .name = "seal of vengeance",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasSeal)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::SealOfRighteousness, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Devotion Aura for armor
        {
            .id = PaladinActions::DevotionAura,
            .name = "devotion aura",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasAura)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Blessing of Sanctuary
        {
            .id = PaladinActions::BlessingOfSanctuary,
            .name = "blessing of sanctuary",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted | PrimaryState::InCombat)
                .forbidClass(ClassState::HasBlessing)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 600000,
            .alternatives = {PaladinActions::BlessingOfKings, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MAIN ROTATION - 969 (Priority 15-20)
        // ====================================================================

        // Hammer of the Righteous (6 sec CD)
        {
            .id = PaladinActions::HammerOfTheRighteous,
            .name = "hammer of the righteous",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Shield of Righteousness (6 sec CD)
        {
            .id = PaladinActions::ShieldOfRighteousness,
            .name = "shield of righteousness",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Consecration (9 sec filler)
        {
            .id = PaladinActions::Consecration,
            .name = "consecration",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(2)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Judgement of Light
        {
            .id = PaladinActions::JudgementOfLight,
            .name = "judgement of light",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::HasSeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High - 1,
            .cooldownMs = 10000,
            .alternatives = {PaladinActions::JudgementOfWisdom, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MANA MANAGEMENT (Priority 12)
        // ====================================================================

        // Divine Plea
        {
            .id = PaladinActions::DivinePlea,
            .name = "divine plea",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(3)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DEFAULT (Priority 5)
        // ====================================================================

        // Basic melee attack
        {
            .id = PaladinActions::Melee,
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

        // Follow when out of combat
        {
            .id = PaladinActions::Follow,
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

    static const ActionTable table = {
        rules,
        RULE_COUNT,
        STRATEGY_ID,
        "protection paladin"
    };

} // namespace PaladinProtectionStrategy

#endif // _PLAYERBOT_PALADINPROTTABLE_H
