/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "DruidExecutors.h"
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

// ============================================================================
// SHAPESHIFTS
// ============================================================================

static bool ExecuteCatForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("cat form", bot))
        return false;
    return botAI->CastSpell("cat form", bot);
}

static bool ExecuteBearForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("bear form", bot))
        return false;
    return botAI->CastSpell("bear form", bot);
}

static bool ExecuteDireBearForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("dire bear form", bot))
        return false;
    return botAI->CastSpell("dire bear form", bot);
}

static bool ExecuteMoonkinForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("moonkin form", bot))
        return false;
    return botAI->CastSpell("moonkin form", bot);
}

static bool ExecuteTreeOfLifeForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("tree of life", bot))
        return false;
    return botAI->CastSpell("tree of life", bot);
}

static bool ExecuteTravelForm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("travel form", bot))
        return false;
    return botAI->CastSpell("travel form", bot);
}

// ============================================================================
// BUFFS
// ============================================================================

static bool ExecuteMarkOfTheWild(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("mark of the wild", bot))
        return false;
    return botAI->CastSpell("mark of the wild", bot);
}

static bool ExecuteGiftOfTheWild(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("gift of the wild", bot))
        return false;
    return botAI->CastSpell("gift of the wild", bot);
}

static bool ExecuteThorns(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("thorns", bot))
        return false;
    return botAI->CastSpell("thorns", bot);
}

// ============================================================================
// BALANCE (MOONKIN) DAMAGE
// ============================================================================

static bool ExecuteWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("wrath", target))
        return false;
    return botAI->CastSpell("wrath", target);
}

static bool ExecuteStarfire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("starfire", target))
        return false;
    return botAI->CastSpell("starfire", target);
}

static bool ExecuteMoonfire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("moonfire", target))
        return false;
    return botAI->CastSpell("moonfire", target);
}

static bool ExecuteInsectSwarm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("insect swarm", target))
        return false;
    return botAI->CastSpell("insect swarm", target);
}

static bool ExecuteStarfall(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("starfall", bot))
        return false;
    return botAI->CastSpell("starfall", bot);
}

static bool ExecuteTyphoon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("typhoon", bot))
        return false;
    return botAI->CastSpell("typhoon", bot);
}

static bool ExecuteHurricane(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("hurricane", target))
        return false;
    return botAI->CastSpell("hurricane", target);
}

static bool ExecuteForceOfNature(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("force of nature", target))
        return false;
    return botAI->CastSpell("force of nature", target);
}

// ============================================================================
// FERAL CAT ABILITIES
// ============================================================================

static bool ExecuteMangleCat(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mangle (cat)", target))
        return false;
    return botAI->CastSpell("mangle (cat)", target);
}

static bool ExecuteShred(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shred", target))
        return false;
    return botAI->CastSpell("shred", target);
}

static bool ExecuteRake(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("rake", target))
        return false;
    return botAI->CastSpell("rake", target);
}

static bool ExecuteRip(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("rip", target))
        return false;
    return botAI->CastSpell("rip", target);
}

static bool ExecuteFerociousBite(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("ferocious bite", target))
        return false;
    return botAI->CastSpell("ferocious bite", target);
}

static bool ExecuteSavageRoar(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("savage roar", bot))
        return false;
    return botAI->CastSpell("savage roar", bot);
}

static bool ExecuteTigersFury(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("tiger's fury", bot))
        return false;
    return botAI->CastSpell("tiger's fury", bot);
}

static bool ExecuteRavage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("ravage", target))
        return false;
    return botAI->CastSpell("ravage", target);
}

static bool ExecutePounce(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("pounce", target))
        return false;
    return botAI->CastSpell("pounce", target);
}

static bool ExecuteProwl(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("prowl", bot))
        return false;
    return botAI->CastSpell("prowl", bot);
}

// ============================================================================
// FERAL BEAR ABILITIES
// ============================================================================

static bool ExecuteMangleBear(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mangle (bear)", target))
        return false;
    return botAI->CastSpell("mangle (bear)", target);
}

static bool ExecuteMaul(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("maul", target))
        return false;
    return botAI->CastSpell("maul", target);
}

static bool ExecuteSwipe(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("swipe (bear)", target))
        return false;
    return botAI->CastSpell("swipe (bear)", target);
}

static bool ExecuteLacerate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("lacerate", target))
        return false;
    return botAI->CastSpell("lacerate", target);
}

static bool ExecuteFaerieFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("faerie fire (feral)", target))
        return false;
    return botAI->CastSpell("faerie fire (feral)", target);
}

static bool ExecuteDemoralizingRoar(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("demoralizing roar", bot))
        return false;
    return botAI->CastSpell("demoralizing roar", bot);
}

static bool ExecuteBash(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("bash", target))
        return false;
    return botAI->CastSpell("bash", target);
}

