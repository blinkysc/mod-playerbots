/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "FSMEngine.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "Playerbots.h"
#include "AiObjectContext.h"
#include "Player.h"
#include "SpellAuras.h"
#include <algorithm>

FSMEngine::FSMEngine(PlayerbotAI* ai)
    : botAI(ai)
    , bot(ai ? ai->GetBot() : nullptr)
    , context(ai ? ai->GetAiObjectContext() : nullptr)
    , lastAction(INVALID_ACTION)
    , lastActionTime(0)
    , lastTimerTick(0)
{
}

FSMEngine::~FSMEngine()
{
}

// ============================================================================
// MAIN TICK
// ============================================================================

bool FSMEngine::DoNextAction(Unit* target, int depth, bool minimal)
{
    if (!botAI || !bot || !bot->IsInWorld())
        return false;

    if (bot->IsBeingTeleported() || bot->IsBeingTeleportedNear())
        return false;

    uint32_t now = getMSTime();

    // Phase 1: Update state bitmask
    UpdateState();

    // Phase 2: Select best action
    ActionId action = SelectBestAction();

    // Phase 3: Execute
    if (action != INVALID_ACTION)
    {
        // Find the action definition
        for (const ActionTable* table : activeStrategies)
        {
            for (uint16_t i = 0; i < table->ruleCount; ++i)
            {
                if (table->rules[i].id == action)
                {
                    if (ExecuteActionInternal(action, &table->rules[i]))
                    {
                        lastAction = action;
                        lastActionTime = now;
                        return true;
                    }
                    break;
                }
            }
        }

        // If not found in tables, try direct execution
        return ExecuteAction(action);
    }

    return false;
}

// ============================================================================
// STATE UPDATE
// ============================================================================

void FSMEngine::UpdateState()
{
    previousState = currentState;
    currentState.clearEvents();

    UpdatePrimaryState();
    UpdateSecondaryState();
    UpdateClassState();
    UpdateEventState();
}

