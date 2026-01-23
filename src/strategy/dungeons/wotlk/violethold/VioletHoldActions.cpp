#include "Playerbots.h"
#include "VioletHoldActions.h"
#include "VioletHoldStrategy.h"

bool AttackErekemAction::Execute(Event event)
{
    // Focus boss first, adds after
    Unit* boss = AI_VALUE2(Unit*, "find target", "erekem");
    if (!boss) { return false; }

    if (GET_CURRENT_TARGET() != boss)
        {
            return Attack(boss);
        }
    return false;
}

bool AttackIchorGlobuleAction::Execute(Event event)
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "ichoron");
    if (!boss) { return false; }

    Unit* currentTarget = GET_CURRENT_TARGET();

    // Tank prioritise boss if it's up
    if (botAI->IsTank(bot) && !boss->HasAura(SPELL_DRAINED))
    {
        if (GET_CURRENT_TARGET() != boss)
        {
            return Attack(boss);
        }
        return false;
    }

    // Target is not findable from threat table using AI_VALUE2(),
    // therefore need to search manually for the unit name
    GuidVector targets = GET_POSSIBLE_TARGETS();

    for (auto i = targets.begin(); i != targets.end(); ++i)
    {
        Unit* unit = botAI->GetUnit(*i);
        if (unit && unit->GetEntry() == NPC_ICHOR_GLOBULE)
        {
            // Check IDs here, NOT Unit* pointers:
            // Don't keep swapping between sentries.
            // If we're already attacking one, don't retarget another
            if (currentTarget && currentTarget->GetEntry() == NPC_ICHOR_GLOBULE)
            {
                return false;
            }
            return Attack(unit);
        }
    }
    // No ichor globules left alive, fall back to targeting boss
    if (currentTarget != boss)
    {
        return Attack(boss);
    }

    return false;
}

bool AttackVoidSentryAction::Execute(Event event)
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "zuramat the obliterator");
    if (!boss) { return false; }

    Unit* currentTarget = GET_CURRENT_TARGET();

    // Target is not findable from threat table using AI_VALUE2(),
    // therefore need to search manually for the unit name
    // GuidVector targets = GET_POSSIBLE_TARGETS_NO_LOS();
    GuidVector targets = GET_POSSIBLE_TARGETS_NO_LOS();

    for (auto i = targets.begin(); i != targets.end(); ++i)
    {
        Unit* unit = botAI->GetUnit(*i);
        if (unit && unit->GetEntry() == NPC_VOID_SENTRY)
        {
            // Check IDs here, NOT Unit* pointers:
            // Don't keep swapping between sentries.
            // If we're already attacking one, don't retarget another
            if (currentTarget && currentTarget->GetEntry() == NPC_VOID_SENTRY)
            {
                return false;
            }
            return Attack(unit);
        }
    }
    // No void sentries left alive, fall back to targeting boss
    if (currentTarget != boss)
    {
        return Attack(boss);
    }

    return false;
}

bool StopAttackAction::Execute(Event event)
{
    return bot->AttackStop();
}
