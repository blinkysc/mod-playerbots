/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "PaladinExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "SpellAuras.h"

// WotLK 3.3.5a Paladin Spell IDs
namespace PaladinSpells
{
    // Auras
    constexpr uint32 DEVOTION_AURA = 48942;
    constexpr uint32 RETRIBUTION_AURA = 54043;
    constexpr uint32 CONCENTRATION_AURA = 19746;
    constexpr uint32 SHADOW_RESISTANCE_AURA = 48943;
    constexpr uint32 FROST_RESISTANCE_AURA = 48945;
    constexpr uint32 FIRE_RESISTANCE_AURA = 48947;
    constexpr uint32 CRUSADER_AURA = 32223;

    // Seals
    constexpr uint32 SEAL_OF_RIGHTEOUSNESS = 21084;
    constexpr uint32 SEAL_OF_COMMAND = 20375;
    constexpr uint32 SEAL_OF_VENGEANCE = 31801;
    constexpr uint32 SEAL_OF_CORRUPTION = 53736;  // Horde version
    constexpr uint32 SEAL_OF_LIGHT = 20165;
    constexpr uint32 SEAL_OF_WISDOM = 20166;
    constexpr uint32 SEAL_OF_JUSTICE = 20164;

    // Judgments
    constexpr uint32 JUDGEMENT_OF_LIGHT = 20271;
    constexpr uint32 JUDGEMENT_OF_WISDOM = 53408;
    constexpr uint32 JUDGEMENT_OF_JUSTICE = 53407;

    // Blessings
    constexpr uint32 BLESSING_OF_MIGHT = 48932;
    constexpr uint32 BLESSING_OF_KINGS = 20217;
    constexpr uint32 BLESSING_OF_WISDOM = 48936;
    constexpr uint32 BLESSING_OF_SANCTUARY = 20911;

    // Holy spells
    constexpr uint32 HOLY_LIGHT = 48782;
    constexpr uint32 FLASH_OF_LIGHT = 48785;
    constexpr uint32 HOLY_SHOCK = 48825;
    constexpr uint32 LAY_ON_HANDS = 48788;
    constexpr uint32 CONSECRATION = 48819;
    constexpr uint32 EXORCISM = 48801;
    constexpr uint32 HOLY_WRATH = 48817;
    constexpr uint32 BEACON_OF_LIGHT = 53563;
    constexpr uint32 SACRED_SHIELD = 53601;

    // Protection abilities
    constexpr uint32 RIGHTEOUS_FURY = 25780;
    constexpr uint32 HAMMER_OF_THE_RIGHTEOUS = 53595;
    constexpr uint32 SHIELD_OF_RIGHTEOUSNESS = 61411;
    constexpr uint32 AVENGERS_SHIELD = 48827;
    constexpr uint32 HOLY_SHIELD = 48952;
    constexpr uint32 RIGHTEOUS_DEFENSE = 31789;
    constexpr uint32 HAND_OF_RECKONING = 62124;

    // Retribution abilities
    constexpr uint32 CRUSADER_STRIKE = 35395;
    constexpr uint32 DIVINE_STORM = 53385;
    constexpr uint32 HAMMER_OF_WRATH = 48806;
    constexpr uint32 REPENTANCE = 20066;

    // Hands
    constexpr uint32 HAND_OF_FREEDOM = 1044;
    constexpr uint32 HAND_OF_PROTECTION = 10278;
    constexpr uint32 HAND_OF_SALVATION = 1038;
    constexpr uint32 HAND_OF_SACRIFICE = 6940;

    // Defensive cooldowns
    constexpr uint32 DIVINE_PROTECTION = 498;
    constexpr uint32 DIVINE_SHIELD = 642;
    constexpr uint32 AVENGING_WRATH = 31884;

    // Utility
    constexpr uint32 HAMMER_OF_JUSTICE = 10308;
    constexpr uint32 CLEANSE = 4987;
    constexpr uint32 PURIFY = 1152;
    constexpr uint32 TURN_EVIL = 10326;
    constexpr uint32 REDEMPTION = 48950;
    constexpr uint32 DIVINE_PLEA = 54428;
}

// ============================================================================
// HELPER MACROS
// ============================================================================

// GET_CURRENT_TARGET is already defined in Playerbots.h
#define CAST_SPELL(spellId) botAI->CastSpell(spellId, GET_CURRENT_TARGET())
#define CAN_CAST(spellId) botAI->CanCastSpell(spellId, GET_CURRENT_TARGET(), true)