void FSMEngine::UpdatePrimaryState()
{
    if (!bot || !botAI)
        return;

    uint64_t& state = currentState.primary;

    // Reset boolean flags (keep multi-bit fields)
    state &= (PrimaryState::HealthLevelMask | PrimaryState::ManaLevelMask |
              PrimaryState::RageLevelMask | PrimaryState::EnergyLevelMask |
              PrimaryState::ComboPointsMask | PrimaryState::AttackerCountMask |
              PrimaryState::EnemyRangeMask | PrimaryState::MasterRangeMask |
              PrimaryState::AoeCountMask | PrimaryState::PartyHealthMask);

    // Combat state
    if (bot->IsInCombat())
        state |= PrimaryState::InCombat;

    // Target state
    Unit* target = GET_CURRENT_TARGET();
    if (target)
    {
        state |= PrimaryState::HasTarget;

        if (bot->IsValidAttackTarget(target))
            state |= PrimaryState::TargetAttackable;

        // Positional checks
        if (target->HasInArc(M_PI, bot))
            state |= PrimaryState::BehindTarget;

        if (bot->HasInArc(M_PI, target))
            state |= PrimaryState::FacingTarget;

        // Target casting
        if (target->IsNonMeleeSpellCast(false, false, true))
            state |= PrimaryState::TargetCasting;

        // Invalid target check
        if (AI_VALUE2(bool, "invalid target", "current target"))
            state |= PrimaryState::InvalidTarget;

        // Range calculation
        float dist = bot->GetDistance(target);
        uint8_t rangeBand = CalculateRangeBand(dist);
        currentState.setField(PrimaryState::EnemyRangeMask, PrimaryState::EnemyRangeShift, rangeBand);
    }
    else
    {
        currentState.setField(PrimaryState::EnemyRangeMask, PrimaryState::EnemyRangeShift, RangeBand::NoTarget);
    }

    // Attacker state
    uint8_t attackerCount = std::min<uint8_t>(7, GET_ATTACKER_COUNT());
    currentState.setField(PrimaryState::AttackerCountMask, PrimaryState::AttackerCountShift, attackerCount);

    if (attackerCount > 0)
        state |= PrimaryState::HasAttackers;

    // Aggro state (simplified check)
    if (target && AI_VALUE2(bool, "has aggro", "current target"))
        state |= PrimaryState::HasAggro;

    // Movement states
    if (AI_VALUE2(bool, "moving", "self target"))
        state |= PrimaryState::Moving;
    if (bot->IsMounted())
        state |= PrimaryState::Mounted;
    if (bot->isSwimming())
        state |= PrimaryState::Swimming;
    if (bot->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
        state |= PrimaryState::Falling;
    if (bot->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_FAR))
        state |= PrimaryState::FallingFar;
    if (bot->GetVehicle())
        state |= PrimaryState::InVehicle;

    // Unit states
    if (!bot->IsAlive())
        state |= PrimaryState::Dead;

    Unit* pet = bot->GetPet();
    if (pet)
    {
        state |= PrimaryState::HasPet;
        if (!pet->IsAlive())
            state |= PrimaryState::PetDead;
    }

    // Group state
    if (bot->GetGroup())
    {
        state |= PrimaryState::InGroup;

        // Role detection (simplified)
        // TODO: Use proper role detection from existing code
    }

    // Health level
    uint8_t healthPct = static_cast<uint8_t>(bot->GetHealthPct());
    uint8_t healthLevel = CalculateHealthLevel(healthPct);
    currentState.setField(PrimaryState::HealthLevelMask, PrimaryState::HealthLevelShift, healthLevel);

    // Mana level
    if (bot->getPowerType() == POWER_MANA)
    {
        uint8_t manaPct = static_cast<uint8_t>(bot->GetPower(POWER_MANA) * 100 / std::max(1u, bot->GetMaxPower(POWER_MANA)));
        uint8_t manaLevel = CalculateManaLevel(manaPct);
        currentState.setField(PrimaryState::ManaLevelMask, PrimaryState::ManaLevelShift, manaLevel);
    }

    // Rage level
    if (bot->getPowerType() == POWER_RAGE)
    {
        uint32_t rage = bot->GetPower(POWER_RAGE) / 10;  // Rage is stored as rage * 10
        uint8_t rageLevel = (rage < 20) ? 0 : (rage < 40) ? 1 : (rage < 60) ? 2 : 3;
        currentState.setField(PrimaryState::RageLevelMask, PrimaryState::RageLevelShift, rageLevel);
    }

    // Energy level
    if (bot->getPowerType() == POWER_ENERGY)
    {
        uint32_t energy = bot->GetPower(POWER_ENERGY);
        uint8_t energyLevel = (energy < 20) ? 0 : (energy < 40) ? 1 : (energy < 60) ? 2 : 3;
        currentState.setField(PrimaryState::EnergyLevelMask, PrimaryState::EnergyLevelShift, energyLevel);
    }

    // Combo points
    if (target)
    {
        uint8_t combo = std::min((uint8_t)5, bot->GetComboPoints());
        currentState.setField(PrimaryState::ComboPointsMask, PrimaryState::ComboPointsShift, combo);
    }

    // Possible adds
    if (AI_VALUE(bool, "possible adds"))
        state |= PrimaryState::PossibleAdds;
}

