/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_BOTSTATEMASK_H
#define _PLAYERBOT_BOTSTATEMASK_H

#include <cstdint>

/**
 * Bitmask-based bot state representation for ultra-fast state checking.
 * Replaces 191 virtual Trigger::IsActive() calls with single bitmask operations.
 *
 * Total: 256 bits (4 x uint64_t)
 * - primary: Combat, health, resources, movement states
 * - secondary: Buffs, items, RPG, group states
 * - classFlags: Class-specific states (stances, forms, procs)
 * - events: Transient event flags (cleared each tick)
 */

// ============================================================================
// PRIMARY STATE FLAGS (64 bits) - Most frequently checked
// ============================================================================
namespace PrimaryState
{
    // Boolean combat states (bits 0-15)
    constexpr uint64_t InCombat           = 1ULL << 0;
    constexpr uint64_t HasTarget          = 1ULL << 1;
    constexpr uint64_t TargetAttackable   = 1ULL << 2;
    constexpr uint64_t HasAttackers       = 1ULL << 3;
    constexpr uint64_t HasAggro           = 1ULL << 4;
    constexpr uint64_t LoseAggro          = 1ULL << 5;  // Had aggro, lost it
    constexpr uint64_t BehindTarget       = 1ULL << 6;
    constexpr uint64_t FacingTarget       = 1ULL << 7;
    constexpr uint64_t TargetCasting      = 1ULL << 8;
    constexpr uint64_t TargetFleeing      = 1ULL << 9;
    constexpr uint64_t InvalidTarget      = 1ULL << 10;
    constexpr uint64_t PossibleAdds       = 1ULL << 11;
    constexpr uint64_t HasNearestAdds     = 1ULL << 12;
    constexpr uint64_t CanInterrupt       = 1ULL << 13;
    constexpr uint64_t Reserved14         = 1ULL << 14;
    constexpr uint64_t Reserved15         = 1ULL << 15;

    // Health level (bits 16-18): 0-7 scale
    // 0=critical(<20%), 1-2=low(<40%), 3-4=medium(<60%), 5-6=high(<85%), 7=full
    constexpr uint64_t HealthLevelShift   = 16;
    constexpr uint64_t HealthLevelMask    = 0x7ULL << HealthLevelShift;

    // Mana level (bits 19-21): 0-7 scale
    constexpr uint64_t ManaLevelShift     = 19;
    constexpr uint64_t ManaLevelMask      = 0x7ULL << ManaLevelShift;

    // Rage level (bits 22-23): 0-3 scale (0-19, 20-39, 40-59, 60+)
    constexpr uint64_t RageLevelShift     = 22;
    constexpr uint64_t RageLevelMask      = 0x3ULL << RageLevelShift;

    // Energy level (bits 24-25): 0-3 scale
    constexpr uint64_t EnergyLevelShift   = 24;
    constexpr uint64_t EnergyLevelMask    = 0x3ULL << EnergyLevelShift;

    // Combo points (bits 26-28): 0-5
    constexpr uint64_t ComboPointsShift   = 26;
    constexpr uint64_t ComboPointsMask    = 0x7ULL << ComboPointsShift;

    // Attacker count (bits 29-31): 0-7+
    constexpr uint64_t AttackerCountShift = 29;
    constexpr uint64_t AttackerCountMask  = 0x7ULL << AttackerCountShift;

    // Enemy distance band (bits 32-34):
    // 0=inside, 1=melee, 2=close, 3=mid, 4=spell, 5=far, 6=out_of_range, 7=no_target
    constexpr uint64_t EnemyRangeShift    = 32;
    constexpr uint64_t EnemyRangeMask     = 0x7ULL << EnemyRangeShift;

    // Master distance band (bits 35-36):
    // 0=close, 1=follow_range, 2=far, 3=out_of_react
    constexpr uint64_t MasterRangeShift   = 35;
    constexpr uint64_t MasterRangeMask    = 0x3ULL << MasterRangeShift;

