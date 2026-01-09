/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "../FSMEngine.h"
#include "../BotStateMask.h"
#include "WarriorFuryTable.h"
#include "PlayerbotAI.h"
#include "Playerbots.h"
#include "Player.h"
#include "SpellMgr.h"
#include "SpellInfo.h"

// ============================================================================
// SPELL IDS (WotLK 3.3.5a)
// ============================================================================

namespace WarriorSpells
{
    // Stances
    constexpr uint32_t BattleStance      = 2457;
    constexpr uint32_t DefensiveStance   = 71;
    constexpr uint32_t BerserkerStance   = 2458;

    // Arms
    constexpr uint32_t Charge            = 11578;  // Rank 3
    constexpr uint32_t MortalStrike      = 47486;  // Rank 8
    constexpr uint32_t Overpower         = 7384;   // Rank 4
    constexpr uint32_t Rend              = 47465;  // Rank 10
    constexpr uint32_t Slam              = 47475;  // Rank 8
    constexpr uint32_t Execute           = 47471;  // Rank 9
    constexpr uint32_t Bladestorm        = 46924;

    // Fury
    constexpr uint32_t Bloodthirst       = 23881;  // Base spell
    constexpr uint32_t Whirlwind         = 1680;
    constexpr uint32_t RagingBlow        = 12950;
    constexpr uint32_t HeroicStrike      = 47450;  // Rank 13
    constexpr uint32_t Cleave            = 47520;  // Rank 8
    constexpr uint32_t Rampage           = 29801;

    // Protection
    constexpr uint32_t ShieldSlam        = 47488;  // Rank 8
    constexpr uint32_t Revenge           = 57823;  // Rank 9
    constexpr uint32_t Devastate         = 47498;  // Rank 5
    constexpr uint32_t ShieldBlock       = 2565;
    constexpr uint32_t Shockwave         = 46968;
    constexpr uint32_t ConcussionBlow    = 12809;

    // Shouts
    constexpr uint32_t BattleShout       = 47436;  // Rank 9
    constexpr uint32_t CommandingShout   = 47440;  // Rank 3
    constexpr uint32_t DemoralizingShout = 47437;  // Rank 8

    // Defensive
    constexpr uint32_t ShieldWall        = 871;
    constexpr uint32_t LastStand         = 12975;
    constexpr uint32_t EnragedRegeneration = 55694;

    // Interrupts
    constexpr uint32_t Pummel            = 6552;
    constexpr uint32_t ShieldBash        = 72;
    constexpr uint32_t HeroicThrow       = 57755;

    // Utility
    constexpr uint32_t Taunt             = 355;
    constexpr uint32_t MockingBlow       = 694;
    constexpr uint32_t Intervene         = 3411;
    constexpr uint32_t Intercept         = 20252;
    constexpr uint32_t Hamstring         = 7373;   // Rank 4 (or 1715)
    constexpr uint32_t SunderArmor       = 47467;  // Rank 7
    constexpr uint32_t ThunderClap       = 47502;  // Rank 9
    constexpr uint32_t VictoryRush       = 34428;
    constexpr uint32_t Berserker         = 18499;  // Berserker Rage
    constexpr uint32_t Recklessness      = 1719;
    constexpr uint32_t DeathWish         = 12328;
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

static bool CastSpell(PlayerbotAI* botAI, uint32_t spellId, Unit* target = nullptr)
{
    if (!botAI || !botAI->GetBot())
        return false;

    // Use the target if provided, otherwise use current target from cache
    if (!target)
        target = GET_CURRENT_TARGET();

    // Check if we can cast (mana/rage, cooldown, range)
    if (!botAI->CanCastSpell(spellId, target))
        return false;

    // Cast the spell
    return botAI->CastSpell(spellId, target);
}

static bool CastSpellSelf(PlayerbotAI* botAI, uint32_t spellId)
{
    return CastSpell(botAI, spellId, botAI->GetBot());
}

// ============================================================================
// ACTION EXECUTOR IMPLEMENTATIONS
// ============================================================================

namespace WarriorExecutors
{
    // ------------------------------------------------------------------------
    // GENERIC ACTIONS
    // ------------------------------------------------------------------------

