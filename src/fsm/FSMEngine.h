/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_FSMENGINE_H
#define _PLAYERBOT_FSMENGINE_H

#include "BotStateMask.h"
#include "ActionTable.h"
#include <vector>
#include <unordered_map>

class PlayerbotAI;
class Player;
class Unit;
class AiObjectContext;

// ============================================================================
// FSM ENGINE - Bitmask-based state machine for bot AI
// ============================================================================

/**
 * Finite State Machine engine that replaces the priority queue-based Engine.
 *
 * Key differences from old Engine:
 * - State is computed once per tick into a bitmask (256 bits)
 * - Action selection is a table lookup, not a queue scan
 * - No string-based lookups at runtime
 * - ~5x faster per tick
 */
class FSMEngine
{
public:
    FSMEngine(PlayerbotAI* ai);
    ~FSMEngine();

    /**
     * Main tick function - replaces Engine::DoNextAction()
     * 1. Updates state bitmask
     * 2. Selects best action from active strategies
     * 3. Executes the action
     * Returns true if an action was executed successfully.
     */
    bool DoNextAction(Unit* target = nullptr, int depth = 0, bool minimal = false);

    /**
     * Update state bitmask from current game state.
     * Called automatically by DoNextAction, but can be called manually
     * to inspect state without executing actions.
     */
    void UpdateState();

    /**
     * Get current state (read-only).
     */
    const BotStateVector& GetState() const { return currentState; }

    /**
     * Add an action table (strategy) to active strategies.
     */
    void AddStrategy(const ActionTable* table);

    /**
     * Remove an action table from active strategies.
     */
    void RemoveStrategy(uint16_t strategyId);

    /**
     * Check if a strategy is active.
     */
    bool HasStrategy(uint16_t strategyId) const;

    /**
     * Clear all active strategies.
     */
    void ClearStrategies();

    /**
     * Register an action executor.
     * Must be called for each action before it can be executed.
     */
    void RegisterAction(const ActionEntry& entry);

    /**
     * Get action entry by ID.
     */
    const ActionEntry* GetActionEntry(ActionId id) const;

    /**
     * Record action cooldown.
     */
    void RecordCooldown(ActionId id, uint32_t durationMs);

    /**
     * Check if action is on cooldown.
     */
    bool IsOnCooldown(ActionId id) const;

    /**
     * Force execute a specific action (for commands).
     */
    bool ExecuteAction(ActionId id);

    /**
     * Get last executed action ID.
     */
    ActionId GetLastAction() const { return lastAction; }

    /**
     * Get previous state (for detecting transitions).
     */
    const BotStateVector& GetPreviousState() const { return previousState; }

private:
    PlayerbotAI* botAI;
    Player* bot;
    AiObjectContext* context;

    // State tracking
    BotStateVector currentState;
    BotStateVector previousState;

    // Active strategy tables
    std::vector<const ActionTable*> activeStrategies;

    // Action registry (ActionId -> ActionEntry)
    std::unordered_map<ActionId, ActionEntry> actionRegistry;

    // Cooldown tracking (ActionId -> expiry time)
    std::unordered_map<ActionId, uint32_t> cooldowns;

    // Last executed action
    ActionId lastAction;
    uint32_t lastActionTime;
    uint32_t lastTimerTick;  // For per-second timer events

    // State update helpers
    void UpdatePrimaryState();
    void UpdateSecondaryState();
    void UpdateClassState();
    void UpdateEventState();

    // Numeric state calculations
    uint8_t CalculateHealthLevel(uint8_t healthPct) const;
    uint8_t CalculateManaLevel(uint8_t manaPct) const;
    uint8_t CalculateRangeBand(float distance) const;

    // Action selection
    ActionId SelectBestAction() const;

    // Action execution
    bool ExecuteActionInternal(ActionId id, const ActionDefinition* def);
};

// ============================================================================
// WARRIOR ACTION IDS (Proof of Concept)
// ============================================================================

namespace WarriorActions
{
    // Generic actions
    constexpr ActionId Melee                = 1;
    constexpr ActionId Shoot                = 2;
    constexpr ActionId Follow               = 3;
    constexpr ActionId ReachMelee           = 4;

    // Stance management
    constexpr ActionId BattleStance         = 10;
    constexpr ActionId DefensiveStance      = 11;
    constexpr ActionId BerserkerStance      = 12;