static bool ExecuteGrowl(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("growl", target))
        return false;
    return botAI->CastSpell("growl", target);
}

static bool ExecuteChallengingRoar(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("challenging roar", bot))
        return false;
    return botAI->CastSpell("challenging roar", bot);
}

static bool ExecuteFrenziedRegeneration(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("frenzied regeneration", bot))
        return false;
    return botAI->CastSpell("frenzied regeneration", bot);
}

static bool ExecuteSurvivalInstincts(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("survival instincts", bot))
        return false;
    return botAI->CastSpell("survival instincts", bot);
}

static bool ExecuteBarkskin(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("barkskin", bot))
        return false;
    return botAI->CastSpell("barkskin", bot);
}

static bool ExecuteEnrage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("enrage", bot))
        return false;
    return botAI->CastSpell("enrage", bot);
}

static bool ExecuteBerserk(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("berserk", bot))
        return false;
    return botAI->CastSpell("berserk", bot);
}

// ============================================================================
// RESTORATION HEALS
// ============================================================================

static bool ExecuteHealingTouch(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("healing touch", bot))
        return false;
    return botAI->CastSpell("healing touch", bot);
}

static bool ExecuteRegrowth(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("regrowth", bot))
        return false;
    return botAI->CastSpell("regrowth", bot);
}

static bool ExecuteRejuvenation(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("rejuvenation", bot))
        return false;
    return botAI->CastSpell("rejuvenation", bot);
}

static bool ExecuteLifebloom(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("lifebloom", bot))
        return false;
    return botAI->CastSpell("lifebloom", bot);
}

static bool ExecuteNourish(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("nourish", bot))
        return false;
    return botAI->CastSpell("nourish", bot);
}

static bool ExecuteWildGrowth(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("wild growth", bot))
        return false;
    return botAI->CastSpell("wild growth", bot);
}

static bool ExecuteSwiftmend(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("swiftmend", bot))
        return false;
    return botAI->CastSpell("swiftmend", bot);
}

static bool ExecuteTranquility(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("tranquility", bot))
        return false;
    return botAI->CastSpell("tranquility", bot);
}

static bool ExecuteNaturesSwiftness(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("nature's swiftness", bot))
        return false;
    return botAI->CastSpell("nature's swiftness", bot);
}

// ============================================================================
// UTILITY
// ============================================================================

static bool ExecuteInnervate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("innervate", bot))
        return false;
    return botAI->CastSpell("innervate", bot);
}

static bool ExecuteRebirth(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Needs dead party member targeting
    return false;
}

static bool ExecuteRemoveCurse(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("remove curse", bot))
        return false;
    return botAI->CastSpell("remove curse", bot);
}

static bool ExecuteAbolishPoison(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("abolish poison", bot))
        return false;
    return botAI->CastSpell("abolish poison", bot);
}

static bool ExecuteHibernate(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("hibernate", target))
        return false;
    return botAI->CastSpell("hibernate", target);
}

static bool ExecuteEntanglingRoots(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("entangling roots", target))
        return false;
    return botAI->CastSpell("entangling roots", target);
}

static bool ExecuteCyclone(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("cyclone", target))
        return false;
    return botAI->CastSpell("cyclone", target);
}