    bool ExecuteMelee(PlayerbotAI* botAI, const BotStateVector& state)
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

    bool ExecuteFollow(PlayerbotAI* botAI, const BotStateVector& state)
    {
        // TODO: Implement using MovementAction infrastructure
        // For now, return false - existing engine will handle follow
        (void)botAI; (void)state;  // Suppress unused warnings
        return false;
    }

    bool ExecuteReachMelee(PlayerbotAI* botAI, const BotStateVector& state)
    {
        // TODO: Implement using MovementAction infrastructure
        // For now, return false - existing engine will handle movement
        (void)botAI; (void)state;  // Suppress unused warnings
        return false;
    }

    // ------------------------------------------------------------------------
    // STANCES
    // ------------------------------------------------------------------------

    bool ExecuteBattleStance(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::BattleStance);
    }

    bool ExecuteDefensiveStance(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::DefensiveStance);
    }

    bool ExecuteBerserkerStance(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::BerserkerStance);
    }

    // ------------------------------------------------------------------------
    // MOVEMENT ABILITIES
    // ------------------------------------------------------------------------

    bool ExecuteCharge(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Charge);
    }

    bool ExecuteIntercept(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Intercept);
    }

    // ------------------------------------------------------------------------
    // FURY ABILITIES
    // ------------------------------------------------------------------------

    bool ExecuteBloodthirst(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Bloodthirst);
    }

    bool ExecuteWhirlwind(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Whirlwind);
    }

    bool ExecuteHeroicStrike(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::HeroicStrike);
    }

    bool ExecuteCleave(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Cleave);
    }

    bool ExecuteRampage(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Rampage);
    }

    // ------------------------------------------------------------------------
    // ARMS ABILITIES
    // ------------------------------------------------------------------------

    bool ExecuteMortalStrike(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::MortalStrike);
    }

    bool ExecuteOverpower(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Overpower);
    }

    bool ExecuteRend(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Rend);
    }

    bool ExecuteSlam(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Slam);
    }

    bool ExecuteExecute(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Execute);
    }

    bool ExecuteBladestorm(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Bladestorm);
    }

    // ------------------------------------------------------------------------
    // PROTECTION ABILITIES
    // ------------------------------------------------------------------------

    bool ExecuteShieldSlam(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::ShieldSlam);
    }

    bool ExecuteRevenge(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Revenge);
    }

    bool ExecuteDevastate(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Devastate);
    }

    bool ExecuteShieldBlock(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::ShieldBlock);
    }

    bool ExecuteShockwave(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Shockwave);
    }

    bool ExecuteConcussionBlow(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::ConcussionBlow);
    }

    // ------------------------------------------------------------------------
    // SHOUTS
    // ------------------------------------------------------------------------

    bool ExecuteBattleShout(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::BattleShout);
    }

    bool ExecuteCommandingShout(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::CommandingShout);
    }

    bool ExecuteDemoralizingShout(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::DemoralizingShout);
    }

    // ------------------------------------------------------------------------
    // DEFENSIVE COOLDOWNS
    // ------------------------------------------------------------------------

    bool ExecuteShieldWall(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::ShieldWall);
    }

    bool ExecuteLastStand(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::LastStand);
    }

    bool ExecuteEnragedRegeneration(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::EnragedRegeneration);
    }

    // ------------------------------------------------------------------------
    // INTERRUPTS
    // ------------------------------------------------------------------------

    bool ExecutePummel(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Pummel);
    }

    bool ExecuteShieldBash(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::ShieldBash);
    }

    bool ExecuteHeroicThrow(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::HeroicThrow);
    }

    // ------------------------------------------------------------------------
    // UTILITY
    // ------------------------------------------------------------------------

    bool ExecuteTaunt(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Taunt);
    }

    bool ExecuteMockingBlow(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::MockingBlow);
    }

    bool ExecuteIntervene(PlayerbotAI* ai, const BotStateVector& state)
    {
        // TODO: Need to select intervene target (party member)
        return CastSpell(ai, WarriorSpells::Intervene);
    }

    bool ExecuteHamstring(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::Hamstring);
    }

    bool ExecuteSunderArmor(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::SunderArmor);
    }

    bool ExecuteThunderClap(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::ThunderClap);
    }

    bool ExecuteVictoryRush(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpell(ai, WarriorSpells::VictoryRush);
    }

    bool ExecuteBerserkerRage(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::Berserker);
    }

    bool ExecuteRecklessness(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::Recklessness);
    }

    bool ExecuteDeathWish(PlayerbotAI* ai, const BotStateVector& state)
    {
        return CastSpellSelf(ai, WarriorSpells::DeathWish);
    }

} // namespace WarriorExecutors