    // Movement & vehicle states (bits 37-44)
    constexpr uint64_t Moving             = 1ULL << 37;
    constexpr uint64_t Mounted            = 1ULL << 38;
    constexpr uint64_t Swimming           = 1ULL << 39;
    constexpr uint64_t Falling            = 1ULL << 40;
    constexpr uint64_t FallingFar         = 1ULL << 41;
    constexpr uint64_t InVehicle          = 1ULL << 42;
    constexpr uint64_t Collision          = 1ULL << 43;
    constexpr uint64_t Stuck              = 1ULL << 44;

    // Unit states (bits 45-51)
    constexpr uint64_t Dead               = 1ULL << 45;
    constexpr uint64_t HasPet             = 1ULL << 46;
    constexpr uint64_t PetDead            = 1ULL << 47;
    constexpr uint64_t InGroup            = 1ULL << 48;
    constexpr uint64_t IsTank             = 1ULL << 49;
    constexpr uint64_t IsHealer           = 1ULL << 50;
    constexpr uint64_t IsMelee            = 1ULL << 51;

    // AOE count near target (bits 52-55): 0-15
    constexpr uint64_t AoeCountShift      = 52;
    constexpr uint64_t AoeCountMask       = 0xFULL << AoeCountShift;

    // Party health state (bits 56-59): worst member health level
    constexpr uint64_t PartyHealthShift   = 56;
    constexpr uint64_t PartyHealthMask    = 0xFULL << PartyHealthShift;

    // Reserved (bits 60-63)
    constexpr uint64_t LongStuck          = 1ULL << 60;
    constexpr uint64_t CombatStuck        = 1ULL << 61;
    constexpr uint64_t CombatLongStuck    = 1ULL << 62;
    constexpr uint64_t Reserved63         = 1ULL << 63;
}

// ============================================================================
// SECONDARY STATE FLAGS (64 bits) - Less frequently checked
// ============================================================================
namespace SecondaryState
{
    // Loot & inventory (bits 0-7)
    constexpr uint64_t LootAvailable      = 1ULL << 0;
    constexpr uint64_t CanLoot            = 1ULL << 1;
    constexpr uint64_t CorpseNear         = 1ULL << 2;
    constexpr uint64_t NoFood             = 1ULL << 3;
    constexpr uint64_t NoDrink            = 1ULL << 4;
    constexpr uint64_t NeedsRepair        = 1ULL << 5;
    constexpr uint64_t BagsFull           = 1ULL << 6;
    constexpr uint64_t Reserved7          = 1ULL << 7;

    // Buff & debuff states (bits 8-15)
    constexpr uint64_t NeedsBuff          = 1ULL << 8;
    constexpr uint64_t NeedsPartyBuff     = 1ULL << 9;
    constexpr uint64_t NeedsCure          = 1ULL << 10;
    constexpr uint64_t NeedsPartyCure     = 1ULL << 11;
    constexpr uint64_t HasAreaDebuff      = 1ULL << 12;
    constexpr uint64_t PartyNeedsHeal     = 1ULL << 13;
    constexpr uint64_t PartyMemberDead    = 1ULL << 14;
    constexpr uint64_t ProtectNeeded      = 1ULL << 15;

    // RPG & travel states (bits 16-23)
    constexpr uint64_t HasRpgTarget       = 1ULL << 16;
    constexpr uint64_t NearRpgTarget      = 1ULL << 17;
    constexpr uint64_t HasTravelTarget    = 1ULL << 18;
    constexpr uint64_t AtTravelTarget     = 1ULL << 19;
    constexpr uint64_t CanFish            = 1ULL << 20;
    constexpr uint64_t BobberReady        = 1ULL << 21;
    constexpr uint64_t VehicleNear        = 1ULL << 22;
    constexpr uint64_t Reserved23         = 1ULL << 23;

