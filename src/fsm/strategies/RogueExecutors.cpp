/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "RogueExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "SpellAuras.h"

// WotLK 3.3.5a Rogue Spell IDs
namespace RogueSpells
{
    // Stealth
    constexpr uint32 STEALTH_1 = 1784;
    constexpr uint32 STEALTH_2 = 1785;
    constexpr uint32 STEALTH_3 = 1786;
    constexpr uint32 STEALTH_4 = 1787;
    constexpr uint32 VANISH = 26889;
    constexpr uint32 SHADOW_DANCE = 51713;

    // Openers
    constexpr uint32 AMBUSH = 48691;
    constexpr uint32 CHEAP_SHOT = 1833;
    constexpr uint32 GARROTE = 48676;
    constexpr uint32 SAP = 51724;

    // Combo builders
    constexpr uint32 SINISTER_STRIKE = 48638;
    constexpr uint32 BACKSTAB = 48657;
    constexpr uint32 HEMORRHAGE = 48660;
    constexpr uint32 MUTILATE = 48666;
    constexpr uint32 GHOSTLY_STRIKE = 14278;
    constexpr uint32 FAN_OF_KNIVES = 51723;

    // Finishers
    constexpr uint32 EVISCERATE = 48668;
    constexpr uint32 ENVENOM = 57993;
    constexpr uint32 RUPTURE = 48672;
    constexpr uint32 KIDNEY_SHOT = 8643;
    constexpr uint32 SLICE_AND_DICE = 6774;
    constexpr uint32 EXPOSE_ARMOR = 8647;
    constexpr uint32 DEADLY_THROW = 48674;

    // Utility
    constexpr uint32 KICK = 1766;
    constexpr uint32 GOUGE = 1776;
    constexpr uint32 BLIND = 2094;
    constexpr uint32 SPRINT = 11305;
    constexpr uint32 EVASION = 26669;
    constexpr uint32 CLOAK_OF_SHADOWS = 31224;
    constexpr uint32 TRICKS_OF_THE_TRADE = 57934;
    constexpr uint32 FEINT = 48659;
    constexpr uint32 DISMANTLE = 51722;

    // Cooldowns
    constexpr uint32 ADRENALINE_RUSH = 13750;
    constexpr uint32 BLADE_FLURRY = 13877;
    constexpr uint32 KILLING_SPREE = 51690;
    constexpr uint32 COLD_BLOOD = 14177;
    constexpr uint32 PREPARATION = 14185;
    constexpr uint32 HUNGER_FOR_BLOOD = 51662;
}

// ============================================================================
// HELPER MACROS
// ============================================================================

// GET_CURRENT_TARGET is already defined in Playerbots.h
#define CAST_SPELL(spellId) botAI->CastSpell(spellId, GET_CURRENT_TARGET())
#define CAN_CAST(spellId) botAI->CanCastSpell(spellId, GET_CURRENT_TARGET(), true)

// ============================================================================
// STEALTH EXECUTORS
// ============================================================================

static bool ExecuteStealth(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;

    // Check if already stealthed
    if (state.classFlags & ClassState::InStealth)
        return false;

    // Find highest rank of stealth
    if (bot->HasSpell(RogueSpells::STEALTH_4) && CAN_CAST(RogueSpells::STEALTH_4))
        return CAST_SPELL(RogueSpells::STEALTH_4);
    if (bot->HasSpell(RogueSpells::STEALTH_3) && CAN_CAST(RogueSpells::STEALTH_3))
        return CAST_SPELL(RogueSpells::STEALTH_3);
    if (bot->HasSpell(RogueSpells::STEALTH_2) && CAN_CAST(RogueSpells::STEALTH_2))
        return CAST_SPELL(RogueSpells::STEALTH_2);
    if (bot->HasSpell(RogueSpells::STEALTH_1) && CAN_CAST(RogueSpells::STEALTH_1))
        return CAST_SPELL(RogueSpells::STEALTH_1);

    return false;
}

static bool ExecuteVanish(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::VANISH)) return false;
    if (!CAN_CAST(RogueSpells::VANISH)) return false;
    return CAST_SPELL(RogueSpells::VANISH);
}

static bool ExecuteShadowDance(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::SHADOW_DANCE)) return false;
    if (!CAN_CAST(RogueSpells::SHADOW_DANCE)) return false;
    return CAST_SPELL(RogueSpells::SHADOW_DANCE);
}

// ============================================================================
// OPENER EXECUTORS
// ============================================================================

static bool ExecuteAmbush(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::AMBUSH)) return false;
    if (!CAN_CAST(RogueSpells::AMBUSH)) return false;
    return CAST_SPELL(RogueSpells::AMBUSH);
}

