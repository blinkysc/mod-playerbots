/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_PALADINHOLYTABLE_H
#define _PLAYERBOT_PALADINHOLYTABLE_H

#include "../BotStateMask.h"
#include "../ActionTable.h"
#include "../FSMEngine.h"

/**
 * Holy Paladin Action Table
 *
 * Holy paladins are healers focusing on single-target healing.
 * Key abilities: Holy Light, Flash of Light, Holy Shock, Beacon of Light
 * Priority: Emergency heals > Beacon maintenance > Holy Shock > Holy Light/Flash of Light
 */

namespace PaladinHolyStrategy
{
    constexpr uint16_t STRATEGY_ID = 300;

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
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Divine Shield if healer in danger
        {
            .id = PaladinActions::DivineShield,
            .name = "divine shield",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasAggro)
                .forbidPrimary(PrimaryState::Dead)
                .maxHealth(HealthLevel::Low)
                .build(),
            .basePriority = ActionPriority::Emergency - 1,
            .cooldownMs = 300000,
            .alternatives = {PaladinActions::DivineProtection, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Holy Shock emergency (instant)
        {
            .id = PaladinActions::HolyShock,
            .name = "holy shock",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .requireSecondary(SecondaryState::PartyNeedsHeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Emergency - 2,
            .cooldownMs = 6000,
            .alternatives = {PaladinActions::FlashOfLight, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DISPELS (Priority 50)
        // ====================================================================

        // Cleanse debuffs
        {
            .id = PaladinActions::Cleanse,
            .name = "cleanse",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .requireSecondary(SecondaryState::NeedsPartyCure)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Dispel,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // BUFFS (Priority 25-30)
        // ====================================================================

        // Beacon of Light on tank
        {
            .id = PaladinActions::BeaconOfLight,
            .name = "beacon of light",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InGroup)
                .forbidPrimary(PrimaryState::Dead)
                // TODO: Check if beacon not on tank
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 10,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Sacred Shield
        {
            .id = PaladinActions::SacredShield,
            .name = "sacred shield",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 9,
            .cooldownMs = 30000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Seal of Wisdom for mana
        {
            .id = PaladinActions::SealOfWisdom,
            .name = "seal of wisdom",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasSeal)
                .build(),
            .basePriority = ActionPriority::High + 6,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::SealOfLight, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Concentration Aura for healing
        {
            .id = PaladinActions::ConcentrationAura,
            .name = "concentration aura",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted)
                .forbidClass(ClassState::HasAura)
                .build(),
            .basePriority = ActionPriority::High + 5,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::DevotionAura, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Blessing of Wisdom
        {
            .id = PaladinActions::BlessingOfWisdom,
            .name = "blessing of wisdom",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead | PrimaryState::Mounted | PrimaryState::InCombat)
                .forbidClass(ClassState::HasBlessing)
                .build(),
            .basePriority = ActionPriority::High + 4,
            .cooldownMs = 600000,
            .alternatives = {PaladinActions::BlessingOfKings, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // HEALING (Priority 15-22)
        // ====================================================================

        // Holy Light for heavy healing
        {
            .id = PaladinActions::HolyLight,
            .name = "holy light",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .requireSecondary(SecondaryState::PartyNeedsHeal)
                .minMana(2)
                .build(),
            .basePriority = ActionPriority::High + 2,
            .cooldownMs = 0,
            .alternatives = {PaladinActions::FlashOfLight, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Flash of Light for light healing
        {
            .id = PaladinActions::FlashOfLight,
            .name = "flash of light",
            .condition = ConditionBuilder()
                .forbidPrimary(PrimaryState::Dead)
                .requireSecondary(SecondaryState::PartyNeedsHeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::High + 1,
            .cooldownMs = 0,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // Judgement of Wisdom for mana
        {
            .id = PaladinActions::JudgementOfWisdom,
            .name = "judgement of wisdom",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .requireClass(ClassState::HasSeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Normal + 3,
            .cooldownMs = 10000,
            .alternatives = {PaladinActions::JudgementOfLight, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
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
                .maxMana(2)
                .build(),
            .basePriority = ActionPriority::Normal + 2,
            .cooldownMs = 60000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // OFFENSIVE (Priority 8-10) - When not healing
        // ====================================================================

        // Exorcism when no healing needed
        {
            .id = PaladinActions::Exorcism,
            .name = "exorcism",
            .condition = ConditionBuilder()
                .requirePrimary(PrimaryState::InCombat | PrimaryState::HasTarget)
                .forbidPrimary(PrimaryState::Dead)
                .forbidSecondary(SecondaryState::PartyNeedsHeal)
                .minMana(1)
                .build(),
            .basePriority = ActionPriority::Normal,
            .cooldownMs = 15000,
            .alternatives = {INVALID_ACTION, INVALID_ACTION, INVALID_ACTION, INVALID_ACTION}
        },

        // ====================================================================
        // DEFAULT (Priority 5)
        // ====================================================================

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
        "holy paladin"
    };

} // namespace PaladinHolyStrategy

#endif // _PLAYERBOT_PALADINHOLYTABLE_H
