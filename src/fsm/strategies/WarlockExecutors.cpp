/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "WarlockExecutors.h"
#include "../FSMEngine.h"
#include "../ActionTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"

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

    bot->Attack(target, true);
    return true;
}

static bool ExecuteFollow(PlayerbotAI* botAI, const BotStateVector& state)
{
    (void)botAI; (void)state;
    return false;
}

static bool ExecuteShoot(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shoot", target))
        return false;
    return botAI->CastSpell("shoot", target);
}

// ============================================================================
// ARMORS
// ============================================================================

static bool ExecuteDemonArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("demon armor", bot))
        return false;
    return botAI->CastSpell("demon armor", bot);
}

static bool ExecuteFelArmor(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("fel armor", bot))
        return false;
    return botAI->CastSpell("fel armor", bot);
}

static bool ExecuteDemonSkin(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("demon skin", bot))
        return false;
    return botAI->CastSpell("demon skin", bot);
}

// ============================================================================
// PET MANAGEMENT
// ============================================================================

static bool ExecuteSummonImp(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("summon imp", bot))
        return false;
    return botAI->CastSpell("summon imp", bot);
}

static bool ExecuteSummonVoidwalker(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("summon voidwalker", bot))
        return false;
    return botAI->CastSpell("summon voidwalker", bot);
}

static bool ExecuteSummonSuccubus(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("summon succubus", bot))
        return false;
    return botAI->CastSpell("summon succubus", bot);
}

static bool ExecuteSummonFelhunter(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("summon felhunter", bot))
        return false;
    return botAI->CastSpell("summon felhunter", bot);
}

static bool ExecuteSummonFelguard(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("summon felguard", bot))
        return false;
    return botAI->CastSpell("summon felguard", bot);
}

static bool ExecuteDemonicEmpowerment(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("demonic empowerment", bot))
        return false;
    return botAI->CastSpell("demonic empowerment", bot);
}

static bool ExecuteHealthFunnel(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    Pet* pet = bot->GetPet();
    if (!pet)
        return false;
    if (!botAI->CanCastSpell("health funnel", pet))
        return false;
    return botAI->CastSpell("health funnel", pet);
}

// ============================================================================
// AFFLICTION DOTS
// ============================================================================

static bool ExecuteCorruption(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("corruption", target))
        return false;
    return botAI->CastSpell("corruption", target);
}

static bool ExecuteCurseOfAgony(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("curse of agony", target))
        return false;
    return botAI->CastSpell("curse of agony", target);
}

static bool ExecuteCurseOfDoom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("curse of doom", target))
        return false;
    return botAI->CastSpell("curse of doom", target);
}

static bool ExecuteCurseOfElements(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("curse of the elements", target))
        return false;
    return botAI->CastSpell("curse of the elements", target);
}

static bool ExecuteCurseOfTongues(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("curse of tongues", target))
        return false;
    return botAI->CastSpell("curse of tongues", target);
}

static bool ExecuteCurseOfWeakness(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("curse of weakness", target))
        return false;
    return botAI->CastSpell("curse of weakness", target);
}

static bool ExecuteUnstableAffliction(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("unstable affliction", target))
        return false;
    return botAI->CastSpell("unstable affliction", target);
}

static bool ExecuteHaunt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("haunt", target))
        return false;
    return botAI->CastSpell("haunt", target);
}

// ============================================================================
// DEMONOLOGY
// ============================================================================

static bool ExecuteMetamorphosis(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("metamorphosis", bot))
        return false;
    return botAI->CastSpell("metamorphosis", bot);
}

static bool ExecuteImmolationAura(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("immolation aura", bot))
        return false;
    return botAI->CastSpell("immolation aura", bot);
}

static bool ExecuteShadowCleave(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadow cleave", target))
        return false;
    return botAI->CastSpell("shadow cleave", target);
}

static bool ExecuteDemonicCharge(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("demonic charge", target))
        return false;
    return botAI->CastSpell("demonic charge", target);
}

// ============================================================================
// DESTRUCTION
// ============================================================================

static bool ExecuteShadowBolt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadow bolt", target))
        return false;
    return botAI->CastSpell("shadow bolt", target);
}

static bool ExecuteIncinerate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("incinerate", target))
        return false;
    return botAI->CastSpell("incinerate", target);
}