    // Arms abilities
    constexpr ActionId Charge               = 20;
    constexpr ActionId MortalStrike         = 21;
    constexpr ActionId Overpower            = 22;
    constexpr ActionId Rend                 = 23;
    constexpr ActionId Slam                 = 24;
    constexpr ActionId Execute              = 25;
    constexpr ActionId Bladestorm           = 26;

    // Fury abilities
    constexpr ActionId Bloodthirst          = 30;
    constexpr ActionId WhirlwindFury        = 31;
    constexpr ActionId RagingBlow           = 32;
    constexpr ActionId HeroicStrike         = 33;
    constexpr ActionId Cleave               = 34;
    constexpr ActionId Rampage              = 35;

    // Protection abilities
    constexpr ActionId ShieldSlam           = 40;
    constexpr ActionId Revenge              = 41;
    constexpr ActionId Devastate            = 42;
    constexpr ActionId ShieldBlock          = 43;
    constexpr ActionId Shockwave            = 44;
    constexpr ActionId ConcussionBlow       = 45;

    // Shouts
    constexpr ActionId BattleShout          = 50;
    constexpr ActionId CommandingShout      = 51;
    constexpr ActionId DemoralizingShout    = 52;

    // Defensive cooldowns
    constexpr ActionId ShieldWall           = 60;
    constexpr ActionId LastStand            = 61;
    constexpr ActionId EnragedRegeneration  = 62;

    // Interrupts
    constexpr ActionId Pummel               = 70;
    constexpr ActionId ShieldBash           = 71;
    constexpr ActionId HeroicThrow          = 72;

    // Utility
    constexpr ActionId Taunt                = 80;
    constexpr ActionId MockingBlow          = 81;
    constexpr ActionId Intervene            = 82;
    constexpr ActionId Intercept            = 83;
    constexpr ActionId Hamstring            = 84;
    constexpr ActionId SunderArmor          = 85;
    constexpr ActionId ThunderClap          = 86;
    constexpr ActionId VictoryRush          = 87;
    constexpr ActionId Berserker            = 88;
    constexpr ActionId Recklessness         = 89;
    constexpr ActionId DeathWish            = 90;

    // Additional Arms/Prot abilities
    constexpr ActionId SweepingStrikes      = 91;
    constexpr ActionId ChallengingShout     = 93;
    constexpr ActionId Disarm               = 94;
    constexpr ActionId SpellReflection      = 95;
}

// ============================================================================
// ROGUE ACTION IDS
// ============================================================================

namespace RogueActions
{
    // Generic actions (shared IDs with other classes for common actions)
    constexpr ActionId Melee                = 1;
    constexpr ActionId Shoot                = 2;
    constexpr ActionId Follow               = 3;
    constexpr ActionId ReachMelee           = 4;

    // Stealth management
    constexpr ActionId Stealth              = 100;
    constexpr ActionId Vanish               = 101;
    constexpr ActionId ShadowDance          = 102;

    // Openers (from stealth)
    constexpr ActionId Ambush               = 110;
    constexpr ActionId CheapShot            = 111;
    constexpr ActionId Garrote              = 112;
    constexpr ActionId Sap                  = 113;

    // Combo point builders
    constexpr ActionId SinisterStrike       = 120;
    constexpr ActionId Backstab             = 121;
    constexpr ActionId Hemorrhage           = 122;
    constexpr ActionId MutilateMainHand     = 123;
    constexpr ActionId Ghostly              = 124;  // Ghostly Strike
    constexpr ActionId FanOfKnives          = 125;

    // Finishers
    constexpr ActionId Eviscerate           = 130;
    constexpr ActionId Envenom              = 131;
    constexpr ActionId Rupture              = 132;
    constexpr ActionId KidneyShot           = 133;
    constexpr ActionId SliceAndDice         = 134;
    constexpr ActionId ExposeArmor          = 135;
    constexpr ActionId DeadlyThrow          = 136;

    // Utility
    constexpr ActionId Kick                 = 140;
    constexpr ActionId Gouge                = 141;
    constexpr ActionId Blind                = 142;
    constexpr ActionId Sprint               = 143;
    constexpr ActionId Evasion              = 144;
    constexpr ActionId CloakOfShadows       = 145;
    constexpr ActionId TricksOfTheTrade     = 146;
    constexpr ActionId Feint                = 147;
    constexpr ActionId Dismantle            = 148;