// ============================================================================
// AURA EXECUTORS
// ============================================================================

static bool ExecuteDevotionAura(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::DEVOTION_AURA)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::DEVOTION_AURA, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::DEVOTION_AURA, bot);
}

static bool ExecuteRetributionAura(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::RETRIBUTION_AURA)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::RETRIBUTION_AURA, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::RETRIBUTION_AURA, bot);
}

static bool ExecuteConcentrationAura(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::CONCENTRATION_AURA)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::CONCENTRATION_AURA, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::CONCENTRATION_AURA, bot);
}

static bool ExecuteCrusaderAura(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::CRUSADER_AURA)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::CRUSADER_AURA, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::CRUSADER_AURA, bot);
}

// ============================================================================
// SEAL EXECUTORS
// ============================================================================

static bool ExecuteSealOfRighteousness(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SEAL_OF_RIGHTEOUSNESS)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::SEAL_OF_RIGHTEOUSNESS, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::SEAL_OF_RIGHTEOUSNESS, bot);
}

static bool ExecuteSealOfCommand(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SEAL_OF_COMMAND)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::SEAL_OF_COMMAND, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::SEAL_OF_COMMAND, bot);
}

static bool ExecuteSealOfVengeance(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    // Try Vengeance (Alliance) first, then Corruption (Horde)
    if (bot->HasSpell(PaladinSpells::SEAL_OF_VENGEANCE))
    {
        if (botAI->CanCastSpell(PaladinSpells::SEAL_OF_VENGEANCE, bot, true))
            return botAI->CastSpell(PaladinSpells::SEAL_OF_VENGEANCE, bot);
    }
    if (bot->HasSpell(PaladinSpells::SEAL_OF_CORRUPTION))
    {
        if (botAI->CanCastSpell(PaladinSpells::SEAL_OF_CORRUPTION, bot, true))
            return botAI->CastSpell(PaladinSpells::SEAL_OF_CORRUPTION, bot);
    }
    return false;
}

static bool ExecuteSealOfLight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SEAL_OF_LIGHT)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::SEAL_OF_LIGHT, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::SEAL_OF_LIGHT, bot);
}

static bool ExecuteSealOfWisdom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SEAL_OF_WISDOM)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::SEAL_OF_WISDOM, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::SEAL_OF_WISDOM, bot);
}

// ============================================================================
// JUDGMENT EXECUTORS
// ============================================================================

static bool ExecuteJudgementOfLight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::JUDGEMENT_OF_LIGHT)) return false;
    if (!CAN_CAST(PaladinSpells::JUDGEMENT_OF_LIGHT)) return false;
    return CAST_SPELL(PaladinSpells::JUDGEMENT_OF_LIGHT);
}

static bool ExecuteJudgementOfWisdom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::JUDGEMENT_OF_WISDOM)) return false;
    if (!CAN_CAST(PaladinSpells::JUDGEMENT_OF_WISDOM)) return false;
    return CAST_SPELL(PaladinSpells::JUDGEMENT_OF_WISDOM);
}

static bool ExecuteJudgementOfJustice(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::JUDGEMENT_OF_JUSTICE)) return false;
    if (!CAN_CAST(PaladinSpells::JUDGEMENT_OF_JUSTICE)) return false;
    return CAST_SPELL(PaladinSpells::JUDGEMENT_OF_JUSTICE);
}

// ============================================================================
// BLESSING EXECUTORS
// ============================================================================

static bool ExecuteBlessingOfMight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::BLESSING_OF_MIGHT)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::BLESSING_OF_MIGHT, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::BLESSING_OF_MIGHT, bot);
}

static bool ExecuteBlessingOfKings(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::BLESSING_OF_KINGS)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::BLESSING_OF_KINGS, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::BLESSING_OF_KINGS, bot);
}

static bool ExecuteBlessingOfWisdom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::BLESSING_OF_WISDOM)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::BLESSING_OF_WISDOM, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::BLESSING_OF_WISDOM, bot);
}

static bool ExecuteBlessingOfSanctuary(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::BLESSING_OF_SANCTUARY)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::BLESSING_OF_SANCTUARY, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::BLESSING_OF_SANCTUARY, bot);
}

// ============================================================================
// HEALING EXECUTORS
// ============================================================================

