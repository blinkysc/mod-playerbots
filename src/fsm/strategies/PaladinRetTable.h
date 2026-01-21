/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_PALADINRETTABLE_H
#define _PLAYERBOT_PALADINRETTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Retribution Paladin Action Table
 *
 * Retribution paladins are melee DPS focusing on burst damage.
 * Key abilities: Crusader Strike, Divine Storm, Hammer of Wrath, Judgement
 * Priority: Hammer of Wrath (execute) > Divine Storm > Crusader Strike > Judgement
 */

namespace PaladinRetributionStrategy
{
    constexpr uint16_t STRATEGY_ID = 302;

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
            .alternatives = {PaladinActions::DivineShield, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Divine Shield emergency
        {
            .id = PaladinActions::DivineShield,
            .name = "divine shield",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::VeryLow)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 300000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Flash of Light self-heal when low
        {
            .id = PaladinActions::FlashOfLight,
            .name = "flash of light",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Emergency - 3,
            .cooldownMs = 0,
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

        // Avenging Wrath burst cooldown
        {
            .id = PaladinActions::AvengingWrath,
            .name = "avenging wrath",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minHealth(HealthLevel::Medium)
                .build(),
            .basePriority = ActionPriority::High + 8,
            .cooldownMs = 180000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Seal of Command/Vengeance
        {
            .id = PaladinActions::SealOfVengeance,
            .name = "seal of vengeance",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasSeal)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::SealOfCommand, PaladinActions::SealOfRighteousness, INVALID_ACTION, INVALID_ACTION}
        },

        // Retribution Aura
        {
            .id = PaladinActions::RetributionAura,
            .name = "retribution aura",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasAura)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::DevotionAura, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Blessing of Might
        {
            .id = PaladinActions::BlessingOfMight,
            .name = "blessing of might",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted | PrimaryState::InCombat)
                .forbidClass(ClassState::HasBlessing)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 600000,
            .alternatives = {PaladinActions::BlessingOfKings, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // EXECUTE PHASE (Priority 22-24)
        // ====================================================================

        // Hammer of Wrath (execute ability)
        {
            .id = PaladinActions::HammerOfWrath,
            .name = "hammer of wrath",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                // TODO: Check target health < 20%
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 6000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MAIN ROTATION (Priority 15-20)
        // ====================================================================

        // Divine Storm (AOE + damage)
        {
            .id = PaladinActions::DivineStorm,
            .name = "divine storm",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 10000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Crusader Strike
        {
            .id = PaladinActions::CrusaderStrike,
            .name = "crusader strike",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High,
            .cooldownMs = 4000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Judgement of Wisdom (for mana return)
        {
            .id = PaladinActions::JudgementOfWisdom,
            .name = "judgement of wisdom",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::HasSeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High - 1,
            .cooldownMs = 10000,
            .alternatives = {PaladinActions::JudgementOfLight, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Consecration
        {
            .id = PaladinActions::Consecration,
            .name = "consecration",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .enemyRange(RangeBand::Melee)
                .minMana(2)
                .minAttackers(2)
                .build(),
            .basePriority = ActionPriority::High - 2,
            .cooldownMs = 8000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Exorcism (Art of War proc or vs undead/demons)
        {
            .id = PaladinActions::Exorcism,
            .name = "exorcism",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Normal + 4,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // MANA MANAGEMENT (Priority 12)
        // ====================================================================

        // Divine Plea when low mana
        {
            .id = PaladinActions::DivinePlea,
            .name = "divine plea",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .maxMana(2)  // Use when mana is low
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
        "retribution paladin"
    };

} // namespace PaladinRetributionStrategy

#endif // _PLAYERBOT_PALADINRETTABLE_H