    // PvP & battleground states (bits 24-31)
    constexpr uint64_t InBattleground     = 1ULL << 24;
    constexpr uint64_t BgActive           = 1ULL << 25;
    constexpr uint64_t BgWaiting          = 1ULL << 26;
    constexpr uint64_t BgInvite           = 1ULL << 27;
    constexpr uint64_t HasFlag            = 1ULL << 28;
    constexpr uint64_t TeamHasFlag        = 1ULL << 29;
    constexpr uint64_t EnemyHasFlag       = 1ULL << 30;
    constexpr uint64_t EnemyPlayerNear    = 1ULL << 31;

    // Group & tank states (bits 32-39)
    constexpr uint64_t TankLowThreat      = 1ULL << 32;
    constexpr uint64_t MasterInCombat     = 1ULL << 33;
    constexpr uint64_t FarFromMaster      = 1ULL << 34;
    constexpr uint64_t OutOfReactRange    = 1ULL << 35;
    constexpr uint64_t CanSelfRes         = 1ULL << 36;
    constexpr uint64_t NoPossibleTargets  = 1ULL << 37;
    constexpr uint64_t Reserved38         = 1ULL << 38;
    constexpr uint64_t Reserved39         = 1ULL << 39;

    // LFG & dungeon states (bits 40-47)
    constexpr uint64_t LfgProposal        = 1ULL << 40;
    constexpr uint64_t UnknownDungeon     = 1ULL << 41;
    constexpr uint64_t NeedWorldBuff      = 1ULL << 42;
    constexpr uint64_t NewPet             = 1ULL << 43;
    constexpr uint64_t NewPlayerNear      = 1ULL << 44;
    constexpr uint64_t NoNonBotsNear      = 1ULL << 45;
    constexpr uint64_t Reserved46         = 1ULL << 46;
    constexpr uint64_t Reserved47         = 1ULL << 47;

    // Reserved for expansion (bits 48-63)
}

// ============================================================================
// CLASS-SPECIFIC STATE FLAGS (64 bits)
// ============================================================================
namespace ClassState
{
    // Warrior stances (bits 0-2)
    constexpr uint64_t BattleStance       = 1ULL << 0;
    constexpr uint64_t DefensiveStance    = 1ULL << 1;
    constexpr uint64_t BerserkerStance    = 1ULL << 2;

    // Rogue states (bits 3-5)
    constexpr uint64_t InStealth          = 1ULL << 3;
    constexpr uint64_t Stealthed          = 1ULL << 4;  // Can stealth
    constexpr uint64_t HasPoisons         = 1ULL << 5;

    // Druid forms (bits 6-10)
    constexpr uint64_t CatForm            = 1ULL << 6;
    constexpr uint64_t BearForm           = 1ULL << 7;
    constexpr uint64_t MoonkinForm        = 1ULL << 8;
    constexpr uint64_t TreeForm           = 1ULL << 9;
    constexpr uint64_t TravelForm         = 1ULL << 10;

    // Paladin auras & seals (bits 11-14)
    constexpr uint64_t HasAura            = 1ULL << 11;
    constexpr uint64_t HasSeal            = 1ULL << 12;
    constexpr uint64_t HasBlessing        = 1ULL << 13;
    constexpr uint64_t Reserved14         = 1ULL << 14;

    // Warlock/Mage pet states (bits 15-18)
    constexpr uint64_t PetAttacking       = 1ULL << 15;
    constexpr uint64_t PetDefensive       = 1ULL << 16;
    constexpr uint64_t PetPassive         = 1ULL << 17;
    constexpr uint64_t Reserved18         = 1ULL << 18;

    // Shaman totems (bits 19-22)
    constexpr uint64_t HasFireTotem       = 1ULL << 19;
    constexpr uint64_t HasEarthTotem      = 1ULL << 20;
    constexpr uint64_t HasWaterTotem      = 1ULL << 21;
    constexpr uint64_t HasAirTotem        = 1ULL << 22;