    // Cooldowns
    constexpr ActionId AdrenalineRush       = 150;
    constexpr ActionId BladeFlurry          = 151;
    constexpr ActionId KillingSpree         = 152;
    constexpr ActionId ColdBlood            = 153;
    constexpr ActionId Preparation          = 154;
    constexpr ActionId HungerForBlood       = 155;

    // Poisons (apply out of combat)
    constexpr ActionId ApplyMainHandPoison  = 160;
    constexpr ActionId ApplyOffHandPoison   = 161;
}

// ============================================================================
// PALADIN ACTION IDS
// ============================================================================

namespace PaladinActions
{
    // Generic actions (shared IDs with other classes for common actions)
    constexpr ActionId Melee                = 1;
    constexpr ActionId Shoot                = 2;
    constexpr ActionId Follow               = 3;
    constexpr ActionId ReachMelee           = 4;
    constexpr ActionId ReachSpell           = 5;

    // Auras (start at 200)
    constexpr ActionId DevotionAura         = 200;
    constexpr ActionId RetributionAura      = 201;
    constexpr ActionId ConcentrationAura    = 202;
    constexpr ActionId ShadowResistanceAura = 203;
    constexpr ActionId FrostResistanceAura  = 204;
    constexpr ActionId FireResistanceAura   = 205;
    constexpr ActionId CrusaderAura         = 206;

    // Seals
    constexpr ActionId SealOfRighteousness  = 210;
    constexpr ActionId SealOfCommand        = 211;
    constexpr ActionId SealOfVengeance      = 212;
    constexpr ActionId SealOfCorruption     = 213;
    constexpr ActionId SealOfLight          = 214;
    constexpr ActionId SealOfWisdom         = 215;
    constexpr ActionId SealOfJustice        = 216;

    // Judgments
    constexpr ActionId JudgementOfLight     = 220;
    constexpr ActionId JudgementOfWisdom    = 221;
    constexpr ActionId JudgementOfJustice   = 222;

    // Blessings
    constexpr ActionId BlessingOfMight      = 230;
    constexpr ActionId BlessingOfKings      = 231;
    constexpr ActionId BlessingOfWisdom     = 232;
    constexpr ActionId BlessingOfSanctuary  = 233;

    // Holy spells
    constexpr ActionId HolyLight            = 240;
    constexpr ActionId FlashOfLight         = 241;
    constexpr ActionId HolyShock            = 242;
    constexpr ActionId LayOnHands           = 243;
    constexpr ActionId Consecration         = 244;
    constexpr ActionId Exorcism             = 245;
    constexpr ActionId HolyWrath            = 246;
    constexpr ActionId BeaconOfLight        = 247;
    constexpr ActionId SacredShield         = 248;

    // Protection abilities
    constexpr ActionId RighteousFury        = 250;
    constexpr ActionId HammerOfTheRighteous = 251;
    constexpr ActionId ShieldOfRighteousness= 252;
    constexpr ActionId AvengersShield       = 253;
    constexpr ActionId HolyShield           = 254;
    constexpr ActionId RighteousDefense     = 255;
    constexpr ActionId HandOfReckoning      = 256;

    // Retribution abilities
    constexpr ActionId CrusaderStrike       = 260;
    constexpr ActionId DivineStorm          = 261;
    constexpr ActionId HammerOfWrath        = 262;
    constexpr ActionId Repentance           = 263;

    // Hands (utility)
    constexpr ActionId HandOfFreedom        = 270;
    constexpr ActionId HandOfProtection     = 271;
    constexpr ActionId HandOfSalvation      = 272;
    constexpr ActionId HandOfSacrifice      = 273;

    // Defensive cooldowns
    constexpr ActionId DivineProtection     = 280;
    constexpr ActionId DivineShield         = 281;
    constexpr ActionId AvengingWrath        = 282;

    // Utility
    constexpr ActionId HammerOfJustice      = 290;
    constexpr ActionId Cleanse              = 291;
    constexpr ActionId PurifyAction         = 292;
    constexpr ActionId TurnEvil             = 293;
    constexpr ActionId Redemption           = 294;
    constexpr ActionId DivinePlea           = 295;
    constexpr ActionId ArtOfWar             = 296;
}

