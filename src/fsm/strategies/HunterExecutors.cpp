/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "HunterExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "Pet.h"

// Helper macros (GET_CURRENT_TARGET, GET_BOT, GET_MASTER defined in Playerbots.h)

// ============================================================================
// BASIC ACTIONS
// ============================================================================

static bool ExecuteMelee(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;

    Player* bot = botAI->GetBot();
    if (!bot)
        return false;

    // Ensure auto-attack is on
    bot->Attack(target, true);
    return true;
}

static bool ExecuteFollow(PlayerbotAI* botAI, const BotStateVector& state)
{
    // TODO: Implement using MovementAction infrastructure
    // For now, return false - existing engine will handle follow
    (void)botAI; (void)state;  // Suppress unused warnings
    return false;
}

static bool ExecuteAutoShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;

    if (!botAI->CanCastSpell("auto shot", target))
        return false;

    return botAI->CastSpell("auto shot", target);
}

// ============================================================================
// ASPECTS
// ============================================================================

static bool ExecuteAspectOfTheHawk(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the hawk", bot))
        return false;
    return botAI->CastSpell("aspect of the hawk", bot);
}

static bool ExecuteAspectOfTheDragonhawk(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the dragonhawk", bot))
        return false;
    return botAI->CastSpell("aspect of the dragonhawk", bot);
}

static bool ExecuteAspectOfTheViper(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the viper", bot))
        return false;
    return botAI->CastSpell("aspect of the viper", bot);
}

static bool ExecuteAspectOfTheWild(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the wild", bot))
        return false;
    return botAI->CastSpell("aspect of the wild", bot);
}

static bool ExecuteAspectOfThePack(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the pack", bot))
        return false;
    return botAI->CastSpell("aspect of the pack", bot);
}

static bool ExecuteAspectOfTheBeast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the beast", bot))
        return false;
    return botAI->CastSpell("aspect of the beast", bot);
}

static bool ExecuteAspectOfTheMonkey(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the monkey", bot))
        return false;
    return botAI->CastSpell("aspect of the monkey", bot);
}

static bool ExecuteAspectOfTheCheetah(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("aspect of the cheetah", bot))
        return false;
    return botAI->CastSpell("aspect of the cheetah", bot);
}

// ============================================================================
// PET MANAGEMENT
// ============================================================================

static bool ExecuteCallPet(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("call pet", bot))
        return false;
    return botAI->CastSpell("call pet", bot);
}

static bool ExecuteRevivePet(PlayerbotAI* botAI, const BotStateVector& state)
{
    Pet* pet = botAI->GetBot()->GetPet();
    if (!pet)
        return false;
    if (!botAI->CanCastSpell("revive pet", pet))
        return false;
    return botAI->CastSpell("revive pet", pet);
}

static bool ExecuteMendPet(PlayerbotAI* botAI, const BotStateVector& state)
{
    Pet* pet = botAI->GetBot()->GetPet();
    if (!pet || pet->GetHealthPct() > 70)
        return false;
    if (!botAI->CanCastSpell("mend pet", pet))
        return false;
    return botAI->CastSpell("mend pet", pet);
}

static bool ExecuteFeedPet(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Feed pet is handled by a specific action that finds suitable food
    // For now, return false - this needs special item handling
    return false;
}

static bool ExecuteDismissPet(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!bot->GetPet())
        return false;
    if (!botAI->CanCastSpell("dismiss pet", bot))
        return false;
    return botAI->CastSpell("dismiss pet", bot);
}

static bool ExecuteKillCommand(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    Pet* pet = botAI->GetBot()->GetPet();
    if (!pet || !pet->IsAlive())
        return false;
    if (!botAI->CanCastSpell("kill command", target))
        return false;
    return botAI->CastSpell("kill command", target);
}

static bool ExecuteBestialWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    Pet* pet = botAI->GetBot()->GetPet();
    if (!pet || !pet->IsAlive())
        return false;
    if (!botAI->CanCastSpell("bestial wrath", target))
        return false;
    return botAI->CastSpell("bestial wrath", target);
}

