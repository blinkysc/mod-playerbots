/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "MageExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "SpellAuras.h"

// ============================================================================
// SPELL NAMES - For level-appropriate spell resolution
// ============================================================================
// Using spell names instead of IDs allows the system to automatically
// resolve to the appropriate rank based on what the bot has learned.

// ============================================================================
// HELPER MACROS
// ============================================================================

// GET_CURRENT_TARGET is already defined in Playerbots.h

// ============================================================================
// ARMOR EXECUTORS
// ============================================================================

static bool ExecuteMoltenArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("molten armor", bot)) return false;
    return botAI->CastSpell("molten armor", bot);
}

static bool ExecuteMageArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("mage armor", bot)) return false;
    return botAI->CastSpell("mage armor", bot);
}

static bool ExecuteIceArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("ice armor", bot)) return false;
    return botAI->CastSpell("ice armor", bot);
}

static bool ExecuteFrostArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("frost armor", bot)) return false;
    return botAI->CastSpell("frost armor", bot);
}

static bool ExecuteArcaneIntellect(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("arcane intellect", bot)) return false;
    return botAI->CastSpell("arcane intellect", bot);
}

// ============================================================================
// COOLDOWN EXECUTORS
// ============================================================================

static bool ExecuteArcanePower(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("arcane power", bot)) return false;
    return botAI->CastSpell("arcane power", bot);
}

static bool ExecutePresenceOfMind(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("presence of mind", bot)) return false;
    return botAI->CastSpell("presence of mind", bot);
}

static bool ExecuteCombustion(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("combustion", bot)) return false;
    return botAI->CastSpell("combustion", bot);
}

static bool ExecuteIcyVeins(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("icy veins", bot)) return false;
    return botAI->CastSpell("icy veins", bot);
}

static bool ExecuteColdSnap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("cold snap", bot)) return false;
    return botAI->CastSpell("cold snap", bot);
}

static bool ExecuteMirrorImage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("mirror image", bot)) return false;
    return botAI->CastSpell("mirror image", bot);
}

// ============================================================================
// DEFENSIVE EXECUTORS
// ============================================================================

static bool ExecuteIceBlock(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("ice block", bot)) return false;
    return botAI->CastSpell("ice block", bot);
}

static bool ExecuteIceBarrier(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("ice barrier", bot)) return false;
    return botAI->CastSpell("ice barrier", bot);
}

static bool ExecuteManaShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("mana shield", bot)) return false;
    return botAI->CastSpell("mana shield", bot);
}

static bool ExecuteFrostWard(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("frost ward", bot)) return false;
    return botAI->CastSpell("frost ward", bot);
}

static bool ExecuteFireWard(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("fire ward", bot)) return false;
    return botAI->CastSpell("fire ward", bot);
}

static bool ExecuteBlink(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("blink", bot)) return false;
    return botAI->CastSpell("blink", bot);
}

static bool ExecuteInvisibility(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("invisibility", bot)) return false;
    return botAI->CastSpell("invisibility", bot);
}

// ============================================================================
// UTILITY EXECUTORS
// ============================================================================

static bool ExecuteEvocation(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("evocation", bot)) return false;
    return botAI->CastSpell("evocation", bot);
}

static bool ExecuteCounterspell(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("counterspell", target)) return false;
    return botAI->CastSpell("counterspell", target);
}

static bool ExecutePolymorph(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("polymorph", target)) return false;
    return botAI->CastSpell("polymorph", target);
}

static bool ExecuteSpellsteal(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("spellsteal", target)) return false;
    return botAI->CastSpell("spellsteal", target);
}

static bool ExecuteRemoveCurse(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("remove curse", bot)) return false;
    return botAI->CastSpell("remove curse", bot);
}

static bool ExecuteSummonWaterElemental(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (bot->GetPet()) return false;  // Already have a pet
    if (!botAI->CanCastSpell("summon water elemental", bot)) return false;
    return botAI->CastSpell("summon water elemental", bot);
}