// ============================================================================
// DEATH KNIGHT ACTION IDS
// ============================================================================

namespace DeathKnightActions
{
    // Generic actions (shared IDs with other classes for common actions)
    constexpr ActionId Melee                = 1;
    constexpr ActionId Shoot                = 2;
    constexpr ActionId Follow               = 3;
    constexpr ActionId ReachMelee           = 4;

    // Presences (start at 400)
    constexpr ActionId BloodPresence        = 400;
    constexpr ActionId FrostPresence        = 401;
    constexpr ActionId UnholyPresence       = 402;

    // Blood abilities
    constexpr ActionId HeartStrike          = 410;
    constexpr ActionId BloodStrike          = 411;
    constexpr ActionId DeathStrike          = 412;
    constexpr ActionId RuneStrike           = 413;
    constexpr ActionId BloodBoil            = 414;
    constexpr ActionId Pestilence           = 415;
    constexpr ActionId MarkOfBlood          = 416;
    constexpr ActionId VampiricBlood        = 417;
    constexpr ActionId RuneTap              = 418;
    constexpr ActionId DancingRuneWeapon    = 419;

    // Frost abilities
    constexpr ActionId IcyTouch             = 420;
    constexpr ActionId Obliterate           = 421;
    constexpr ActionId FrostStrike          = 422;
    constexpr ActionId HowlingBlast         = 423;
    constexpr ActionId UnbreakableArmor     = 424;
    constexpr ActionId Deathchill           = 425;
    constexpr ActionId FrostFever           = 426;

    // Unholy abilities
    constexpr ActionId PlagueStrike         = 430;
    constexpr ActionId ScourgeStrike        = 431;
    constexpr ActionId DeathCoil            = 432;
    constexpr ActionId DeathAndDecay        = 433;
    constexpr ActionId SummonGargoyle       = 434;
    constexpr ActionId UnholyFrenzy         = 435;
    constexpr ActionId BloodPlague          = 436;
    constexpr ActionId GhoulFrenzy          = 437;
    constexpr ActionId CorpseExplosion      = 438;

    // Utility
    constexpr ActionId DeathGrip            = 440;
    constexpr ActionId MindFreeze           = 441;
    constexpr ActionId Strangulate          = 442;
    constexpr ActionId ChainsOfIce          = 443;
    constexpr ActionId IceboundFortitude    = 444;
    constexpr ActionId AntiMagicShell       = 445;
    constexpr ActionId ArmyOfTheDead        = 446;
    constexpr ActionId EmpowerRuneWeapon    = 447;
    constexpr ActionId HornOfWinter         = 448;
    constexpr ActionId RaiseDead            = 449;
    constexpr ActionId DarkCommand          = 450;

    // Diseases
    constexpr ActionId ApplyFrostFever      = 460;
    constexpr ActionId ApplyBloodPlague     = 461;
}

// ============================================================================
// MAGE ACTION IDS (500-599)
// ============================================================================
namespace MageActions
{
    // Basic actions
    constexpr ActionId Melee                = 500;
    constexpr ActionId Follow               = 501;
    constexpr ActionId ReachSpell           = 502;

    // Armors (buffs)
    constexpr ActionId MoltenArmor          = 510;
    constexpr ActionId MageArmor            = 511;
    constexpr ActionId IceArmor             = 512;
    constexpr ActionId FrostArmor           = 513;
    constexpr ActionId ArcaneIntellect      = 514;

    // Cooldowns
    constexpr ActionId ArcanePower          = 520;
    constexpr ActionId PresenceOfMind       = 521;
    constexpr ActionId Combustion           = 522;
    constexpr ActionId IcyVeins             = 523;
    constexpr ActionId ColdSnap             = 524;
    constexpr ActionId MirrorImage          = 525;

    // Defensive
    constexpr ActionId IceBlock             = 530;
    constexpr ActionId IceBarrier           = 531;
    constexpr ActionId ManaShield           = 532;
    constexpr ActionId FrostWard            = 533;
    constexpr ActionId FireWard             = 534;
    constexpr ActionId Blink                = 535;
    constexpr ActionId Invisibility         = 536;

    // Utility
    constexpr ActionId Evocation            = 540;
    constexpr ActionId Counterspell         = 541;
    constexpr ActionId Polymorph            = 542;
    constexpr ActionId Spellsteal           = 543;
    constexpr ActionId RemoveCurse          = 544;
    constexpr ActionId SummonWaterElemental = 545;