static bool ExecuteIntimidation(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    Pet* pet = botAI->GetBot()->GetPet();
    if (!pet || !pet->IsAlive())
        return false;
    if (!botAI->CanCastSpell("intimidation", target))
        return false;
    return botAI->CastSpell("intimidation", target);
}

// ============================================================================
// SHOTS - MARKSMANSHIP
// ============================================================================

static bool ExecuteArcaneShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("arcane shot", target))
        return false;
    return botAI->CastSpell("arcane shot", target);
}

static bool ExecuteSteadyShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("steady shot", target))
        return false;
    return botAI->CastSpell("steady shot", target);
}

static bool ExecuteAimedShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("aimed shot", target))
        return false;
    return botAI->CastSpell("aimed shot", target);
}

static bool ExecuteMultiShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("multi-shot", target))
        return false;
    return botAI->CastSpell("multi-shot", target);
}

static bool ExecuteKillShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    // Kill Shot requires target below 20% health
    if (target->GetHealthPct() > 20)
        return false;
    if (!botAI->CanCastSpell("kill shot", target))
        return false;
    return botAI->CastSpell("kill shot", target);
}

static bool ExecuteChimeraShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("chimera shot", target))
        return false;
    return botAI->CastSpell("chimera shot", target);
}

static bool ExecuteSilencingShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("silencing shot", target))
        return false;
    return botAI->CastSpell("silencing shot", target);
}

static bool ExecuteScatterShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("scatter shot", target))
        return false;
    return botAI->CastSpell("scatter shot", target);
}

// ============================================================================
// SHOTS - SURVIVAL
// ============================================================================

static bool ExecuteExplosiveShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("explosive shot", target))
        return false;
    return botAI->CastSpell("explosive shot", target);
}

static bool ExecuteBlackArrow(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("black arrow", target))
        return false;
    return botAI->CastSpell("black arrow", target);
}

static bool ExecuteWyvernSting(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("wyvern sting", target))
        return false;
    return botAI->CastSpell("wyvern sting", target);
}

// ============================================================================
// STINGS
// ============================================================================

static bool ExecuteSerpentSting(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("serpent sting", target))
        return false;
    return botAI->CastSpell("serpent sting", target);
}

static bool ExecuteScorpidSting(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("scorpid sting", target))
        return false;
    return botAI->CastSpell("scorpid sting", target);
}

static bool ExecuteViperSting(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("viper sting", target))
        return false;
    return botAI->CastSpell("viper sting", target);
}

// ============================================================================
// TRAPS
// ============================================================================

static bool ExecuteFrostTrap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("frost trap", bot))
        return false;
    return botAI->CastSpell("frost trap", bot);
}

static bool ExecuteFreezingTrap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("freezing trap", bot))
        return false;
    return botAI->CastSpell("freezing trap", bot);
}

static bool ExecuteExplosiveTrap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("explosive trap", bot))
        return false;
    return botAI->CastSpell("explosive trap", bot);
}

static bool ExecuteImmolationTrap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("immolation trap", bot))
        return false;
    return botAI->CastSpell("immolation trap", bot);
}

static bool ExecuteSnakeTrap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("snake trap", bot))
        return false;
    return botAI->CastSpell("snake trap", bot);
}

// ============================================================================
// UTILITY
// ============================================================================

static bool ExecuteHuntersMark(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("hunter's mark", target))
        return false;
    return botAI->CastSpell("hunter's mark", target);
}

static bool ExecuteTranquilizingShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("tranquilizing shot", target))
        return false;
    return botAI->CastSpell("tranquilizing shot", target);
}

static bool ExecuteDistractingShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("distracting shot", target))
        return false;
    return botAI->CastSpell("distracting shot", target);
}

static bool ExecuteConcussiveShot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("concussive shot", target))
        return false;
    return botAI->CastSpell("concussive shot", target);
}

