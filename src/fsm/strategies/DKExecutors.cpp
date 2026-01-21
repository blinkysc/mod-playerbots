/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "DKExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "SpellAuras.h"

// WotLK 3.3.5a Death Knight Spell IDs
namespace DKSpells
{
    // Presences (verified from AzerothCore spell_dk.cpp)
    constexpr uint32 BLOOD_PRESENCE = 48266;
    constexpr uint32 FROST_PRESENCE = 48263;
    constexpr uint32 UNHOLY_PRESENCE = 48265;

    // Blood abilities
    constexpr uint32 HEART_STRIKE = 55262;
    constexpr uint32 BLOOD_STRIKE = 49930;
    constexpr uint32 DEATH_STRIKE = 49924;
    constexpr uint32 RUNE_STRIKE = 56815;
    constexpr uint32 BLOOD_BOIL = 49941;
    constexpr uint32 PESTILENCE = 50842;
    constexpr uint32 MARK_OF_BLOOD = 49005;
    constexpr uint32 VAMPIRIC_BLOOD = 55233;
    constexpr uint32 RUNE_TAP = 48982;
    constexpr uint32 DANCING_RUNE_WEAPON = 49028;

    // Frost abilities
    constexpr uint32 ICY_TOUCH = 49909;
    constexpr uint32 OBLITERATE = 51425;
    constexpr uint32 FROST_STRIKE = 55268;
    constexpr uint32 HOWLING_BLAST = 51411;
    constexpr uint32 UNBREAKABLE_ARMOR = 51271;
    constexpr uint32 DEATHCHILL = 49796;

    // Unholy abilities
    constexpr uint32 PLAGUE_STRIKE = 49921;
    constexpr uint32 SCOURGE_STRIKE = 55271;
    constexpr uint32 DEATH_COIL = 49895;
    constexpr uint32 DEATH_AND_DECAY = 49938;
    constexpr uint32 SUMMON_GARGOYLE = 49206;
    constexpr uint32 UNHOLY_FRENZY = 49016;
    constexpr uint32 GHOUL_FRENZY = 63560;
    constexpr uint32 CORPSE_EXPLOSION = 51328;

    // Utility
    constexpr uint32 DEATH_GRIP = 49576;
    constexpr uint32 MIND_FREEZE = 47528;
    constexpr uint32 STRANGULATE = 47476;
    constexpr uint32 CHAINS_OF_ICE = 45524;
    constexpr uint32 ICEBOUND_FORTITUDE = 48792;
    constexpr uint32 ANTI_MAGIC_SHELL = 48707;
    constexpr uint32 ARMY_OF_THE_DEAD = 42650;
    constexpr uint32 EMPOWER_RUNE_WEAPON = 47568;
    constexpr uint32 HORN_OF_WINTER = 57623;
    constexpr uint32 RAISE_DEAD = 46584;
    constexpr uint32 DARK_COMMAND = 56222;

    // Diseases
    constexpr uint32 FROST_FEVER = 55095;  // Applied by Icy Touch
    constexpr uint32 BLOOD_PLAGUE = 55078; // Applied by Plague Strike
}

// ============================================================================
// HELPER MACROS
// ============================================================================

// GET_CURRENT_TARGET is already defined in Playerbots.h
#define CAST_SPELL(spellId) botAI->CastSpell(spellId, GET_CURRENT_TARGET())
#define CAN_CAST(spellId) botAI->CanCastSpell(spellId, GET_CURRENT_TARGET(), true)

// ============================================================================
// PRESENCE EXECUTORS
// ============================================================================

static bool ExecuteBloodPresence(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::BLOOD_PRESENCE)) return false;
    if (!botAI->CanCastSpell(DKSpells::BLOOD_PRESENCE, bot, true)) return false;
    return botAI->CastSpell(DKSpells::BLOOD_PRESENCE, bot);
}

static bool ExecuteFrostPresence(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::FROST_PRESENCE)) return false;
    if (!botAI->CanCastSpell(DKSpells::FROST_PRESENCE, bot, true)) return false;
    return botAI->CastSpell(DKSpells::FROST_PRESENCE, bot);
}