void FSMEngine::UpdateSecondaryState()
{
    if (!bot || !botAI)
        return;

    uint64_t& state = currentState.secondary;
    state = 0;  // Reset all secondary flags

    // Loot state
    if (AI_VALUE(bool, "has available loot"))
        state |= SecondaryState::LootAvailable;

    // Battleground state
    if (bot->InBattleground())
    {
        state |= SecondaryState::InBattleground;
        if (bot->GetBattleground() && bot->GetBattleground()->GetStatus() == STATUS_IN_PROGRESS)
            state |= SecondaryState::BgActive;
    }

    // Master distance (if in group with master)
    Player* master = botAI->GetMaster();
    if (master && master != bot)
    {
        float masterDist = bot->GetDistance(master);
        uint8_t masterRange;
        if (masterDist <= sPlayerbotAIConfig->followDistance)
            masterRange = 0;  // Close
        else if (masterDist <= sPlayerbotAIConfig->sightDistance)
            masterRange = 1;  // Follow range
        else if (masterDist <= sPlayerbotAIConfig->reactDistance)
            masterRange = 2;  // Far
        else
            masterRange = 3;  // Out of react

        currentState.setField(PrimaryState::MasterRangeMask, PrimaryState::MasterRangeShift, masterRange);

        if (masterRange >= 2)
            state |= SecondaryState::FarFromMaster;
        if (masterRange >= 3)
            state |= SecondaryState::OutOfReactRange;
    }

    // Party heal state
    if (currentState.primary & PrimaryState::InGroup)
    {
        // TODO: Check party members for heal requirements
        // For now, use existing value
        Unit* healTarget = AI_VALUE(Unit*, "party member to heal");
        if (healTarget)
            state |= SecondaryState::PartyNeedsHeal;

        // Check for dead party members
        Unit* deadMember = AI_VALUE(Unit*, "party member to resurrect");
        if (deadMember)
            state |= SecondaryState::PartyMemberDead;
    }

    // Area debuff
    if (AI_VALUE(bool, "has area debuff"))
        state |= SecondaryState::HasAreaDebuff;
}

void FSMEngine::UpdateClassState()
{
    if (!bot || !botAI)
        return;

    uint64_t& state = currentState.classFlags;
    state = 0;  // Reset all class flags

    uint8_t botClass = bot->getClass();

    switch (botClass)
    {
        case CLASS_WARRIOR:
        {
            // Check stance
            if (bot->HasAura(2457))  // Battle Stance
                state |= ClassState::BattleStance;
            else if (bot->HasAura(71))  // Defensive Stance
                state |= ClassState::DefensiveStance;
            else if (bot->HasAura(2458))  // Berserker Stance
                state |= ClassState::BerserkerStance;

            // Check for Overpower proc (target dodged)
            if (bot->HasAura(60503))  // Overpower available aura
                state |= ClassState::Overpower;

            // Check for Revenge proc
            if (bot->HasAura(20132) || bot->HasAura(5301))  // Revenge auras
                state |= ClassState::Revenge;

            // Sword and Board proc
            if (bot->HasAura(50227))
                state |= ClassState::SwordAndBoard;
            break;
        }

        case CLASS_ROGUE:
        {
            if (bot->HasAura(1784) || bot->HasAura(1785) || bot->HasAura(1786) || bot->HasAura(1787))
                state |= ClassState::InStealth;
            break;
        }

        case CLASS_DRUID:
        {
            // Check forms
            ShapeshiftForm form = bot->GetShapeshiftForm();
            switch (form)
            {
                case FORM_CAT:
                    state |= ClassState::CatForm;
                    break;
                case FORM_BEAR:
                case FORM_DIREBEAR:
                    state |= ClassState::BearForm;
                    break;
                case FORM_MOONKIN:
                    state |= ClassState::MoonkinForm;
                    break;
                case FORM_TREE:
                    state |= ClassState::TreeForm;
                    break;
                case FORM_TRAVEL:
                case FORM_AQUA:
                case FORM_FLIGHT:
                case FORM_FLIGHT_EPIC:
                    state |= ClassState::TravelForm;
                    break;
                default:
                    break;
            }
            break;
        }

        case CLASS_PALADIN:
        {
            // Check for active aura
            // TODO: Specific aura detection
            break;
        }

        case CLASS_MAGE:
        {
            // Hot Streak
            if (bot->HasAura(48108))
                state |= ClassState::HotStreak;
            // Fingers of Frost
            if (bot->HasAura(44544))
                state |= ClassState::FingersOfFrost;
            break;
        }

        case CLASS_WARLOCK:
        {
            // Nightfall
            if (bot->HasAura(18095))
                state |= ClassState::Nightfall;
            break;
        }

        case CLASS_HUNTER:
        {
            // Check ammo
            Item* ammo = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
            if (ammo)
                state |= ClassState::HasAmmo;

            // Pet happiness
            Pet* pet = bot->GetPet();
            if (pet && pet->GetHappinessState() == HAPPY)
                state |= ClassState::PetHappy;
            break;
        }

        case CLASS_DEATH_KNIGHT:
        {
            // Check presence
            if (bot->HasAura(48263))  // Blood Presence
                state |= ClassState::BloodPresence;
            else if (bot->HasAura(48266))  // Frost Presence
                state |= ClassState::FrostPresence;
            else if (bot->HasAura(48265))  // Unholy Presence
                state |= ClassState::UnholyPresence;
            break;
        }

        case CLASS_SHAMAN:
        {
            // TODO: Totem detection
            break;
        }

        default:
            break;
    }
}