static bool ExecuteFeignDeath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("feign death", bot))
        return false;
    return botAI->CastSpell("feign death", bot);
}

static bool ExecuteDeterrence(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("deterrence", bot))
        return false;
    return botAI->CastSpell("deterrence", bot);
}

static bool ExecuteDisengage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("disengage", bot))
        return false;
    return botAI->CastSpell("disengage", bot);
}

static bool ExecuteMasterCall(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("master's call", bot))
        return false;
    return botAI->CastSpell("master's call", bot);
}

static bool ExecuteMisdirection(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Target the tank or master for misdirection
    Player* master = botAI->GetMaster();
    if (!master)
        return false;
    if (!botAI->CanCastSpell("misdirection", master))
        return false;
    return botAI->CastSpell("misdirection", master);
}

static bool ExecuteFlare(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("flare", target))
        return false;
    return botAI->CastSpell("flare", target);
}

static bool ExecuteRaptorStrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("raptor strike", target))
        return false;
    return botAI->CastSpell("raptor strike", target);
}

static bool ExecuteMongooseBite(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mongoose bite", target))
        return false;
    return botAI->CastSpell("mongoose bite", target);
}

static bool ExecuteWingClip(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("wing clip", target))
        return false;
    return botAI->CastSpell("wing clip", target);
}

static bool ExecuteCounterAttack(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("counterattack", target))
        return false;
    return botAI->CastSpell("counterattack", target);
}

// ============================================================================
// COOLDOWNS
// ============================================================================

static bool ExecuteRapidFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("rapid fire", bot))
        return false;
    return botAI->CastSpell("rapid fire", bot);
}