static bool ExecuteUnholyPresence(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::UNHOLY_PRESENCE)) return false;
    if (!botAI->CanCastSpell(DKSpells::UNHOLY_PRESENCE, bot, true)) return false;
    return botAI->CastSpell(DKSpells::UNHOLY_PRESENCE, bot);
}

// ============================================================================
// BLOOD EXECUTORS
// ============================================================================

static bool ExecuteHeartStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::HEART_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::HEART_STRIKE)) return false;
    return CAST_SPELL(DKSpells::HEART_STRIKE);
}

static bool ExecuteBloodStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::BLOOD_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::BLOOD_STRIKE)) return false;
    return CAST_SPELL(DKSpells::BLOOD_STRIKE);
}

static bool ExecuteDeathStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DEATH_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::DEATH_STRIKE)) return false;
    return CAST_SPELL(DKSpells::DEATH_STRIKE);
}

static bool ExecuteRuneStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::RUNE_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::RUNE_STRIKE)) return false;
    return CAST_SPELL(DKSpells::RUNE_STRIKE);
}

static bool ExecuteBloodBoil(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::BLOOD_BOIL)) return false;
    if (!botAI->CanCastSpell(DKSpells::BLOOD_BOIL, bot, true)) return false;
    return botAI->CastSpell(DKSpells::BLOOD_BOIL, bot);
}

static bool ExecutePestilence(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::PESTILENCE)) return false;
    if (!CAN_CAST(DKSpells::PESTILENCE)) return false;
    return CAST_SPELL(DKSpells::PESTILENCE);
}

static bool ExecuteVampiricBlood(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::VAMPIRIC_BLOOD)) return false;
    if (!botAI->CanCastSpell(DKSpells::VAMPIRIC_BLOOD, bot, true)) return false;
    return botAI->CastSpell(DKSpells::VAMPIRIC_BLOOD, bot);
}

static bool ExecuteRuneTap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::RUNE_TAP)) return false;
    if (!botAI->CanCastSpell(DKSpells::RUNE_TAP, bot, true)) return false;
    return botAI->CastSpell(DKSpells::RUNE_TAP, bot);
}

static bool ExecuteDancingRuneWeapon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DANCING_RUNE_WEAPON)) return false;
    if (!CAN_CAST(DKSpells::DANCING_RUNE_WEAPON)) return false;
    return CAST_SPELL(DKSpells::DANCING_RUNE_WEAPON);
}

// ============================================================================
// FROST EXECUTORS
// ============================================================================

static bool ExecuteIcyTouch(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::ICY_TOUCH)) return false;
    if (!CAN_CAST(DKSpells::ICY_TOUCH)) return false;
    return CAST_SPELL(DKSpells::ICY_TOUCH);
}

static bool ExecuteObliterate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::OBLITERATE)) return false;
    if (!CAN_CAST(DKSpells::OBLITERATE)) return false;
    return CAST_SPELL(DKSpells::OBLITERATE);
}

static bool ExecuteFrostStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::FROST_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::FROST_STRIKE)) return false;
    return CAST_SPELL(DKSpells::FROST_STRIKE);
}

static bool ExecuteHowlingBlast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::HOWLING_BLAST)) return false;
    if (!CAN_CAST(DKSpells::HOWLING_BLAST)) return false;
    return CAST_SPELL(DKSpells::HOWLING_BLAST);
}

static bool ExecuteUnbreakableArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::UNBREAKABLE_ARMOR)) return false;
    if (!botAI->CanCastSpell(DKSpells::UNBREAKABLE_ARMOR, bot, true)) return false;
    return botAI->CastSpell(DKSpells::UNBREAKABLE_ARMOR, bot);
}

static bool ExecuteDeathchill(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DEATHCHILL)) return false;
    if (!botAI->CanCastSpell(DKSpells::DEATHCHILL, bot, true)) return false;
    return botAI->CastSpell(DKSpells::DEATHCHILL, bot);
}

// ============================================================================
// UNHOLY EXECUTORS
// ============================================================================

static bool ExecutePlagueStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::PLAGUE_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::PLAGUE_STRIKE)) return false;
    return CAST_SPELL(DKSpells::PLAGUE_STRIKE);
}