    // Fire damage
    constexpr ActionId Fireball             = 550;
    constexpr ActionId Pyroblast            = 551;
    constexpr ActionId Scorch               = 552;
    constexpr ActionId FireBlast            = 553;
    constexpr ActionId LivingBomb           = 554;
    constexpr ActionId Flamestrike          = 555;
    constexpr ActionId BlastWave            = 556;
    constexpr ActionId DragonsBreath        = 557;

    // Frost damage
    constexpr ActionId Frostbolt            = 560;
    constexpr ActionId FrostfireBolt        = 561;
    constexpr ActionId IceLance             = 562;
    constexpr ActionId Blizzard             = 563;
    constexpr ActionId ConeOfCold           = 564;
    constexpr ActionId FrostNova            = 565;
    constexpr ActionId DeepFreeze           = 566;

    // Arcane damage
    constexpr ActionId ArcaneBlast          = 570;
    constexpr ActionId ArcaneMissiles       = 571;
    constexpr ActionId ArcaneBarrage        = 572;
    constexpr ActionId ArcaneExplosion      = 573;
}

// ============================================================================
// HUNTER ACTION IDS (600-699)
// ============================================================================
namespace HunterActions
{
    // Basic actions
    constexpr ActionId Melee                = 600;
    constexpr ActionId Follow               = 601;
    constexpr ActionId ReachSpell           = 602;
    constexpr ActionId AutoShot             = 603;

    // Aspects
    constexpr ActionId AspectOfTheHawk      = 610;
    constexpr ActionId AspectOfTheDragonhawk= 611;
    constexpr ActionId AspectOfTheViper     = 612;
    constexpr ActionId AspectOfTheWild      = 613;
    constexpr ActionId AspectOfThePack      = 614;
    constexpr ActionId AspectOfTheBeast     = 615;
    constexpr ActionId AspectOfTheMonkey    = 616;
    constexpr ActionId AspectOfTheCheetah   = 617;

    // Pet management
    constexpr ActionId CallPet              = 620;
    constexpr ActionId RevivePet            = 621;
    constexpr ActionId MendPet              = 622;
    constexpr ActionId FeedPet              = 623;
    constexpr ActionId DismissPet           = 624;
    constexpr ActionId KillCommand          = 625;
    constexpr ActionId BestialWrath         = 626;
    constexpr ActionId Intimidation         = 627;

    // Shots - Marksmanship
    constexpr ActionId ArcaneShot           = 630;
    constexpr ActionId SteadyShot           = 631;
    constexpr ActionId AimedShot            = 632;
    constexpr ActionId MultiShot            = 633;
    constexpr ActionId KillShot             = 634;
    constexpr ActionId ChimeraShot          = 635;
    constexpr ActionId SilencingShot        = 636;
    constexpr ActionId ScatterShot          = 637;

    // Shots - Survival
    constexpr ActionId ExplosiveShot        = 640;
    constexpr ActionId BlackArrow           = 641;
    constexpr ActionId WyvernSting          = 642;

    // Stings
    constexpr ActionId SerpentSting         = 650;
    constexpr ActionId ScorpidSting         = 651;
    constexpr ActionId ViperSting           = 652;

    // Traps
    constexpr ActionId FrostTrap            = 660;
    constexpr ActionId FreezingTrap         = 661;
    constexpr ActionId ExplosiveTrap        = 662;
    constexpr ActionId ImmolationTrap       = 663;
    constexpr ActionId SnakeTrap            = 664;

    // Utility
    constexpr ActionId HuntersMark          = 670;
    constexpr ActionId TranquilizingShot    = 671;
    constexpr ActionId DistractingShot      = 672;
    constexpr ActionId ConcussiveShot       = 673;
    constexpr ActionId FeignDeath           = 674;
    constexpr ActionId Deterrence           = 675;
    constexpr ActionId Disengage            = 676;
    constexpr ActionId MasterCall           = 677;
    constexpr ActionId Misdirection         = 678;
    constexpr ActionId Flare                = 679;
    constexpr ActionId RaptorStrike         = 680;
    constexpr ActionId MongooseBite         = 681;
    constexpr ActionId WingClip             = 682;
    constexpr ActionId CounterAttack        = 683;