    // Proc states (bits 23-30) - class-specific procs
    constexpr uint64_t ProcActive         = 1ULL << 23;  // Generic proc flag
    constexpr uint64_t Overpower          = 1ULL << 24;  // Warrior
    constexpr uint64_t Revenge            = 1ULL << 25;  // Warrior
    constexpr uint64_t SwordAndBoard      = 1ULL << 26;  // Warrior
    constexpr uint64_t HotStreak          = 1ULL << 27;  // Mage
    constexpr uint64_t FingersOfFrost     = 1ULL << 28;  // Mage
    constexpr uint64_t Nightfall          = 1ULL << 29;  // Warlock
    constexpr uint64_t Reserved30         = 1ULL << 30;

    // Hunter states (bits 31-34)
    constexpr uint64_t AspectActive       = 1ULL << 31;
    constexpr uint64_t HasAmmo            = 1ULL << 32;
    constexpr uint64_t PetHappy           = 1ULL << 33;
    constexpr uint64_t Reserved34         = 1ULL << 34;

    // Death Knight presences (bits 35-37)
    constexpr uint64_t BloodPresence      = 1ULL << 35;
    constexpr uint64_t FrostPresence      = 1ULL << 36;
    constexpr uint64_t UnholyPresence     = 1ULL << 37;

    // Reserved (bits 38-63)
}

// ============================================================================
// EVENT FLAGS (64 bits) - Transient, cleared each tick
// ============================================================================
namespace EventState
{
    constexpr uint64_t TargetChanged      = 1ULL << 0;
    constexpr uint64_t DamageTaken        = 1ULL << 1;
    constexpr uint64_t HealReceived       = 1ULL << 2;
    constexpr uint64_t CombatStart        = 1ULL << 3;
    constexpr uint64_t CombatEnd          = 1ULL << 4;
    constexpr uint64_t SpellInterrupt     = 1ULL << 5;
    constexpr uint64_t TargetDied         = 1ULL << 6;
    constexpr uint64_t PartyMemberDied    = 1ULL << 7;
    constexpr uint64_t BuffApplied        = 1ULL << 8;
    constexpr uint64_t DebuffApplied      = 1ULL << 9;
    constexpr uint64_t TimerTick          = 1ULL << 10;  // 1-second timer
    constexpr uint64_t RandomTrigger      = 1ULL << 11;  // Random check passed

    // Reserved (bits 12-63)
}

// ============================================================================
// BOT STATE VECTOR - Complete bot state in 256 bits
// ============================================================================
struct BotStateVector
{
    uint64_t primary;       // PrimaryState flags
    uint64_t secondary;     // SecondaryState flags
    uint64_t classFlags;    // ClassState flags
    uint64_t events;        // EventState flags (cleared each tick)

    // Default constructor
    BotStateVector() : primary(0), secondary(0), classFlags(0), events(0) {}

    // Check if all specified flags are set in a layer
    bool hasAll(uint64_t mask, uint8_t layer = 0) const
    {
        const uint64_t* layers[] = {&primary, &secondary, &classFlags, &events};
        return (*layers[layer] & mask) == mask;
    }

    // Check if any of the specified flags are set
    bool hasAny(uint64_t mask, uint8_t layer = 0) const
    {
        const uint64_t* layers[] = {&primary, &secondary, &classFlags, &events};
        return (*layers[layer] & mask) != 0;
    }

    // Check if none of the specified flags are set
    bool hasNone(uint64_t mask, uint8_t layer = 0) const
    {
        const uint64_t* layers[] = {&primary, &secondary, &classFlags, &events};
        return (*layers[layer] & mask) == 0;
    }

    // Get a multi-bit field value
    uint8_t getField(uint64_t mask, uint8_t shift, uint8_t layer = 0) const
    {
        const uint64_t* layers[] = {&primary, &secondary, &classFlags, &events};
        return static_cast<uint8_t>((*layers[layer] & mask) >> shift);
    }