static bool ExecuteScourgeStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::SCOURGE_STRIKE)) return false;
    if (!CAN_CAST(DKSpells::SCOURGE_STRIKE)) return false;
    return CAST_SPELL(DKSpells::SCOURGE_STRIKE);
}

static bool ExecuteDeathCoil(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DEATH_COIL)) return false;
    if (!CAN_CAST(DKSpells::DEATH_COIL)) return false;
    return CAST_SPELL(DKSpells::DEATH_COIL);
}

static bool ExecuteDeathAndDecay(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DEATH_AND_DECAY)) return false;
    if (!botAI->CanCastSpell(DKSpells::DEATH_AND_DECAY, bot, true)) return false;
    return botAI->CastSpell(DKSpells::DEATH_AND_DECAY, bot);
}

static bool ExecuteSummonGargoyle(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::SUMMON_GARGOYLE)) return false;
    if (!CAN_CAST(DKSpells::SUMMON_GARGOYLE)) return false;
    return CAST_SPELL(DKSpells::SUMMON_GARGOYLE);
}

static bool ExecuteUnholyFrenzy(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::UNHOLY_FRENZY)) return false;
    if (!botAI->CanCastSpell(DKSpells::UNHOLY_FRENZY, bot, true)) return false;
    return botAI->CastSpell(DKSpells::UNHOLY_FRENZY, bot);
}

static bool ExecuteGhoulFrenzy(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::GHOUL_FRENZY)) return false;
    // Cast on pet
    Unit* pet = bot->GetPet();
    if (!pet) return false;
    if (!botAI->CanCastSpell(DKSpells::GHOUL_FRENZY, pet, true)) return false;
    return botAI->CastSpell(DKSpells::GHOUL_FRENZY, pet);
}

// ============================================================================
// UTILITY EXECUTORS
// ============================================================================

static bool ExecuteDeathGrip(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DEATH_GRIP)) return false;
    if (!CAN_CAST(DKSpells::DEATH_GRIP)) return false;
    return CAST_SPELL(DKSpells::DEATH_GRIP);
}

static bool ExecuteMindFreeze(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::MIND_FREEZE)) return false;
    if (!CAN_CAST(DKSpells::MIND_FREEZE)) return false;
    return CAST_SPELL(DKSpells::MIND_FREEZE);
}

static bool ExecuteStrangulate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::STRANGULATE)) return false;
    if (!CAN_CAST(DKSpells::STRANGULATE)) return false;
    return CAST_SPELL(DKSpells::STRANGULATE);
}

static bool ExecuteChainsOfIce(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::CHAINS_OF_ICE)) return false;
    if (!CAN_CAST(DKSpells::CHAINS_OF_ICE)) return false;
    return CAST_SPELL(DKSpells::CHAINS_OF_ICE);
}

static bool ExecuteIceboundFortitude(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::ICEBOUND_FORTITUDE)) return false;
    if (!botAI->CanCastSpell(DKSpells::ICEBOUND_FORTITUDE, bot, true)) return false;
    return botAI->CastSpell(DKSpells::ICEBOUND_FORTITUDE, bot);
}

static bool ExecuteAntiMagicShell(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::ANTI_MAGIC_SHELL)) return false;
    if (!botAI->CanCastSpell(DKSpells::ANTI_MAGIC_SHELL, bot, true)) return false;
    return botAI->CastSpell(DKSpells::ANTI_MAGIC_SHELL, bot);
}

static bool ExecuteArmyOfTheDead(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::ARMY_OF_THE_DEAD)) return false;
    if (!botAI->CanCastSpell(DKSpells::ARMY_OF_THE_DEAD, bot, true)) return false;
    return botAI->CastSpell(DKSpells::ARMY_OF_THE_DEAD, bot);
}

static bool ExecuteEmpowerRuneWeapon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::EMPOWER_RUNE_WEAPON)) return false;
    if (!botAI->CanCastSpell(DKSpells::EMPOWER_RUNE_WEAPON, bot, true)) return false;
    return botAI->CastSpell(DKSpells::EMPOWER_RUNE_WEAPON, bot);
}