    // Cooldowns
    constexpr ActionId RapidFire            = 690;
    constexpr ActionId Readiness            = 691;
}

// ============================================================================
// PRIEST ACTION IDS (700-799)
// ============================================================================
namespace PriestActions
{
    // Basic actions
    constexpr ActionId Melee                = 700;
    constexpr ActionId Follow               = 701;
    constexpr ActionId ReachSpell           = 702;

    // Buffs
    constexpr ActionId PowerWordFortitude   = 710;
    constexpr ActionId DivineSpirit         = 711;
    constexpr ActionId InnerFire            = 712;
    constexpr ActionId ShadowProtection     = 713;
    constexpr ActionId FearWard             = 714;
    constexpr ActionId Shadowform           = 715;

    // Holy heals
    constexpr ActionId Heal                 = 720;
    constexpr ActionId FlashHeal            = 721;
    constexpr ActionId GreaterHeal          = 722;
    constexpr ActionId Renew                = 723;
    constexpr ActionId HolyNova             = 724;
    constexpr ActionId BindingHeal          = 725;
    constexpr ActionId CircleOfHealing      = 726;
    constexpr ActionId PrayerOfHealing      = 727;
    constexpr ActionId PrayerOfMending      = 728;
    constexpr ActionId DivineHymn           = 729;
    constexpr ActionId Lightwell            = 730;

    // Holy cooldowns
    constexpr ActionId GuardianSpirit       = 735;

    // Discipline
    constexpr ActionId PowerWordShield      = 740;
    constexpr ActionId Penance              = 741;
    constexpr ActionId InnerFocus           = 742;
    constexpr ActionId PowerInfusion        = 743;
    constexpr ActionId PainSuppression      = 744;

    // Shadow damage
    constexpr ActionId MindBlast            = 750;
    constexpr ActionId ShadowWordPain       = 751;
    constexpr ActionId ShadowWordDeath      = 752;
    constexpr ActionId MindFlay             = 753;
    constexpr ActionId VampiricTouch        = 754;
    constexpr ActionId VampiricEmbrace      = 755;
    constexpr ActionId DevouringPlague      = 756;
    constexpr ActionId MindSear             = 757;

    // Shadow cooldowns
    constexpr ActionId Dispersion           = 760;
    constexpr ActionId Shadowfiend          = 761;

    // Utility
    constexpr ActionId DispelMagic          = 770;
    constexpr ActionId MassDispel           = 771;
    constexpr ActionId ShackleUndead        = 772;
    constexpr ActionId PsychicScream        = 773;
    constexpr ActionId Fade                 = 774;
    constexpr ActionId MindControl          = 775;
    constexpr ActionId Smite                = 776;
    constexpr ActionId HolyFire             = 777;
    constexpr ActionId Resurrection         = 778;
    constexpr ActionId AbolishDisease       = 779;
    constexpr ActionId CureDisease          = 780;
}

// ============================================================================
// WARLOCK ACTION IDS (800-899)
// ============================================================================
namespace WarlockActions
{
    // Basic actions
    constexpr ActionId Melee                = 800;
    constexpr ActionId Follow               = 801;
    constexpr ActionId ReachSpell           = 802;
    constexpr ActionId Shoot                = 803;

    // Armors
    constexpr ActionId DemonArmor           = 810;
    constexpr ActionId FelArmor             = 811;
    constexpr ActionId DemonSkin            = 812;

    // Pet management
    constexpr ActionId SummonImp            = 820;
    constexpr ActionId SummonVoidwalker     = 821;
    constexpr ActionId SummonSuccubus       = 822;
    constexpr ActionId SummonFelhunter      = 823;
    constexpr ActionId SummonFelguard       = 824;
    constexpr ActionId DemonicEmpowerment   = 825;
    constexpr ActionId HealthFunnel         = 826;

    // Affliction DoTs
    constexpr ActionId Corruption           = 830;
    constexpr ActionId CurseOfAgony         = 831;
    constexpr ActionId CurseOfDoom          = 832;
    constexpr ActionId CurseOfElements      = 833;
    constexpr ActionId CurseOfTongues       = 834;
    constexpr ActionId CurseOfWeakness      = 835;
    constexpr ActionId UnstableAffliction   = 836;
    constexpr ActionId Haunt                = 837;