    // Set a multi-bit field value
    void setField(uint64_t mask, uint8_t shift, uint8_t value, uint8_t layer = 0)
    {
        uint64_t* layers[] = {&primary, &secondary, &classFlags, &events};
        *layers[layer] = (*layers[layer] & ~mask) | (static_cast<uint64_t>(value) << shift);
    }

    // Clear event flags (call at start of each tick)
    void clearEvents() { events = 0; }

    // Fast equality check
    bool operator==(const BotStateVector& other) const
    {
        return primary == other.primary &&
               secondary == other.secondary &&
               classFlags == other.classFlags;
        // Note: events not compared (transient)
    }
};

// ============================================================================
// HELPER MACROS FOR STATE CHECKS
// ============================================================================

// Check single flag
#define FSM_HAS_FLAG(state, flag) ((state).primary & (flag))
#define FSM_HAS_FLAG2(state, flag) ((state).secondary & (flag))
#define FSM_HAS_CLASS(state, flag) ((state).classFlags & (flag))
#define FSM_HAS_EVENT(state, flag) ((state).events & (flag))

// Set/clear flag
#define FSM_SET_FLAG(state, flag) ((state).primary |= (flag))
#define FSM_CLEAR_FLAG(state, flag) ((state).primary &= ~(flag))
#define FSM_SET_OR_CLEAR(state, flag, cond) \
    do { if (cond) (state).primary |= (flag); else (state).primary &= ~(flag); } while(0)

// Get health level (0-7)
#define FSM_HEALTH_LEVEL(state) \
    (((state).primary & PrimaryState::HealthLevelMask) >> PrimaryState::HealthLevelShift)

// Get mana level (0-7)
#define FSM_MANA_LEVEL(state) \
    (((state).primary & PrimaryState::ManaLevelMask) >> PrimaryState::ManaLevelShift)

// Get attacker count (0-7)
#define FSM_ATTACKER_COUNT(state) \
    (((state).primary & PrimaryState::AttackerCountMask) >> PrimaryState::AttackerCountShift)

// Get enemy range band (0-7)
#define FSM_ENEMY_RANGE(state) \
    (((state).primary & PrimaryState::EnemyRangeMask) >> PrimaryState::EnemyRangeShift)

// Get combo points (0-5)
#define FSM_COMBO_POINTS(state) \
    (((state).primary & PrimaryState::ComboPointsMask) >> PrimaryState::ComboPointsShift)

// ============================================================================
// RANGE BAND CONSTANTS
// ============================================================================
namespace RangeBand
{
    constexpr uint8_t Inside       = 0;  // Inside target hitbox
    constexpr uint8_t Melee        = 1;  // Within melee range
    constexpr uint8_t Close        = 2;  // Within tooCloseDistance
    constexpr uint8_t Mid          = 3;  // Within meleeDistance
    constexpr uint8_t Spell        = 4;  // Within spell range
    constexpr uint8_t Far          = 5;  // Within sight distance
    constexpr uint8_t OutOfRange   = 6;  // Beyond sight distance
    constexpr uint8_t NoTarget     = 7;  // No target
}

// ============================================================================
// HEALTH LEVEL CONSTANTS
// ============================================================================
namespace HealthLevel
{
    constexpr uint8_t Critical     = 0;  // < 20%
    constexpr uint8_t VeryLow      = 1;  // < 30%
    constexpr uint8_t Low          = 2;  // < 40%
    constexpr uint8_t MedLow       = 3;  // < 50%
    constexpr uint8_t Medium       = 4;  // < 60%
    constexpr uint8_t MedHigh      = 5;  // < 75%
    constexpr uint8_t High         = 6;  // < 85%
    constexpr uint8_t Full         = 7;  // >= 85%
}

#endif // _PLAYERBOT_BOTSTATEMASK_H