static bool ExecuteDash(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("dash", bot))
        return false;
    return botAI->CastSpell("dash", bot);
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterDruidActions(FSMEngine* engine)
{
    // Basic actions
    engine->RegisterAction({DruidActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({DruidActions::Follow, "follow", ExecuteFollow});

    // Shapeshifts
    engine->RegisterAction({DruidActions::CatForm, "cat form", ExecuteCatForm});
    engine->RegisterAction({DruidActions::BearForm, "bear form", ExecuteBearForm});
    engine->RegisterAction({DruidActions::DireBearForm, "dire bear form", ExecuteDireBearForm});
    engine->RegisterAction({DruidActions::MoonkinForm, "moonkin form", ExecuteMoonkinForm});
    engine->RegisterAction({DruidActions::TreeOfLifeForm, "tree of life", ExecuteTreeOfLifeForm});
    engine->RegisterAction({DruidActions::TravelForm, "travel form", ExecuteTravelForm});

    // Buffs
    engine->RegisterAction({DruidActions::MarkOfTheWild, "mark of the wild", ExecuteMarkOfTheWild});
    engine->RegisterAction({DruidActions::GiftOfTheWild, "gift of the wild", ExecuteGiftOfTheWild});
    engine->RegisterAction({DruidActions::Thorns, "thorns", ExecuteThorns});

    // Balance (Moonkin) damage
    engine->RegisterAction({DruidActions::Wrath, "wrath", ExecuteWrath});
    engine->RegisterAction({DruidActions::Starfire, "starfire", ExecuteStarfire});
    engine->RegisterAction({DruidActions::Moonfire, "moonfire", ExecuteMoonfire});
    engine->RegisterAction({DruidActions::InsectSwarm, "insect swarm", ExecuteInsectSwarm});
    engine->RegisterAction({DruidActions::Starfall, "starfall", ExecuteStarfall});
    engine->RegisterAction({DruidActions::Typhoon, "typhoon", ExecuteTyphoon});
    engine->RegisterAction({DruidActions::Hurricane, "hurricane", ExecuteHurricane});
    engine->RegisterAction({DruidActions::ForceOfNature, "force of nature", ExecuteForceOfNature});

    // Feral Cat abilities
    engine->RegisterAction({DruidActions::MangleCat, "mangle (cat)", ExecuteMangleCat});
    engine->RegisterAction({DruidActions::Shred, "shred", ExecuteShred});
    engine->RegisterAction({DruidActions::Rake, "rake", ExecuteRake});
    engine->RegisterAction({DruidActions::Rip, "rip", ExecuteRip});
    engine->RegisterAction({DruidActions::FerociousBite, "ferocious bite", ExecuteFerociousBite});
    engine->RegisterAction({DruidActions::SavageRoar, "savage roar", ExecuteSavageRoar});
    engine->RegisterAction({DruidActions::TigersFury, "tiger's fury", ExecuteTigersFury});
    engine->RegisterAction({DruidActions::Ravage, "ravage", ExecuteRavage});
    engine->RegisterAction({DruidActions::Pounce, "pounce", ExecutePounce});
    engine->RegisterAction({DruidActions::Prowl, "prowl", ExecuteProwl});

    // Feral Bear abilities
    engine->RegisterAction({DruidActions::MangleBear, "mangle (bear)", ExecuteMangleBear});
    engine->RegisterAction({DruidActions::Maul, "maul", ExecuteMaul});
    engine->RegisterAction({DruidActions::Swipe, "swipe (bear)", ExecuteSwipe});
    engine->RegisterAction({DruidActions::Lacerate, "lacerate", ExecuteLacerate});
    engine->RegisterAction({DruidActions::FaerieFire, "faerie fire (feral)", ExecuteFaerieFire});
    engine->RegisterAction({DruidActions::DemoralizingRoar, "demoralizing roar", ExecuteDemoralizingRoar});
    engine->RegisterAction({DruidActions::Bash, "bash", ExecuteBash});
    engine->RegisterAction({DruidActions::Growl, "growl", ExecuteGrowl});
    engine->RegisterAction({DruidActions::ChallengingRoar, "challenging roar", ExecuteChallengingRoar});
    engine->RegisterAction({DruidActions::FrenziedRegeneration, "frenzied regeneration", ExecuteFrenziedRegeneration});
    engine->RegisterAction({DruidActions::SurvivalInstincts, "survival instincts", ExecuteSurvivalInstincts});
    engine->RegisterAction({DruidActions::Barkskin, "barkskin", ExecuteBarkskin});
    engine->RegisterAction({DruidActions::Enrage, "enrage", ExecuteEnrage});
    engine->RegisterAction({DruidActions::Berserk, "berserk", ExecuteBerserk});

    // Restoration heals
    engine->RegisterAction({DruidActions::HealingTouch, "healing touch", ExecuteHealingTouch});
    engine->RegisterAction({DruidActions::Regrowth, "regrowth", ExecuteRegrowth});
    engine->RegisterAction({DruidActions::Rejuvenation, "rejuvenation", ExecuteRejuvenation});
    engine->RegisterAction({DruidActions::Lifebloom, "lifebloom", ExecuteLifebloom});
    engine->RegisterAction({DruidActions::Nourish, "nourish", ExecuteNourish});
    engine->RegisterAction({DruidActions::WildGrowth, "wild growth", ExecuteWildGrowth});
    engine->RegisterAction({DruidActions::Swiftmend, "swiftmend", ExecuteSwiftmend});
    engine->RegisterAction({DruidActions::Tranquility, "tranquility", ExecuteTranquility});
    engine->RegisterAction({DruidActions::NaturesSwiftness, "nature's swiftness", ExecuteNaturesSwiftness});

    // Utility
    engine->RegisterAction({DruidActions::Innervate, "innervate", ExecuteInnervate});
    engine->RegisterAction({DruidActions::Rebirth, "rebirth", ExecuteRebirth});
    engine->RegisterAction({DruidActions::RemoveCurse, "remove curse", ExecuteRemoveCurse});
    engine->RegisterAction({DruidActions::AbolishPoison, "abolish poison", ExecuteAbolishPoison});
    engine->RegisterAction({DruidActions::Hibernate, "hibernate", ExecuteHibernate});
    engine->RegisterAction({DruidActions::EntanglingRoots, "entangling roots", ExecuteEntanglingRoots});
    engine->RegisterAction({DruidActions::Cyclone, "cyclone", ExecuteCyclone});
    engine->RegisterAction({DruidActions::Dash, "dash", ExecuteDash});
}