    // Demonology
    constexpr ActionId Metamorphosis        = 840;
    constexpr ActionId ImmolationAura       = 841;
    constexpr ActionId ShadowCleave         = 842;
    constexpr ActionId DemonicCharge        = 843;

    // Destruction
    constexpr ActionId ShadowBolt           = 850;
    constexpr ActionId Incinerate           = 851;
    constexpr ActionId Immolate             = 852;
    constexpr ActionId Conflagrate          = 853;
    constexpr ActionId ChaosBolt            = 854;
    constexpr ActionId SoulFire             = 855;
    constexpr ActionId Shadowburn           = 856;
    constexpr ActionId SearingPain          = 857;

    // AoE
    constexpr ActionId RainOfFire           = 860;
    constexpr ActionId SeedOfCorruption     = 861;
    constexpr ActionId Hellfire             = 862;
    constexpr ActionId Shadowflame          = 863;

    // Life Tap / Mana
    constexpr ActionId LifeTap              = 870;
    constexpr ActionId DarkPact             = 871;
    constexpr ActionId DrainMana            = 872;
    constexpr ActionId DrainLife            = 873;
    constexpr ActionId DrainSoul            = 874;

    // Utility
    constexpr ActionId Fear                 = 880;
    constexpr ActionId HowlOfTerror         = 881;
    constexpr ActionId DeathCoil            = 882;
    constexpr ActionId Shadowfury           = 883;
    constexpr ActionId Banish               = 884;
    constexpr ActionId CreateHealthstone    = 885;
    constexpr ActionId CreateSoulstone      = 886;
}

// ============================================================================
// DRUID ACTION IDS (900-999)
// ============================================================================
namespace DruidActions
{
    // Basic actions
    constexpr ActionId Melee                = 900;
    constexpr ActionId Follow               = 901;
    constexpr ActionId ReachSpell           = 902;

    // Shapeshifts
    constexpr ActionId CatForm              = 910;
    constexpr ActionId BearForm             = 911;
    constexpr ActionId DireBearForm         = 912;
    constexpr ActionId MoonkinForm          = 913;
    constexpr ActionId TreeOfLifeForm       = 914;
    constexpr ActionId TravelForm           = 915;
    constexpr ActionId AquaticForm          = 916;
    constexpr ActionId FlightForm           = 917;
    constexpr ActionId SwiftFlightForm      = 918;

    // Buffs
    constexpr ActionId MarkOfTheWild        = 920;
    constexpr ActionId GiftOfTheWild        = 921;
    constexpr ActionId Thorns               = 922;

    // Balance (Moonkin) damage
    constexpr ActionId Wrath                = 930;
    constexpr ActionId Starfire             = 931;
    constexpr ActionId Moonfire             = 932;
    constexpr ActionId InsectSwarm          = 933;
    constexpr ActionId Starfall             = 934;
    constexpr ActionId Typhoon              = 935;
    constexpr ActionId Hurricane            = 936;
    constexpr ActionId ForceOfNature        = 937;

    // Feral Cat abilities
    constexpr ActionId MangleCat            = 940;
    constexpr ActionId Shred                = 941;
    constexpr ActionId Rake                 = 942;
    constexpr ActionId Rip                  = 943;
    constexpr ActionId FerociousBite        = 944;
    constexpr ActionId SavageRoar           = 945;
    constexpr ActionId TigersFury           = 946;
    constexpr ActionId Ravage               = 947;
    constexpr ActionId Pounce               = 948;
    constexpr ActionId Prowl                = 949;

    // Feral Bear abilities
    constexpr ActionId MangleBear           = 950;
    constexpr ActionId Maul                 = 951;
    constexpr ActionId Swipe                = 952;
    constexpr ActionId Lacerate             = 953;
    constexpr ActionId FaerieFire           = 954;
    constexpr ActionId DemoralizingRoar     = 955;
    constexpr ActionId Bash                 = 956;
    constexpr ActionId Growl                = 957;
    constexpr ActionId ChallengingRoar      = 958;
    constexpr ActionId FrenziedRegeneration = 959;
    constexpr ActionId SurvivalInstincts    = 960;
    constexpr ActionId Barkskin             = 961;
    constexpr ActionId Enrage               = 962;
    constexpr ActionId Berserk              = 963;