void FSMEngine::UpdateEventState()
{
    // Events are transient - detected from state changes
    uint64_t& events = currentState.events;

    // Combat start/end
    bool wasCombat = previousState.primary & PrimaryState::InCombat;
    bool isCombat = currentState.primary & PrimaryState::InCombat;

    if (!wasCombat && isCombat)
        events |= EventState::CombatStart;
    if (wasCombat && !isCombat)
        events |= EventState::CombatEnd;

    // Target changed
    // Note: We can't easily detect this without storing previous target GUID
    // For now, skip this event detection

    // Timer tick (every second)
    uint32_t now = getMSTime();
    if (now - lastTimerTick >= 1000)
    {
        events |= EventState::TimerTick;
        lastTimerTick = now;
    }
}

// ============================================================================
// HELPER CALCULATIONS
// ============================================================================

uint8_t FSMEngine::CalculateHealthLevel(uint8_t healthPct) const
{
    if (healthPct < 20) return HealthLevel::Critical;
    if (healthPct < 30) return HealthLevel::VeryLow;
    if (healthPct < 40) return HealthLevel::Low;
    if (healthPct < 50) return HealthLevel::MedLow;
    if (healthPct < 60) return HealthLevel::Medium;
    if (healthPct < 75) return HealthLevel::MedHigh;
    if (healthPct < 85) return HealthLevel::High;
    return HealthLevel::Full;
}

uint8_t FSMEngine::CalculateManaLevel(uint8_t manaPct) const
{
    // Same scale as health for consistency
    if (manaPct < 20) return 0;
    if (manaPct < 30) return 1;
    if (manaPct < 40) return 2;
    if (manaPct < 50) return 3;
    if (manaPct < 60) return 4;
    if (manaPct < 75) return 5;
    if (manaPct < 85) return 6;
    return 7;
}

uint8_t FSMEngine::CalculateRangeBand(float distance) const
{
    if (distance < 0.5f)
        return RangeBand::Inside;
    if (distance <= NOMINAL_MELEE_RANGE)
        return RangeBand::Melee;
    if (distance <= sPlayerbotAIConfig->tooCloseDistance)
        return RangeBand::Close;
    if (distance <= sPlayerbotAIConfig->meleeDistance)
        return RangeBand::Mid;
    if (distance <= sPlayerbotAIConfig->spellDistance)
        return RangeBand::Spell;
    if (distance <= sPlayerbotAIConfig->sightDistance)
        return RangeBand::Far;
    return RangeBand::OutOfRange;
}

// ============================================================================
// ACTION SELECTION
// ============================================================================