static bool ExecuteCheapShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::CHEAP_SHOT)) return false;
    if (!CAN_CAST(RogueSpells::CHEAP_SHOT)) return false;
    return CAST_SPELL(RogueSpells::CHEAP_SHOT);
}

static bool ExecuteGarrote(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::GARROTE)) return false;
    if (!CAN_CAST(RogueSpells::GARROTE)) return false;
    return CAST_SPELL(RogueSpells::GARROTE);
}

static bool ExecuteSap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::SAP)) return false;
    if (!CAN_CAST(RogueSpells::SAP)) return false;
    return CAST_SPELL(RogueSpells::SAP);
}

// ============================================================================
// COMBO BUILDER EXECUTORS
// ============================================================================

static bool ExecuteSinisterStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::SINISTER_STRIKE)) return false;
    if (!CAN_CAST(RogueSpells::SINISTER_STRIKE)) return false;
    return CAST_SPELL(RogueSpells::SINISTER_STRIKE);
}

static bool ExecuteBackstab(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::BACKSTAB)) return false;
    if (!CAN_CAST(RogueSpells::BACKSTAB)) return false;
    return CAST_SPELL(RogueSpells::BACKSTAB);
}

static bool ExecuteHemorrhage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::HEMORRHAGE)) return false;
    if (!CAN_CAST(RogueSpells::HEMORRHAGE)) return false;
    return CAST_SPELL(RogueSpells::HEMORRHAGE);
}

static bool ExecuteMutilate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::MUTILATE)) return false;
    if (!CAN_CAST(RogueSpells::MUTILATE)) return false;
    return CAST_SPELL(RogueSpells::MUTILATE);
}

static bool ExecuteFanOfKnives(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::FAN_OF_KNIVES)) return false;
    if (!CAN_CAST(RogueSpells::FAN_OF_KNIVES)) return false;
    return CAST_SPELL(RogueSpells::FAN_OF_KNIVES);
}

// ============================================================================
// FINISHER EXECUTORS
// ============================================================================

static bool ExecuteEviscerate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::EVISCERATE)) return false;
    if (!CAN_CAST(RogueSpells::EVISCERATE)) return false;
    return CAST_SPELL(RogueSpells::EVISCERATE);
}

static bool ExecuteEnvenom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::ENVENOM)) return false;
    if (!CAN_CAST(RogueSpells::ENVENOM)) return false;
    return CAST_SPELL(RogueSpells::ENVENOM);
}

static bool ExecuteRupture(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::RUPTURE)) return false;
    if (!CAN_CAST(RogueSpells::RUPTURE)) return false;
    return CAST_SPELL(RogueSpells::RUPTURE);
}

static bool ExecuteKidneyShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::KIDNEY_SHOT)) return false;
    if (!CAN_CAST(RogueSpells::KIDNEY_SHOT)) return false;
    return CAST_SPELL(RogueSpells::KIDNEY_SHOT);
}

static bool ExecuteSliceAndDice(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::SLICE_AND_DICE)) return false;
    if (!CAN_CAST(RogueSpells::SLICE_AND_DICE)) return false;
    return botAI->CastSpell(RogueSpells::SLICE_AND_DICE, bot);  // Self-buff
}

static bool ExecuteExposeArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::EXPOSE_ARMOR)) return false;
    if (!CAN_CAST(RogueSpells::EXPOSE_ARMOR)) return false;
    return CAST_SPELL(RogueSpells::EXPOSE_ARMOR);
}

// ============================================================================
// UTILITY EXECUTORS
// ============================================================================

static bool ExecuteKick(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::KICK)) return false;
    if (!CAN_CAST(RogueSpells::KICK)) return false;
    return CAST_SPELL(RogueSpells::KICK);
}

static bool ExecuteGouge(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::GOUGE)) return false;
    if (!CAN_CAST(RogueSpells::GOUGE)) return false;
    return CAST_SPELL(RogueSpells::GOUGE);
}

static bool ExecuteBlind(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::BLIND)) return false;
    if (!CAN_CAST(RogueSpells::BLIND)) return false;
    return CAST_SPELL(RogueSpells::BLIND);
}

static bool ExecuteSprint(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::SPRINT)) return false;
    if (!CAN_CAST(RogueSpells::SPRINT)) return false;
    return botAI->CastSpell(RogueSpells::SPRINT, bot);  // Self-buff
}

static bool ExecuteEvasion(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::EVASION)) return false;
    if (!CAN_CAST(RogueSpells::EVASION)) return false;
    return botAI->CastSpell(RogueSpells::EVASION, bot);  // Self-buff
}