    // Restoration heals
    constexpr ActionId HealingTouch         = 970;
    constexpr ActionId Regrowth             = 971;
    constexpr ActionId Rejuvenation         = 972;
    constexpr ActionId Lifebloom            = 973;
    constexpr ActionId Nourish              = 974;
    constexpr ActionId WildGrowth           = 975;
    constexpr ActionId Swiftmend            = 976;
    constexpr ActionId Tranquility          = 977;
    constexpr ActionId NaturesSwiftness     = 978;

    // Utility
    constexpr ActionId Innervate            = 980;
    constexpr ActionId Rebirth              = 981;
    constexpr ActionId RemoveCurse          = 982;
    constexpr ActionId AbolishPoison        = 983;
    constexpr ActionId Hibernate            = 984;
    constexpr ActionId EntanglingRoots      = 985;
    constexpr ActionId Cyclone              = 986;
    constexpr ActionId Dash                 = 987;
}

// ============================================================================
// SHAMAN ACTION IDS (1000-1099)
// ============================================================================
namespace ShamanActions
{
    // Basic actions
    constexpr ActionId Melee                = 1000;
    constexpr ActionId Follow               = 1001;
    constexpr ActionId ReachSpell           = 1002;

    // Totems - General
    constexpr ActionId StrengthOfEarthTotem = 1010;
    constexpr ActionId StoneskinTotem       = 1011;
    constexpr ActionId TremorTotem          = 1012;
    constexpr ActionId EarthbindTotem       = 1013;
    constexpr ActionId SearingTotem         = 1014;
    constexpr ActionId MagmaTotem           = 1015;
    constexpr ActionId FireNovaTotem        = 1016;
    constexpr ActionId FlametongueTotem     = 1017;
    constexpr ActionId HealingStreamTotem   = 1018;
    constexpr ActionId ManaSpringTotem      = 1019;
    constexpr ActionId TotemOfWrath         = 1020;
    constexpr ActionId WindfuryTotem        = 1021;
    constexpr ActionId GraceOfAirTotem      = 1022;
    constexpr ActionId WrathOfAirTotem      = 1023;

    // Buffs
    constexpr ActionId LightningShield      = 1030;
    constexpr ActionId WaterShield          = 1031;
    constexpr ActionId EarthShield          = 1032;
    constexpr ActionId Bloodlust            = 1033;
    constexpr ActionId Heroism              = 1034;

    // Elemental damage
    constexpr ActionId LightningBolt        = 1040;
    constexpr ActionId ChainLightning       = 1041;
    constexpr ActionId LavaBurst            = 1042;
    constexpr ActionId FlameShock           = 1043;
    constexpr ActionId FrostShock           = 1044;
    constexpr ActionId EarthShock           = 1045;
    constexpr ActionId Thunderstorm         = 1046;
    constexpr ActionId FireNova             = 1047;

    // Enhancement abilities
    constexpr ActionId Stormstrike          = 1050;
    constexpr ActionId LavaLash             = 1051;
    constexpr ActionId FeralSpirit          = 1052;
    constexpr ActionId ShamanisticRage      = 1053;
    constexpr ActionId MaelstromWeapon      = 1054;

    // Weapon enhancements
    constexpr ActionId WindfuryWeapon       = 1060;
    constexpr ActionId FlametongueWeapon    = 1061;
    constexpr ActionId RockbiterWeapon      = 1062;
    constexpr ActionId FrostbrandWeapon     = 1063;
    constexpr ActionId EarthlivingWeapon    = 1064;

    // Restoration heals
    constexpr ActionId HealingWave          = 1070;
    constexpr ActionId LesserHealingWave    = 1071;
    constexpr ActionId ChainHeal            = 1072;
    constexpr ActionId Riptide              = 1073;
    constexpr ActionId TidalForce           = 1074;
    constexpr ActionId NaturesSwiftness     = 1075;

    // Utility
    constexpr ActionId PurgeAction          = 1080;
    constexpr ActionId WindShear            = 1081;
    constexpr ActionId Hex                  = 1082;
    constexpr ActionId GhostWolf            = 1083;
    constexpr ActionId AncestralSpirit      = 1084;
    constexpr ActionId CleanseSpiritAction  = 1085;
    constexpr ActionId CurePoison           = 1086;
    constexpr ActionId CureDisease          = 1087;
}

#endif // _PLAYERBOT_FSMENGINE_H