static bool ExecuteHolyLight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HOLY_LIGHT)) return false;
    // Heal self or lowest health party member
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to heal")->Get();
    if (!target) target = bot;
    if (!botAI->CanCastSpell(PaladinSpells::HOLY_LIGHT, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::HOLY_LIGHT, target);
}

static bool ExecuteFlashOfLight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::FLASH_OF_LIGHT)) return false;
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to heal")->Get();
    if (!target) target = bot;
    if (!botAI->CanCastSpell(PaladinSpells::FLASH_OF_LIGHT, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::FLASH_OF_LIGHT, target);
}

static bool ExecuteHolyShock(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HOLY_SHOCK)) return false;
    // Can be used offensively or defensively
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to heal")->Get();
    if (target && target->GetHealthPct() < 50)
    {
        if (botAI->CanCastSpell(PaladinSpells::HOLY_SHOCK, target, true))
            return botAI->CastSpell(PaladinSpells::HOLY_SHOCK, target);
    }
    // Use offensively
    target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!CAN_CAST(PaladinSpells::HOLY_SHOCK)) return false;
    return CAST_SPELL(PaladinSpells::HOLY_SHOCK);
}

static bool ExecuteLayOnHands(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::LAY_ON_HANDS)) return false;
    // Emergency heal - find lowest health target
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to heal")->Get();
    if (!target) target = bot;
    if (target->GetHealthPct() > 15) return false;  // Only use in emergencies
    if (!botAI->CanCastSpell(PaladinSpells::LAY_ON_HANDS, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::LAY_ON_HANDS, target);
}

static bool ExecuteBeaconOfLight(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::BEACON_OF_LIGHT)) return false;
    // Put beacon on tank
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("main tank")->Get();
    if (!target) target = botAI->GetMaster();
    if (!target) return false;
    if (!botAI->CanCastSpell(PaladinSpells::BEACON_OF_LIGHT, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::BEACON_OF_LIGHT, target);
}

static bool ExecuteSacredShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SACRED_SHIELD)) return false;
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("main tank")->Get();
    if (!target) target = botAI->GetMaster();
    if (!target) target = bot;
    if (!botAI->CanCastSpell(PaladinSpells::SACRED_SHIELD, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::SACRED_SHIELD, target);
}

// ============================================================================
// OFFENSIVE EXECUTORS
// ============================================================================

static bool ExecuteConsecration(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::CONSECRATION)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::CONSECRATION, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::CONSECRATION, bot);
}

static bool ExecuteExorcism(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::EXORCISM)) return false;
    if (!CAN_CAST(PaladinSpells::EXORCISM)) return false;
    return CAST_SPELL(PaladinSpells::EXORCISM);
}

static bool ExecuteHolyWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HOLY_WRATH)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::HOLY_WRATH, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::HOLY_WRATH, bot);
}

// ============================================================================
// PROTECTION EXECUTORS
// ============================================================================

static bool ExecuteRighteousFury(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::RIGHTEOUS_FURY)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::RIGHTEOUS_FURY, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::RIGHTEOUS_FURY, bot);
}

static bool ExecuteHammerOfTheRighteous(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAMMER_OF_THE_RIGHTEOUS)) return false;
    if (!CAN_CAST(PaladinSpells::HAMMER_OF_THE_RIGHTEOUS)) return false;
    return CAST_SPELL(PaladinSpells::HAMMER_OF_THE_RIGHTEOUS);
}

static bool ExecuteShieldOfRighteousness(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::SHIELD_OF_RIGHTEOUSNESS)) return false;
    if (!CAN_CAST(PaladinSpells::SHIELD_OF_RIGHTEOUSNESS)) return false;
    return CAST_SPELL(PaladinSpells::SHIELD_OF_RIGHTEOUSNESS);
}

static bool ExecuteAvengersShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::AVENGERS_SHIELD)) return false;
    if (!CAN_CAST(PaladinSpells::AVENGERS_SHIELD)) return false;
    return CAST_SPELL(PaladinSpells::AVENGERS_SHIELD);
}

static bool ExecuteHolyShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HOLY_SHIELD)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::HOLY_SHIELD, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::HOLY_SHIELD, bot);
}

static bool ExecuteRighteousDefense(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::RIGHTEOUS_DEFENSE)) return false;
    // Taunt off party member who has aggro
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to protect")->Get();
    if (!target) return false;
    if (!botAI->CanCastSpell(PaladinSpells::RIGHTEOUS_DEFENSE, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::RIGHTEOUS_DEFENSE, target);
}