static bool ExecuteCloakOfShadows(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::CLOAK_OF_SHADOWS)) return false;
    if (!CAN_CAST(RogueSpells::CLOAK_OF_SHADOWS)) return false;
    return botAI->CastSpell(RogueSpells::CLOAK_OF_SHADOWS, bot);
}

static bool ExecuteTricksOfTheTrade(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::TRICKS_OF_THE_TRADE)) return false;

    // Find tank to cast on
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("main tank")->Get();
    if (!target)
        target = botAI->GetMaster();
    if (!target || target == bot)
        return false;

    if (!botAI->CanCastSpell(RogueSpells::TRICKS_OF_THE_TRADE, target, true))
        return false;

    return botAI->CastSpell(RogueSpells::TRICKS_OF_THE_TRADE, target);
}

static bool ExecuteFeint(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::FEINT)) return false;
    if (!CAN_CAST(RogueSpells::FEINT)) return false;
    return botAI->CastSpell(RogueSpells::FEINT, bot);
}

// ============================================================================
// COOLDOWN EXECUTORS
// ============================================================================

static bool ExecuteAdrenalineRush(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::ADRENALINE_RUSH)) return false;
    if (!CAN_CAST(RogueSpells::ADRENALINE_RUSH)) return false;
    return botAI->CastSpell(RogueSpells::ADRENALINE_RUSH, bot);
}

static bool ExecuteBladeFlurry(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::BLADE_FLURRY)) return false;
    if (!CAN_CAST(RogueSpells::BLADE_FLURRY)) return false;
    return botAI->CastSpell(RogueSpells::BLADE_FLURRY, bot);
}

static bool ExecuteKillingSpree(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::KILLING_SPREE)) return false;
    if (!CAN_CAST(RogueSpells::KILLING_SPREE)) return false;
    return botAI->CastSpell(RogueSpells::KILLING_SPREE, bot);
}

static bool ExecuteColdBlood(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::COLD_BLOOD)) return false;
    if (!CAN_CAST(RogueSpells::COLD_BLOOD)) return false;
    return botAI->CastSpell(RogueSpells::COLD_BLOOD, bot);
}

static bool ExecuteHungerForBlood(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::HUNGER_FOR_BLOOD)) return false;
    if (!CAN_CAST(RogueSpells::HUNGER_FOR_BLOOD)) return false;
    return botAI->CastSpell(RogueSpells::HUNGER_FOR_BLOOD, bot);
}

static bool ExecutePreparation(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(RogueSpells::PREPARATION)) return false;
    if (!CAN_CAST(RogueSpells::PREPARATION)) return false;
    return botAI->CastSpell(RogueSpells::PREPARATION, bot);
}

// ============================================================================
// BASIC ACTIONS
// ============================================================================

static bool ExecuteMelee(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    botAI->GetBot()->Attack(target, true);
    return true;
}

static bool ExecuteFollow(PlayerbotAI* botAI, const BotStateVector& state)
{
    // TODO: Implement using MovementAction infrastructure
    // For now, return false - existing engine will handle follow
    (void)botAI; (void)state;  // Suppress unused warnings
    return false;
}