// ============================================================================
// REGISTRATION FUNCTION
// ============================================================================

void RegisterWarriorActions(FSMEngine* engine)
{
    using namespace WarriorActions;
    using namespace WarriorExecutors;

    // Generic
    engine->RegisterAction({Melee, "melee", ExecuteMelee});
    engine->RegisterAction({Follow, "follow", ExecuteFollow});
    engine->RegisterAction({ReachMelee, "reach melee", ExecuteReachMelee});

    // Stances
    engine->RegisterAction({BattleStance, "battle stance", ExecuteBattleStance, nullptr, nullptr, WarriorSpells::BattleStance});
    engine->RegisterAction({DefensiveStance, "defensive stance", ExecuteDefensiveStance, nullptr, nullptr, WarriorSpells::DefensiveStance});
    engine->RegisterAction({BerserkerStance, "berserker stance", ExecuteBerserkerStance, nullptr, nullptr, WarriorSpells::BerserkerStance});

    // Movement
    engine->RegisterAction({Charge, "charge", ExecuteCharge, nullptr, nullptr, WarriorSpells::Charge});
    engine->RegisterAction({Intercept, "intercept", ExecuteIntercept, nullptr, nullptr, WarriorSpells::Intercept});

    // Fury
    engine->RegisterAction({Bloodthirst, "bloodthirst", ExecuteBloodthirst, nullptr, nullptr, WarriorSpells::Bloodthirst});
    engine->RegisterAction({WhirlwindFury, "whirlwind", ExecuteWhirlwind, nullptr, nullptr, WarriorSpells::Whirlwind});
    engine->RegisterAction({HeroicStrike, "heroic strike", ExecuteHeroicStrike, nullptr, nullptr, WarriorSpells::HeroicStrike});
    engine->RegisterAction({Cleave, "cleave", ExecuteCleave, nullptr, nullptr, WarriorSpells::Cleave});
    engine->RegisterAction({Rampage, "rampage", ExecuteRampage, nullptr, nullptr, WarriorSpells::Rampage});

    // Arms
    engine->RegisterAction({MortalStrike, "mortal strike", ExecuteMortalStrike, nullptr, nullptr, WarriorSpells::MortalStrike});
    engine->RegisterAction({Overpower, "overpower", ExecuteOverpower, nullptr, nullptr, WarriorSpells::Overpower});
    engine->RegisterAction({Rend, "rend", ExecuteRend, nullptr, nullptr, WarriorSpells::Rend});
    engine->RegisterAction({Slam, "slam", ExecuteSlam, nullptr, nullptr, WarriorSpells::Slam});
    engine->RegisterAction({Execute, "execute", ExecuteExecute, nullptr, nullptr, WarriorSpells::Execute});
    engine->RegisterAction({Bladestorm, "bladestorm", ExecuteBladestorm, nullptr, nullptr, WarriorSpells::Bladestorm});

    // Protection
    engine->RegisterAction({ShieldSlam, "shield slam", ExecuteShieldSlam, nullptr, nullptr, WarriorSpells::ShieldSlam});
    engine->RegisterAction({Revenge, "revenge", ExecuteRevenge, nullptr, nullptr, WarriorSpells::Revenge});
    engine->RegisterAction({Devastate, "devastate", ExecuteDevastate, nullptr, nullptr, WarriorSpells::Devastate});
    engine->RegisterAction({ShieldBlock, "shield block", ExecuteShieldBlock, nullptr, nullptr, WarriorSpells::ShieldBlock});
    engine->RegisterAction({Shockwave, "shockwave", ExecuteShockwave, nullptr, nullptr, WarriorSpells::Shockwave});
    engine->RegisterAction({ConcussionBlow, "concussion blow", ExecuteConcussionBlow, nullptr, nullptr, WarriorSpells::ConcussionBlow});

    // Shouts
    engine->RegisterAction({BattleShout, "battle shout", ExecuteBattleShout, nullptr, nullptr, WarriorSpells::BattleShout});
    engine->RegisterAction({CommandingShout, "commanding shout", ExecuteCommandingShout, nullptr, nullptr, WarriorSpells::CommandingShout});
    engine->RegisterAction({DemoralizingShout, "demoralizing shout", ExecuteDemoralizingShout, nullptr, nullptr, WarriorSpells::DemoralizingShout});

    // Defensive
    engine->RegisterAction({ShieldWall, "shield wall", ExecuteShieldWall, nullptr, nullptr, WarriorSpells::ShieldWall});
    engine->RegisterAction({LastStand, "last stand", ExecuteLastStand, nullptr, nullptr, WarriorSpells::LastStand});
    engine->RegisterAction({EnragedRegeneration, "enraged regeneration", ExecuteEnragedRegeneration, nullptr, nullptr, WarriorSpells::EnragedRegeneration});

    // Interrupts
    engine->RegisterAction({Pummel, "pummel", ExecutePummel, nullptr, nullptr, WarriorSpells::Pummel});
    engine->RegisterAction({ShieldBash, "shield bash", ExecuteShieldBash, nullptr, nullptr, WarriorSpells::ShieldBash});
    engine->RegisterAction({HeroicThrow, "heroic throw", ExecuteHeroicThrow, nullptr, nullptr, WarriorSpells::HeroicThrow});

    // Utility
    engine->RegisterAction({Taunt, "taunt", ExecuteTaunt, nullptr, nullptr, WarriorSpells::Taunt});
    engine->RegisterAction({MockingBlow, "mocking blow", ExecuteMockingBlow, nullptr, nullptr, WarriorSpells::MockingBlow});
    engine->RegisterAction({Intervene, "intervene", ExecuteIntervene, nullptr, nullptr, WarriorSpells::Intervene});
    engine->RegisterAction({Hamstring, "hamstring", ExecuteHamstring, nullptr, nullptr, WarriorSpells::Hamstring});
    engine->RegisterAction({SunderArmor, "sunder armor", ExecuteSunderArmor, nullptr, nullptr, WarriorSpells::SunderArmor});
    engine->RegisterAction({ThunderClap, "thunder clap", ExecuteThunderClap, nullptr, nullptr, WarriorSpells::ThunderClap});
    engine->RegisterAction({VictoryRush, "victory rush", ExecuteVictoryRush, nullptr, nullptr, WarriorSpells::VictoryRush});
    engine->RegisterAction({Berserker, "berserker rage", ExecuteBerserkerRage, nullptr, nullptr, WarriorSpells::Berserker});
    engine->RegisterAction({Recklessness, "recklessness", ExecuteRecklessness, nullptr, nullptr, WarriorSpells::Recklessness});
    engine->RegisterAction({DeathWish, "death wish", ExecuteDeathWish, nullptr, nullptr, WarriorSpells::DeathWish});

    // Add the Fury strategy table
    engine->AddStrategy(&WarriorFuryStrategy::table);
}