static bool ExecuteHandOfReckoning(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAND_OF_RECKONING)) return false;
    if (!CAN_CAST(PaladinSpells::HAND_OF_RECKONING)) return false;
    return CAST_SPELL(PaladinSpells::HAND_OF_RECKONING);
}

// ============================================================================
// RETRIBUTION EXECUTORS
// ============================================================================

static bool ExecuteCrusaderStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::CRUSADER_STRIKE)) return false;
    if (!CAN_CAST(PaladinSpells::CRUSADER_STRIKE)) return false;
    return CAST_SPELL(PaladinSpells::CRUSADER_STRIKE);
}

static bool ExecuteDivineStorm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::DIVINE_STORM)) return false;
    if (!CAN_CAST(PaladinSpells::DIVINE_STORM)) return false;
    return CAST_SPELL(PaladinSpells::DIVINE_STORM);
}

static bool ExecuteHammerOfWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAMMER_OF_WRATH)) return false;
    if (!CAN_CAST(PaladinSpells::HAMMER_OF_WRATH)) return false;
    return CAST_SPELL(PaladinSpells::HAMMER_OF_WRATH);
}

static bool ExecuteRepentance(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::REPENTANCE)) return false;
    if (!CAN_CAST(PaladinSpells::REPENTANCE)) return false;
    return CAST_SPELL(PaladinSpells::REPENTANCE);
}

// ============================================================================
// DEFENSIVE EXECUTORS
// ============================================================================

static bool ExecuteDivineProtection(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::DIVINE_PROTECTION)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::DIVINE_PROTECTION, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::DIVINE_PROTECTION, bot);
}

static bool ExecuteDivineShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::DIVINE_SHIELD)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::DIVINE_SHIELD, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::DIVINE_SHIELD, bot);
}

static bool ExecuteAvengingWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::AVENGING_WRATH)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::AVENGING_WRATH, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::AVENGING_WRATH, bot);
}

// ============================================================================
// UTILITY EXECUTORS
// ============================================================================

static bool ExecuteHammerOfJustice(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAMMER_OF_JUSTICE)) return false;
    if (!CAN_CAST(PaladinSpells::HAMMER_OF_JUSTICE)) return false;
    return CAST_SPELL(PaladinSpells::HAMMER_OF_JUSTICE);
}

static bool ExecuteCleanse(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::CLEANSE)) return false;
    // Find target that needs cleansing
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to cure")->Get();
    if (!target) return false;
    if (!botAI->CanCastSpell(PaladinSpells::CLEANSE, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::CLEANSE, target);
}

static bool ExecuteDivinePlea(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::DIVINE_PLEA)) return false;
    if (!botAI->CanCastSpell(PaladinSpells::DIVINE_PLEA, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::DIVINE_PLEA, bot);
}

static bool ExecuteHandOfFreedom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAND_OF_FREEDOM)) return false;
    // Cast on self or snared party member
    if (!botAI->CanCastSpell(PaladinSpells::HAND_OF_FREEDOM, bot, true)) return false;
    return botAI->CastSpell(PaladinSpells::HAND_OF_FREEDOM, bot);
}

static bool ExecuteHandOfProtection(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(PaladinSpells::HAND_OF_PROTECTION)) return false;
    Unit* target = botAI->GetAiObjectContext()->GetValue<Unit*>("party member to protect")->Get();
    if (!target) return false;
    if (!botAI->CanCastSpell(PaladinSpells::HAND_OF_PROTECTION, target, true)) return false;
    return botAI->CastSpell(PaladinSpells::HAND_OF_PROTECTION, target);
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
    (void)botAI; (void)state;
    return false;
}

static bool ExecuteReachMelee(PlayerbotAI* botAI, const BotStateVector& state)
{
    (void)botAI; (void)state;
    return false;
}