static bool ExecuteReachMelee(PlayerbotAI* botAI, const BotStateVector& state)
{
    // TODO: Implement using MovementAction infrastructure
    // For now, return false - existing engine will handle movement
    (void)botAI; (void)state;  // Suppress unused warnings
    return false;
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterRogueActions(FSMEngine* engine)
{
    if (!engine) return;

    // Stealth
    engine->RegisterAction({RogueActions::Stealth, "stealth", ExecuteStealth, nullptr, nullptr, RogueSpells::STEALTH_4});
    engine->RegisterAction({RogueActions::Vanish, "vanish", ExecuteVanish, nullptr, nullptr, RogueSpells::VANISH});
    engine->RegisterAction({RogueActions::ShadowDance, "shadow dance", ExecuteShadowDance, nullptr, nullptr, RogueSpells::SHADOW_DANCE});

    // Openers
    engine->RegisterAction({RogueActions::Ambush, "ambush", ExecuteAmbush, nullptr, nullptr, RogueSpells::AMBUSH});
    engine->RegisterAction({RogueActions::CheapShot, "cheap shot", ExecuteCheapShot, nullptr, nullptr, RogueSpells::CHEAP_SHOT});
    engine->RegisterAction({RogueActions::Garrote, "garrote", ExecuteGarrote, nullptr, nullptr, RogueSpells::GARROTE});
    engine->RegisterAction({RogueActions::Sap, "sap", ExecuteSap, nullptr, nullptr, RogueSpells::SAP});

    // Combo builders
    engine->RegisterAction({RogueActions::SinisterStrike, "sinister strike", ExecuteSinisterStrike, nullptr, nullptr, RogueSpells::SINISTER_STRIKE});
    engine->RegisterAction({RogueActions::Backstab, "backstab", ExecuteBackstab, nullptr, nullptr, RogueSpells::BACKSTAB});
    engine->RegisterAction({RogueActions::Hemorrhage, "hemorrhage", ExecuteHemorrhage, nullptr, nullptr, RogueSpells::HEMORRHAGE});
    engine->RegisterAction({RogueActions::MutilateMainHand, "mutilate", ExecuteMutilate, nullptr, nullptr, RogueSpells::MUTILATE});
    engine->RegisterAction({RogueActions::FanOfKnives, "fan of knives", ExecuteFanOfKnives, nullptr, nullptr, RogueSpells::FAN_OF_KNIVES});

    // Finishers
    engine->RegisterAction({RogueActions::Eviscerate, "eviscerate", ExecuteEviscerate, nullptr, nullptr, RogueSpells::EVISCERATE});
    engine->RegisterAction({RogueActions::Envenom, "envenom", ExecuteEnvenom, nullptr, nullptr, RogueSpells::ENVENOM});
    engine->RegisterAction({RogueActions::Rupture, "rupture", ExecuteRupture, nullptr, nullptr, RogueSpells::RUPTURE});
    engine->RegisterAction({RogueActions::KidneyShot, "kidney shot", ExecuteKidneyShot, nullptr, nullptr, RogueSpells::KIDNEY_SHOT});
    engine->RegisterAction({RogueActions::SliceAndDice, "slice and dice", ExecuteSliceAndDice, nullptr, nullptr, RogueSpells::SLICE_AND_DICE});
    engine->RegisterAction({RogueActions::ExposeArmor, "expose armor", ExecuteExposeArmor, nullptr, nullptr, RogueSpells::EXPOSE_ARMOR});

    // Utility
    engine->RegisterAction({RogueActions::Kick, "kick", ExecuteKick, nullptr, nullptr, RogueSpells::KICK});
    engine->RegisterAction({RogueActions::Gouge, "gouge", ExecuteGouge, nullptr, nullptr, RogueSpells::GOUGE});
    engine->RegisterAction({RogueActions::Blind, "blind", ExecuteBlind, nullptr, nullptr, RogueSpells::BLIND});
    engine->RegisterAction({RogueActions::Sprint, "sprint", ExecuteSprint, nullptr, nullptr, RogueSpells::SPRINT});
    engine->RegisterAction({RogueActions::Evasion, "evasion", ExecuteEvasion, nullptr, nullptr, RogueSpells::EVASION});
    engine->RegisterAction({RogueActions::CloakOfShadows, "cloak of shadows", ExecuteCloakOfShadows, nullptr, nullptr, RogueSpells::CLOAK_OF_SHADOWS});
    engine->RegisterAction({RogueActions::TricksOfTheTrade, "tricks of the trade", ExecuteTricksOfTheTrade, nullptr, nullptr, RogueSpells::TRICKS_OF_THE_TRADE});
    engine->RegisterAction({RogueActions::Feint, "feint", ExecuteFeint, nullptr, nullptr, RogueSpells::FEINT});

    // Cooldowns
    engine->RegisterAction({RogueActions::AdrenalineRush, "adrenaline rush", ExecuteAdrenalineRush, nullptr, nullptr, RogueSpells::ADRENALINE_RUSH});
    engine->RegisterAction({RogueActions::BladeFlurry, "blade flurry", ExecuteBladeFlurry, nullptr, nullptr, RogueSpells::BLADE_FLURRY});
    engine->RegisterAction({RogueActions::KillingSpree, "killing spree", ExecuteKillingSpree, nullptr, nullptr, RogueSpells::KILLING_SPREE});
    engine->RegisterAction({RogueActions::ColdBlood, "cold blood", ExecuteColdBlood, nullptr, nullptr, RogueSpells::COLD_BLOOD});
    engine->RegisterAction({RogueActions::HungerForBlood, "hunger for blood", ExecuteHungerForBlood, nullptr, nullptr, RogueSpells::HUNGER_FOR_BLOOD});
    engine->RegisterAction({RogueActions::Preparation, "preparation", ExecutePreparation, nullptr, nullptr, RogueSpells::PREPARATION});

    // Basic actions
    engine->RegisterAction({RogueActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({RogueActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({RogueActions::ReachMelee, "reach melee", ExecuteReachMelee});
}