ActionId FSMEngine::SelectBestAction() const
{
    ActionId bestAction = INVALID_ACTION;
    float bestPriority = 0.0f;
    uint32_t now = getMSTime();

    // Iterate through all active strategy tables
    for (const ActionTable* table : activeStrategies)
    {
        if (!table || !table->rules)
            continue;

        // Check each rule
        for (uint16_t i = 0; i < table->ruleCount; ++i)
        {
            const ActionDefinition& rule = table->rules[i];

            // Skip if lower priority than current best
            if (rule.basePriority <= bestPriority)
                continue;

            // Fast bitmask condition check
            if (!rule.condition.matches(currentState))
                continue;

            // Check cooldown
            auto cdIt = cooldowns.find(rule.id);
            if (cdIt != cooldowns.end() && now < cdIt->second)
                continue;

            // Check if action is registered and possible
            auto entryIt = actionRegistry.find(rule.id);
            if (entryIt != actionRegistry.end())
            {
                const ActionEntry& entry = entryIt->second;

                // Check isPossible
                if (entry.isPossible && !entry.isPossible(botAI, currentState))
                    continue;

                // Check isUseful
                if (entry.isUseful && !entry.isUseful(botAI, currentState))
                    continue;
            }

            // This is a valid candidate
            bestAction = rule.id;
            bestPriority = rule.basePriority;
        }
    }

    return bestAction;
}

// ============================================================================
// ACTION EXECUTION
// ============================================================================

bool FSMEngine::ExecuteActionInternal(ActionId id, const ActionDefinition* def)
{
    auto entryIt = actionRegistry.find(id);
    if (entryIt == actionRegistry.end())
        return false;

    const ActionEntry& entry = entryIt->second;

    if (!entry.executor)
        return false;

    // Execute the action
    bool success = entry.executor(botAI, currentState);

    // Record cooldown if specified
    if (def && def->cooldownMs > 0)
    {
        RecordCooldown(id, def->cooldownMs);
    }

    // On failure, try alternatives
    if (!success && def)
    {
        for (int i = 0; i < 4 && def->alternatives[i] != INVALID_ACTION; ++i)
        {
            if (ExecuteAction(def->alternatives[i]))
                return true;
        }
    }

    return success;
}

bool FSMEngine::ExecuteAction(ActionId id)
{
    auto entryIt = actionRegistry.find(id);
    if (entryIt == actionRegistry.end())
        return false;

    const ActionEntry& entry = entryIt->second;

    if (!entry.executor)
        return false;

    // Check isPossible
    if (entry.isPossible && !entry.isPossible(botAI, currentState))
        return false;

    // Execute
    return entry.executor(botAI, currentState);
}

// ============================================================================
// STRATEGY MANAGEMENT
// ============================================================================

void FSMEngine::AddStrategy(const ActionTable* table)
{
    if (!table)
        return;

    // Check if already added
    for (const ActionTable* t : activeStrategies)
    {
        if (t == table || t->strategyId == table->strategyId)
            return;
    }

    activeStrategies.push_back(table);
}

void FSMEngine::RemoveStrategy(uint16_t strategyId)
{
    activeStrategies.erase(
        std::remove_if(activeStrategies.begin(), activeStrategies.end(),
            [strategyId](const ActionTable* t) { return t && t->strategyId == strategyId; }),
        activeStrategies.end());
}

bool FSMEngine::HasStrategy(uint16_t strategyId) const
{
    for (const ActionTable* t : activeStrategies)
    {
        if (t && t->strategyId == strategyId)
            return true;
    }
    return false;
}

void FSMEngine::ClearStrategies()
{
    activeStrategies.clear();
}

// ============================================================================
// ACTION REGISTRY
// ============================================================================

void FSMEngine::RegisterAction(const ActionEntry& entry)
{
    actionRegistry[entry.id] = entry;
}

const ActionEntry* FSMEngine::GetActionEntry(ActionId id) const
{
    auto it = actionRegistry.find(id);
    return (it != actionRegistry.end()) ? &it->second : nullptr;
}

// ============================================================================
// COOLDOWN MANAGEMENT
// ============================================================================

void FSMEngine::RecordCooldown(ActionId id, uint32_t durationMs)
{
    cooldowns[id] = getMSTime() + durationMs;
}

bool FSMEngine::IsOnCooldown(ActionId id) const
{
    auto it = cooldowns.find(id);
    if (it == cooldowns.end())
        return false;
    return getMSTime() < it->second;
}