// ============================================================================
// FIRE DAMAGE EXECUTORS
// ============================================================================

static bool ExecuteFireball(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("fireball", target)) return false;
    return botAI->CastSpell("fireball", target);
}

static bool ExecutePyroblast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("pyroblast", target)) return false;
    return botAI->CastSpell("pyroblast", target);
}

static bool ExecuteScorch(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("scorch", target)) return false;
    return botAI->CastSpell("scorch", target);
}

static bool ExecuteFireBlast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("fire blast", target)) return false;
    return botAI->CastSpell("fire blast", target);
}

static bool ExecuteLivingBomb(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("living bomb", target)) return false;
    return botAI->CastSpell("living bomb", target);
}

static bool ExecuteFlamestrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("flamestrike", target)) return false;
    return botAI->CastSpell("flamestrike", target);
}

static bool ExecuteBlastWave(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("blast wave", bot)) return false;
    return botAI->CastSpell("blast wave", bot);
}

static bool ExecuteDragonsBreath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("dragon's breath", bot)) return false;
    return botAI->CastSpell("dragon's breath", bot);
}

// ============================================================================
// FROST DAMAGE EXECUTORS
// ============================================================================

static bool ExecuteFrostbolt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("frostbolt", target)) return false;
    return botAI->CastSpell("frostbolt", target);
}

static bool ExecuteFrostfireBolt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("frostfire bolt", target)) return false;
    return botAI->CastSpell("frostfire bolt", target);
}

static bool ExecuteIceLance(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("ice lance", target)) return false;
    return botAI->CastSpell("ice lance", target);
}

static bool ExecuteBlizzard(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("blizzard", target)) return false;
    return botAI->CastSpell("blizzard", target);
}

static bool ExecuteConeOfCold(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("cone of cold", bot)) return false;
    return botAI->CastSpell("cone of cold", bot);
}

static bool ExecuteFrostNova(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("frost nova", bot)) return false;
    return botAI->CastSpell("frost nova", bot);
}

static bool ExecuteDeepFreeze(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("deep freeze", target)) return false;
    return botAI->CastSpell("deep freeze", target);
}

// ============================================================================
// ARCANE DAMAGE EXECUTORS
// ============================================================================

static bool ExecuteArcaneBlast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("arcane blast", target)) return false;
    return botAI->CastSpell("arcane blast", target);
}

static bool ExecuteArcaneMissiles(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("arcane missiles", target)) return false;
    return botAI->CastSpell("arcane missiles", target);
}

static bool ExecuteArcaneBarrage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target) return false;
    if (!botAI->CanCastSpell("arcane barrage", target)) return false;
    return botAI->CastSpell("arcane barrage", target);
}

static bool ExecuteArcaneExplosion(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot) return false;
    if (!botAI->CanCastSpell("arcane explosion", bot)) return false;
    return botAI->CastSpell("arcane explosion", bot);
}

// ============================================================================
// BASIC ACTION EXECUTORS
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
    return false;  // Handled by movement system
}