static bool ExecuteReadiness(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("readiness", bot))
        return false;
    return botAI->CastSpell("readiness", bot);
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterHunterActions(FSMEngine* engine)
{
    // Basic actions
    engine->RegisterAction({HunterActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({HunterActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({HunterActions::AutoShot, "auto shot", ExecuteAutoShot});

    // Aspects
    engine->RegisterAction({HunterActions::AspectOfTheHawk, "aspect of the hawk", ExecuteAspectOfTheHawk});
    engine->RegisterAction({HunterActions::AspectOfTheDragonhawk, "aspect of the dragonhawk", ExecuteAspectOfTheDragonhawk});
    engine->RegisterAction({HunterActions::AspectOfTheViper, "aspect of the viper", ExecuteAspectOfTheViper});
    engine->RegisterAction({HunterActions::AspectOfTheWild, "aspect of the wild", ExecuteAspectOfTheWild});
    engine->RegisterAction({HunterActions::AspectOfThePack, "aspect of the pack", ExecuteAspectOfThePack});
    engine->RegisterAction({HunterActions::AspectOfTheBeast, "aspect of the beast", ExecuteAspectOfTheBeast});
    engine->RegisterAction({HunterActions::AspectOfTheMonkey, "aspect of the monkey", ExecuteAspectOfTheMonkey});
    engine->RegisterAction({HunterActions::AspectOfTheCheetah, "aspect of the cheetah", ExecuteAspectOfTheCheetah});

    // Pet management
    engine->RegisterAction({HunterActions::CallPet, "call pet", ExecuteCallPet});
    engine->RegisterAction({HunterActions::RevivePet, "revive pet", ExecuteRevivePet});
    engine->RegisterAction({HunterActions::MendPet, "mend pet", ExecuteMendPet});
    engine->RegisterAction({HunterActions::FeedPet, "feed pet", ExecuteFeedPet});
    engine->RegisterAction({HunterActions::DismissPet, "dismiss pet", ExecuteDismissPet});
    engine->RegisterAction({HunterActions::KillCommand, "kill command", ExecuteKillCommand});
    engine->RegisterAction({HunterActions::BestialWrath, "bestial wrath", ExecuteBestialWrath});
    engine->RegisterAction({HunterActions::Intimidation, "intimidation", ExecuteIntimidation});

    // Shots - Marksmanship
    engine->RegisterAction({HunterActions::ArcaneShot, "arcane shot", ExecuteArcaneShot});
    engine->RegisterAction({HunterActions::SteadyShot, "steady shot", ExecuteSteadyShot});
    engine->RegisterAction({HunterActions::AimedShot, "aimed shot", ExecuteAimedShot});
    engine->RegisterAction({HunterActions::MultiShot, "multi-shot", ExecuteMultiShot});
    engine->RegisterAction({HunterActions::KillShot, "kill shot", ExecuteKillShot});
    engine->RegisterAction({HunterActions::ChimeraShot, "chimera shot", ExecuteChimeraShot});
    engine->RegisterAction({HunterActions::SilencingShot, "silencing shot", ExecuteSilencingShot});
    engine->RegisterAction({HunterActions::ScatterShot, "scatter shot", ExecuteScatterShot});

    // Shots - Survival
    engine->RegisterAction({HunterActions::ExplosiveShot, "explosive shot", ExecuteExplosiveShot});
    engine->RegisterAction({HunterActions::BlackArrow, "black arrow", ExecuteBlackArrow});
    engine->RegisterAction({HunterActions::WyvernSting, "wyvern sting", ExecuteWyvernSting});

    // Stings
    engine->RegisterAction({HunterActions::SerpentSting, "serpent sting", ExecuteSerpentSting});
    engine->RegisterAction({HunterActions::ScorpidSting, "scorpid sting", ExecuteScorpidSting});
    engine->RegisterAction({HunterActions::ViperSting, "viper sting", ExecuteViperSting});

    // Traps
    engine->RegisterAction({HunterActions::FrostTrap, "frost trap", ExecuteFrostTrap});
    engine->RegisterAction({HunterActions::FreezingTrap, "freezing trap", ExecuteFreezingTrap});
    engine->RegisterAction({HunterActions::ExplosiveTrap, "explosive trap", ExecuteExplosiveTrap});
    engine->RegisterAction({HunterActions::ImmolationTrap, "immolation trap", ExecuteImmolationTrap});
    engine->RegisterAction({HunterActions::SnakeTrap, "snake trap", ExecuteSnakeTrap});

    // Utility
    engine->RegisterAction({HunterActions::HuntersMark, "hunter's mark", ExecuteHuntersMark});
    engine->RegisterAction({HunterActions::TranquilizingShot, "tranquilizing shot", ExecuteTranquilizingShot});
    engine->RegisterAction({HunterActions::DistractingShot, "distracting shot", ExecuteDistractingShot});
    engine->RegisterAction({HunterActions::ConcussiveShot, "concussive shot", ExecuteConcussiveShot});
    engine->RegisterAction({HunterActions::FeignDeath, "feign death", ExecuteFeignDeath});
    engine->RegisterAction({HunterActions::Deterrence, "deterrence", ExecuteDeterrence});
    engine->RegisterAction({HunterActions::Disengage, "disengage", ExecuteDisengage});
    engine->RegisterAction({HunterActions::MasterCall, "master's call", ExecuteMasterCall});
    engine->RegisterAction({HunterActions::Misdirection, "misdirection", ExecuteMisdirection});
    engine->RegisterAction({HunterActions::Flare, "flare", ExecuteFlare});
    engine->RegisterAction({HunterActions::RaptorStrike, "raptor strike", ExecuteRaptorStrike});
    engine->RegisterAction({HunterActions::MongooseBite, "mongoose bite", ExecuteMongooseBite});
    engine->RegisterAction({HunterActions::WingClip, "wing clip", ExecuteWingClip});
    engine->RegisterAction({HunterActions::CounterAttack, "counterattack", ExecuteCounterAttack});

    // Cooldowns
    engine->RegisterAction({HunterActions::RapidFire, "rapid fire", ExecuteRapidFire});
    engine->RegisterAction({HunterActions::Readiness, "readiness", ExecuteReadiness});
}