static bool ExecuteImmolate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("immolate", target))
        return false;
    return botAI->CastSpell("immolate", target);
}

static bool ExecuteConflagrate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("conflagrate", target))
        return false;
    return botAI->CastSpell("conflagrate", target);
}

static bool ExecuteChaosBolt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("chaos bolt", target))
        return false;
    return botAI->CastSpell("chaos bolt", target);
}

static bool ExecuteSoulFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("soul fire", target))
        return false;
    return botAI->CastSpell("soul fire", target);
}

static bool ExecuteShadowburn(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadowburn", target))
        return false;
    return botAI->CastSpell("shadowburn", target);
}

static bool ExecuteSearingPain(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("searing pain", target))
        return false;
    return botAI->CastSpell("searing pain", target);
}

// ============================================================================
// AOE
// ============================================================================

static bool ExecuteRainOfFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("rain of fire", target))
        return false;
    return botAI->CastSpell("rain of fire", target);
}

static bool ExecuteSeedOfCorruption(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("seed of corruption", target))
        return false;
    return botAI->CastSpell("seed of corruption", target);
}

static bool ExecuteHellfire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("hellfire", bot))
        return false;
    return botAI->CastSpell("hellfire", bot);
}

static bool ExecuteShadowflame(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadowflame", target))
        return false;
    return botAI->CastSpell("shadowflame", target);
}

// ============================================================================
// LIFE TAP / MANA
// ============================================================================

static bool ExecuteLifeTap(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("life tap", bot))
        return false;
    return botAI->CastSpell("life tap", bot);
}

static bool ExecuteDarkPact(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("dark pact", bot))
        return false;
    return botAI->CastSpell("dark pact", bot);
}

static bool ExecuteDrainMana(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("drain mana", target))
        return false;
    return botAI->CastSpell("drain mana", target);
}

static bool ExecuteDrainLife(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("drain life", target))
        return false;
    return botAI->CastSpell("drain life", target);
}

static bool ExecuteDrainSoul(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("drain soul", target))
        return false;
    return botAI->CastSpell("drain soul", target);
}

// ============================================================================
// UTILITY
// ============================================================================

static bool ExecuteFear(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("fear", target))
        return false;
    return botAI->CastSpell("fear", target);
}

static bool ExecuteHowlOfTerror(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("howl of terror", bot))
        return false;
    return botAI->CastSpell("howl of terror", bot);
}

static bool ExecuteDeathCoil(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("death coil", target))
        return false;
    return botAI->CastSpell("death coil", target);
}

static bool ExecuteShadowfury(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadowfury", target))
        return false;
    return botAI->CastSpell("shadowfury", target);
}

static bool ExecuteBanish(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("banish", target))
        return false;
    return botAI->CastSpell("banish", target);
}

static bool ExecuteCreateHealthstone(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("create healthstone", bot))
        return false;
    return botAI->CastSpell("create healthstone", bot);
}