static bool ExecuteReachSpell(PlayerbotAI* botAI, const BotStateVector& state)
{
    (void)botAI; (void)state;
    return false;  // Handled by movement system
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterMageActions(FSMEngine* engine)
{
    if (!engine) return;

    // Armors
    engine->RegisterAction({MageActions::MoltenArmor, "molten armor", ExecuteMoltenArmor});
    engine->RegisterAction({MageActions::MageArmor, "mage armor", ExecuteMageArmor});
    engine->RegisterAction({MageActions::IceArmor, "ice armor", ExecuteIceArmor});
    engine->RegisterAction({MageActions::FrostArmor, "frost armor", ExecuteFrostArmor});
    engine->RegisterAction({MageActions::ArcaneIntellect, "arcane intellect", ExecuteArcaneIntellect});

    // Cooldowns
    engine->RegisterAction({MageActions::ArcanePower, "arcane power", ExecuteArcanePower});
    engine->RegisterAction({MageActions::PresenceOfMind, "presence of mind", ExecutePresenceOfMind});
    engine->RegisterAction({MageActions::Combustion, "combustion", ExecuteCombustion});
    engine->RegisterAction({MageActions::IcyVeins, "icy veins", ExecuteIcyVeins});
    engine->RegisterAction({MageActions::ColdSnap, "cold snap", ExecuteColdSnap});
    engine->RegisterAction({MageActions::MirrorImage, "mirror image", ExecuteMirrorImage});

    // Defensive
    engine->RegisterAction({MageActions::IceBlock, "ice block", ExecuteIceBlock});
    engine->RegisterAction({MageActions::IceBarrier, "ice barrier", ExecuteIceBarrier});
    engine->RegisterAction({MageActions::ManaShield, "mana shield", ExecuteManaShield});
    engine->RegisterAction({MageActions::FrostWard, "frost ward", ExecuteFrostWard});
    engine->RegisterAction({MageActions::FireWard, "fire ward", ExecuteFireWard});
    engine->RegisterAction({MageActions::Blink, "blink", ExecuteBlink});
    engine->RegisterAction({MageActions::Invisibility, "invisibility", ExecuteInvisibility});

    // Utility
    engine->RegisterAction({MageActions::Evocation, "evocation", ExecuteEvocation});
    engine->RegisterAction({MageActions::Counterspell, "counterspell", ExecuteCounterspell});
    engine->RegisterAction({MageActions::Polymorph, "polymorph", ExecutePolymorph});
    engine->RegisterAction({MageActions::Spellsteal, "spellsteal", ExecuteSpellsteal});
    engine->RegisterAction({MageActions::RemoveCurse, "remove curse", ExecuteRemoveCurse});
    engine->RegisterAction({MageActions::SummonWaterElemental, "summon water elemental", ExecuteSummonWaterElemental});

    // Fire damage
    engine->RegisterAction({MageActions::Fireball, "fireball", ExecuteFireball});
    engine->RegisterAction({MageActions::Pyroblast, "pyroblast", ExecutePyroblast});
    engine->RegisterAction({MageActions::Scorch, "scorch", ExecuteScorch});
    engine->RegisterAction({MageActions::FireBlast, "fire blast", ExecuteFireBlast});
    engine->RegisterAction({MageActions::LivingBomb, "living bomb", ExecuteLivingBomb});
    engine->RegisterAction({MageActions::Flamestrike, "flamestrike", ExecuteFlamestrike});
    engine->RegisterAction({MageActions::BlastWave, "blast wave", ExecuteBlastWave});
    engine->RegisterAction({MageActions::DragonsBreath, "dragon's breath", ExecuteDragonsBreath});

    // Frost damage
    engine->RegisterAction({MageActions::Frostbolt, "frostbolt", ExecuteFrostbolt});
    engine->RegisterAction({MageActions::FrostfireBolt, "frostfire bolt", ExecuteFrostfireBolt});
    engine->RegisterAction({MageActions::IceLance, "ice lance", ExecuteIceLance});
    engine->RegisterAction({MageActions::Blizzard, "blizzard", ExecuteBlizzard});
    engine->RegisterAction({MageActions::ConeOfCold, "cone of cold", ExecuteConeOfCold});
    engine->RegisterAction({MageActions::FrostNova, "frost nova", ExecuteFrostNova});
    engine->RegisterAction({MageActions::DeepFreeze, "deep freeze", ExecuteDeepFreeze});

    // Arcane damage
    engine->RegisterAction({MageActions::ArcaneBlast, "arcane blast", ExecuteArcaneBlast});
    engine->RegisterAction({MageActions::ArcaneMissiles, "arcane missiles", ExecuteArcaneMissiles});
    engine->RegisterAction({MageActions::ArcaneBarrage, "arcane barrage", ExecuteArcaneBarrage});
    engine->RegisterAction({MageActions::ArcaneExplosion, "arcane explosion", ExecuteArcaneExplosion});

    // Basic actions
    engine->RegisterAction({MageActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({MageActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({MageActions::ReachSpell, "reach spell", ExecuteReachSpell});
}