static bool ExecuteReachSpell(PlayerbotAI* botAI, const BotStateVector& state)
{
    (void)botAI; (void)state;
    return false;
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterPaladinActions(FSMEngine* engine)
{
    if (!engine) return;

    // Auras
    engine->RegisterAction({PaladinActions::DevotionAura, "devotion aura", ExecuteDevotionAura, nullptr, nullptr, PaladinSpells::DEVOTION_AURA});
    engine->RegisterAction({PaladinActions::RetributionAura, "retribution aura", ExecuteRetributionAura, nullptr, nullptr, PaladinSpells::RETRIBUTION_AURA});
    engine->RegisterAction({PaladinActions::ConcentrationAura, "concentration aura", ExecuteConcentrationAura, nullptr, nullptr, PaladinSpells::CONCENTRATION_AURA});
    engine->RegisterAction({PaladinActions::CrusaderAura, "crusader aura", ExecuteCrusaderAura, nullptr, nullptr, PaladinSpells::CRUSADER_AURA});

    // Seals
    engine->RegisterAction({PaladinActions::SealOfRighteousness, "seal of righteousness", ExecuteSealOfRighteousness, nullptr, nullptr, PaladinSpells::SEAL_OF_RIGHTEOUSNESS});
    engine->RegisterAction({PaladinActions::SealOfCommand, "seal of command", ExecuteSealOfCommand, nullptr, nullptr, PaladinSpells::SEAL_OF_COMMAND});
    engine->RegisterAction({PaladinActions::SealOfVengeance, "seal of vengeance", ExecuteSealOfVengeance, nullptr, nullptr, PaladinSpells::SEAL_OF_VENGEANCE});
    engine->RegisterAction({PaladinActions::SealOfLight, "seal of light", ExecuteSealOfLight, nullptr, nullptr, PaladinSpells::SEAL_OF_LIGHT});
    engine->RegisterAction({PaladinActions::SealOfWisdom, "seal of wisdom", ExecuteSealOfWisdom, nullptr, nullptr, PaladinSpells::SEAL_OF_WISDOM});

    // Judgments
    engine->RegisterAction({PaladinActions::JudgementOfLight, "judgement of light", ExecuteJudgementOfLight, nullptr, nullptr, PaladinSpells::JUDGEMENT_OF_LIGHT});
    engine->RegisterAction({PaladinActions::JudgementOfWisdom, "judgement of wisdom", ExecuteJudgementOfWisdom, nullptr, nullptr, PaladinSpells::JUDGEMENT_OF_WISDOM});
    engine->RegisterAction({PaladinActions::JudgementOfJustice, "judgement of justice", ExecuteJudgementOfJustice, nullptr, nullptr, PaladinSpells::JUDGEMENT_OF_JUSTICE});

    // Blessings
    engine->RegisterAction({PaladinActions::BlessingOfMight, "blessing of might", ExecuteBlessingOfMight, nullptr, nullptr, PaladinSpells::BLESSING_OF_MIGHT});
    engine->RegisterAction({PaladinActions::BlessingOfKings, "blessing of kings", ExecuteBlessingOfKings, nullptr, nullptr, PaladinSpells::BLESSING_OF_KINGS});
    engine->RegisterAction({PaladinActions::BlessingOfWisdom, "blessing of wisdom", ExecuteBlessingOfWisdom, nullptr, nullptr, PaladinSpells::BLESSING_OF_WISDOM});
    engine->RegisterAction({PaladinActions::BlessingOfSanctuary, "blessing of sanctuary", ExecuteBlessingOfSanctuary, nullptr, nullptr, PaladinSpells::BLESSING_OF_SANCTUARY});

    // Holy spells
    engine->RegisterAction({PaladinActions::HolyLight, "holy light", ExecuteHolyLight, nullptr, nullptr, PaladinSpells::HOLY_LIGHT});
    engine->RegisterAction({PaladinActions::FlashOfLight, "flash of light", ExecuteFlashOfLight, nullptr, nullptr, PaladinSpells::FLASH_OF_LIGHT});
    engine->RegisterAction({PaladinActions::HolyShock, "holy shock", ExecuteHolyShock, nullptr, nullptr, PaladinSpells::HOLY_SHOCK});
    engine->RegisterAction({PaladinActions::LayOnHands, "lay on hands", ExecuteLayOnHands, nullptr, nullptr, PaladinSpells::LAY_ON_HANDS});
    engine->RegisterAction({PaladinActions::BeaconOfLight, "beacon of light", ExecuteBeaconOfLight, nullptr, nullptr, PaladinSpells::BEACON_OF_LIGHT});
    engine->RegisterAction({PaladinActions::SacredShield, "sacred shield", ExecuteSacredShield, nullptr, nullptr, PaladinSpells::SACRED_SHIELD});

    // Offensive
    engine->RegisterAction({PaladinActions::Consecration, "consecration", ExecuteConsecration, nullptr, nullptr, PaladinSpells::CONSECRATION});
    engine->RegisterAction({PaladinActions::Exorcism, "exorcism", ExecuteExorcism, nullptr, nullptr, PaladinSpells::EXORCISM});
    engine->RegisterAction({PaladinActions::HolyWrath, "holy wrath", ExecuteHolyWrath, nullptr, nullptr, PaladinSpells::HOLY_WRATH});

    // Protection
    engine->RegisterAction({PaladinActions::RighteousFury, "righteous fury", ExecuteRighteousFury, nullptr, nullptr, PaladinSpells::RIGHTEOUS_FURY});
    engine->RegisterAction({PaladinActions::HammerOfTheRighteous, "hammer of the righteous", ExecuteHammerOfTheRighteous, nullptr, nullptr, PaladinSpells::HAMMER_OF_THE_RIGHTEOUS});
    engine->RegisterAction({PaladinActions::ShieldOfRighteousness, "shield of righteousness", ExecuteShieldOfRighteousness, nullptr, nullptr, PaladinSpells::SHIELD_OF_RIGHTEOUSNESS});
    engine->RegisterAction({PaladinActions::AvengersShield, "avenger's shield", ExecuteAvengersShield, nullptr, nullptr, PaladinSpells::AVENGERS_SHIELD});
    engine->RegisterAction({PaladinActions::HolyShield, "holy shield", ExecuteHolyShield, nullptr, nullptr, PaladinSpells::HOLY_SHIELD});
    engine->RegisterAction({PaladinActions::RighteousDefense, "righteous defense", ExecuteRighteousDefense, nullptr, nullptr, PaladinSpells::RIGHTEOUS_DEFENSE});
    engine->RegisterAction({PaladinActions::HandOfReckoning, "hand of reckoning", ExecuteHandOfReckoning, nullptr, nullptr, PaladinSpells::HAND_OF_RECKONING});

    // Retribution
    engine->RegisterAction({PaladinActions::CrusaderStrike, "crusader strike", ExecuteCrusaderStrike, nullptr, nullptr, PaladinSpells::CRUSADER_STRIKE});
    engine->RegisterAction({PaladinActions::DivineStorm, "divine storm", ExecuteDivineStorm, nullptr, nullptr, PaladinSpells::DIVINE_STORM});
    engine->RegisterAction({PaladinActions::HammerOfWrath, "hammer of wrath", ExecuteHammerOfWrath, nullptr, nullptr, PaladinSpells::HAMMER_OF_WRATH});
    engine->RegisterAction({PaladinActions::Repentance, "repentance", ExecuteRepentance, nullptr, nullptr, PaladinSpells::REPENTANCE});

    // Defensive
    engine->RegisterAction({PaladinActions::DivineProtection, "divine protection", ExecuteDivineProtection, nullptr, nullptr, PaladinSpells::DIVINE_PROTECTION});
    engine->RegisterAction({PaladinActions::DivineShield, "divine shield", ExecuteDivineShield, nullptr, nullptr, PaladinSpells::DIVINE_SHIELD});
    engine->RegisterAction({PaladinActions::AvengingWrath, "avenging wrath", ExecuteAvengingWrath, nullptr, nullptr, PaladinSpells::AVENGING_WRATH});

    // Utility
    engine->RegisterAction({PaladinActions::HammerOfJustice, "hammer of justice", ExecuteHammerOfJustice, nullptr, nullptr, PaladinSpells::HAMMER_OF_JUSTICE});
    engine->RegisterAction({PaladinActions::Cleanse, "cleanse", ExecuteCleanse, nullptr, nullptr, PaladinSpells::CLEANSE});
    engine->RegisterAction({PaladinActions::DivinePlea, "divine plea", ExecuteDivinePlea, nullptr, nullptr, PaladinSpells::DIVINE_PLEA});
    engine->RegisterAction({PaladinActions::HandOfFreedom, "hand of freedom", ExecuteHandOfFreedom, nullptr, nullptr, PaladinSpells::HAND_OF_FREEDOM});
    engine->RegisterAction({PaladinActions::HandOfProtection, "hand of protection", ExecuteHandOfProtection, nullptr, nullptr, PaladinSpells::HAND_OF_PROTECTION});

    // Basic actions
    engine->RegisterAction({PaladinActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({PaladinActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({PaladinActions::ReachMelee, "reach melee", ExecuteReachMelee});
    engine->RegisterAction({PaladinActions::ReachSpell, "reach spell", ExecuteReachSpell});
}