static bool ExecuteCreateSoulstone(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("create soulstone", bot))
        return false;
    return botAI->CastSpell("create soulstone", bot);
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterWarlockActions(FSMEngine* engine)
{
    // Basic actions
    engine->RegisterAction({WarlockActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({WarlockActions::Follow, "follow", ExecuteFollow});
    engine->RegisterAction({WarlockActions::Shoot, "shoot", ExecuteShoot});

    // Armors
    engine->RegisterAction({WarlockActions::DemonArmor, "demon armor", ExecuteDemonArmor});
    engine->RegisterAction({WarlockActions::FelArmor, "fel armor", ExecuteFelArmor});
    engine->RegisterAction({WarlockActions::DemonSkin, "demon skin", ExecuteDemonSkin});

    // Pet management
    engine->RegisterAction({WarlockActions::SummonImp, "summon imp", ExecuteSummonImp});
    engine->RegisterAction({WarlockActions::SummonVoidwalker, "summon voidwalker", ExecuteSummonVoidwalker});
    engine->RegisterAction({WarlockActions::SummonSuccubus, "summon succubus", ExecuteSummonSuccubus});
    engine->RegisterAction({WarlockActions::SummonFelhunter, "summon felhunter", ExecuteSummonFelhunter});
    engine->RegisterAction({WarlockActions::SummonFelguard, "summon felguard", ExecuteSummonFelguard});
    engine->RegisterAction({WarlockActions::DemonicEmpowerment, "demonic empowerment", ExecuteDemonicEmpowerment});
    engine->RegisterAction({WarlockActions::HealthFunnel, "health funnel", ExecuteHealthFunnel});

    // Affliction DoTs
    engine->RegisterAction({WarlockActions::Corruption, "corruption", ExecuteCorruption});
    engine->RegisterAction({WarlockActions::CurseOfAgony, "curse of agony", ExecuteCurseOfAgony});
    engine->RegisterAction({WarlockActions::CurseOfDoom, "curse of doom", ExecuteCurseOfDoom});
    engine->RegisterAction({WarlockActions::CurseOfElements, "curse of the elements", ExecuteCurseOfElements});
    engine->RegisterAction({WarlockActions::CurseOfTongues, "curse of tongues", ExecuteCurseOfTongues});
    engine->RegisterAction({WarlockActions::CurseOfWeakness, "curse of weakness", ExecuteCurseOfWeakness});
    engine->RegisterAction({WarlockActions::UnstableAffliction, "unstable affliction", ExecuteUnstableAffliction});
    engine->RegisterAction({WarlockActions::Haunt, "haunt", ExecuteHaunt});

    // Demonology
    engine->RegisterAction({WarlockActions::Metamorphosis, "metamorphosis", ExecuteMetamorphosis});
    engine->RegisterAction({WarlockActions::ImmolationAura, "immolation aura", ExecuteImmolationAura});
    engine->RegisterAction({WarlockActions::ShadowCleave, "shadow cleave", ExecuteShadowCleave});
    engine->RegisterAction({WarlockActions::DemonicCharge, "demonic charge", ExecuteDemonicCharge});

    // Destruction
    engine->RegisterAction({WarlockActions::ShadowBolt, "shadow bolt", ExecuteShadowBolt});
    engine->RegisterAction({WarlockActions::Incinerate, "incinerate", ExecuteIncinerate});
    engine->RegisterAction({WarlockActions::Immolate, "immolate", ExecuteImmolate});
    engine->RegisterAction({WarlockActions::Conflagrate, "conflagrate", ExecuteConflagrate});
    engine->RegisterAction({WarlockActions::ChaosBolt, "chaos bolt", ExecuteChaosBolt});
    engine->RegisterAction({WarlockActions::SoulFire, "soul fire", ExecuteSoulFire});
    engine->RegisterAction({WarlockActions::Shadowburn, "shadowburn", ExecuteShadowburn});
    engine->RegisterAction({WarlockActions::SearingPain, "searing pain", ExecuteSearingPain});

    // AoE
    engine->RegisterAction({WarlockActions::RainOfFire, "rain of fire", ExecuteRainOfFire});
    engine->RegisterAction({WarlockActions::SeedOfCorruption, "seed of corruption", ExecuteSeedOfCorruption});
    engine->RegisterAction({WarlockActions::Hellfire, "hellfire", ExecuteHellfire});
    engine->RegisterAction({WarlockActions::Shadowflame, "shadowflame", ExecuteShadowflame});

    // Life Tap / Mana
    engine->RegisterAction({WarlockActions::LifeTap, "life tap", ExecuteLifeTap});
    engine->RegisterAction({WarlockActions::DarkPact, "dark pact", ExecuteDarkPact});
    engine->RegisterAction({WarlockActions::DrainMana, "drain mana", ExecuteDrainMana});
    engine->RegisterAction({WarlockActions::DrainLife, "drain life", ExecuteDrainLife});
    engine->RegisterAction({WarlockActions::DrainSoul, "drain soul", ExecuteDrainSoul});

    // Utility
    engine->RegisterAction({WarlockActions::Fear, "fear", ExecuteFear});
    engine->RegisterAction({WarlockActions::HowlOfTerror, "howl of terror", ExecuteHowlOfTerror});
    engine->RegisterAction({WarlockActions::DeathCoil, "death coil", ExecuteDeathCoil});
    engine->RegisterAction({WarlockActions::Shadowfury, "shadowfury", ExecuteShadowfury});
    engine->RegisterAction({WarlockActions::Banish, "banish", ExecuteBanish});
    engine->RegisterAction({WarlockActions::CreateHealthstone, "create healthstone", ExecuteCreateHealthstone});
    engine->RegisterAction({WarlockActions::CreateSoulstone, "create soulstone", ExecuteCreateSoulstone});
}