static bool ExecuteHornOfWinter(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::HORN_OF_WINTER)) return false;
    if (!botAI->CanCastSpell(DKSpells::HORN_OF_WINTER, bot, true)) return false;
    return botAI->CastSpell(DKSpells::HORN_OF_WINTER, bot);
}

static bool ExecuteRaiseDead(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::RAISE_DEAD)) return false;
    if (bot->GetPet()) return false;  // Already have a pet
    if (!botAI->CanCastSpell(DKSpells::RAISE_DEAD, bot, true)) return false;
    return botAI->CastSpell(DKSpells::RAISE_DEAD, bot);
}

static bool ExecuteDarkCommand(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot || !bot->HasSpell(DKSpells::DARK_COMMAND)) return false;
    if (!CAN_CAST(DKSpells::DARK_COMMAND)) return false;
    return CAST_SPELL(DKSpells::DARK_COMMAND);
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

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterDeathKnightActions(FSMEngine* engine)
{
    if (!engine) return;

    // Presences
    engine->RegisterAction({DeathKnightActions::BloodPresence, "blood presence", ExecuteBloodPresence, nullptr, nullptr, DKSpells::BLOOD_PRESENCE});
    engine->RegisterAction({DeathKnightActions::FrostPresence, "frost presence", ExecuteFrostPresence, nullptr, nullptr, DKSpells::FROST_PRESENCE});
    engine->RegisterAction({DeathKnightActions::UnholyPresence, "unholy presence", ExecuteUnholyPresence, nullptr, nullptr, DKSpells::UNHOLY_PRESENCE});

    // Blood
    engine->RegisterAction({DeathKnightActions::HeartStrike, "heart strike", ExecuteHeartStrike, nullptr, nullptr, DKSpells::HEART_STRIKE});
    engine->RegisterAction({DeathKnightActions::BloodStrike, "blood strike", ExecuteBloodStrike, nullptr, nullptr, DKSpells::BLOOD_STRIKE});
    engine->RegisterAction({DeathKnightActions::DeathStrike, "death strike", ExecuteDeathStrike, nullptr, nullptr, DKSpells::DEATH_STRIKE});
    engine->RegisterAction({DeathKnightActions::RuneStrike, "rune strike", ExecuteRuneStrike, nullptr, nullptr, DKSpells::RUNE_STRIKE});
    engine->RegisterAction({DeathKnightActions::BloodBoil, "blood boil", ExecuteBloodBoil, nullptr, nullptr, DKSpells::BLOOD_BOIL});
    engine->RegisterAction({DeathKnightActions::Pestilence, "pestilence", ExecutePestilence, nullptr, nullptr, DKSpells::PESTILENCE});
    engine->RegisterAction({DeathKnightActions::VampiricBlood, "vampiric blood", ExecuteVampiricBlood, nullptr, nullptr, DKSpells::VAMPIRIC_BLOOD});
    engine->RegisterAction({DeathKnightActions::RuneTap, "rune tap", ExecuteRuneTap, nullptr, nullptr, DKSpells::RUNE_TAP});
    engine->RegisterAction({DeathKnightActions::DancingRuneWeapon, "dancing rune weapon", ExecuteDancingRuneWeapon, nullptr, nullptr, DKSpells::DANCING_RUNE_WEAPON});

    // Frost
    engine->RegisterAction({DeathKnightActions::IcyTouch, "icy touch", ExecuteIcyTouch, nullptr, nullptr, DKSpells::ICY_TOUCH});
    engine->RegisterAction({DeathKnightActions::Obliterate, "obliterate", ExecuteObliterate, nullptr, nullptr, DKSpells::OBLITERATE});
    engine->RegisterAction({DeathKnightActions::FrostStrike, "frost strike", ExecuteFrostStrike, nullptr, nullptr, DKSpells::FROST_STRIKE});
    engine->RegisterAction({DeathKnightActions::HowlingBlast, "howling blast", ExecuteHowlingBlast, nullptr, nullptr, DKSpells::HOWLING_BLAST});
    engine->RegisterAction({DeathKnightActions::UnbreakableArmor, "unbreakable armor", ExecuteUnbreakableArmor, nullptr, nullptr, DKSpells::UNBREAKABLE_ARMOR});
    engine->RegisterAction({DeathKnightActions::Deathchill, "deathchill", ExecuteDeathchill, nullptr, nullptr, DKSpells::DEATHCHILL});

    // Unholy
    engine->RegisterAction({DeathKnightActions::PlagueStrike, "plague strike", ExecutePlagueStrike, nullptr, nullptr, DKSpells::PLAGUE_STRIKE});
    engine->RegisterAction({DeathKnightActions::ScourgeStrike, "scourge strike", ExecuteScourgeStrike, nullptr, nullptr, DKSpells::SCOURGE_STRIKE});
    engine->RegisterAction({DeathKnightActions::DeathCoil, "death coil", ExecuteDeathCoil, nullptr, nullptr, DKSpells::DEATH_COIL});
    engine->RegisterAction({DeathKnightActions::DeathAndDecay, "death and decay", ExecuteDeathAndDecay, nullptr, nullptr, DKSpells::DEATH_AND_DECAY});
    engine->RegisterAction({DeathKnightActions::SummonGargoyle, "summon gargoyle", ExecuteSummonGargoyle, nullptr, nullptr, DKSpells::SUMMON_GARGOYLE});
    engine->RegisterAction({DeathKnightActions::UnholyFrenzy, "unholy frenzy", ExecuteUnholyFrenzy, nullptr, nullptr, DKSpells::UNHOLY_FRENZY});
    engine->RegisterAction({DeathKnightActions::GhoulFrenzy, "ghoul frenzy", ExecuteGhoulFrenzy, nullptr, nullptr, DKSpells::GHOUL_FRENZY});

    // Utility
    engine->RegisterAction({DeathKnightActions::DeathGrip, "death grip", ExecuteDeathGrip, nullptr, nullptr, DKSpells::DEATH_GRIP});
    engine->RegisterAction({DeathKnightActions::MindFreeze, "mind freeze", ExecuteMindFreeze, nullptr, nullptr, DKSpells::MIND_FREEZE});
    engine->RegisterAction({DeathKnightActions::Strangulate, "strangulate", ExecuteStrangulate, nullptr, nullptr, DKSpells::STRANGULATE});
    engine->RegisterAction({DeathKnightActions::ChainsOfIce, "chains of ice", ExecuteChainsOfIce, nullptr, nullptr, DKSpells::CHAINS_OF_ICE});
    engine->RegisterAction({DeathKnightActions::IceboundFortitude, "icebound fortitude", ExecuteIceboundFortitude, nullptr, nullptr, DKSpells::ICEBOUND_FORTITUDE});
    engine->RegisterAction({DeathKnightActions::AntiMagicShell, "anti-magic shell", ExecuteAntiMagicShell, nullptr, nullptr, DKSpells::ANTI_MAGIC_SHELL});
    engine->RegisterAction({DeathKnightActions::ArmyOfTheDead, "army of the dead", ExecuteArmyOfTheDead, nullptr, nullptr, DKSpells::ARMY_OF_THE_DEAD});
    engine->RegisterAction({DeathKnightActions::EmpowerRuneWeapon, "empower rune weapon", ExecuteEmpowerRuneWeapon, nullptr, nullptr, DKSpells::EMPOWER_RUNE_WEAPON});
    engine->RegisterAction({DeathKnightActions::HornOfWinter, "horn of winter", ExecuteHornOfWinter, nullptr, nullptr, DKSpells::HORN_OF_WINTER});
    engine->RegisterAction({DeathKnightActions::RaiseDead, "raise dead", ExecuteRaiseDead, nullptr, nullptr, DKSpells::RAISE_DEAD});
    engine->RegisterAction({DeathKnightActions::DarkCommand, "dark command", ExecuteDarkCommand, nullptr, nullptr, DKSpells::DARK_COMMAND});

    // Basic actions
    engine->RegisterAction({DeathKnightActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({DeathKnightActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({DeathKnightActions::ReachMelee, "reach melee", ExecuteReachMelee});
}
