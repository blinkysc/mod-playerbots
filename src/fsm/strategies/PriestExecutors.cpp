/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "PriestExecutors.h"
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
// BUFFS
// ============================================================================

static bool ExecutePowerWordFortitude(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("power word: fortitude", bot))
        return false;
    return botAI->CastSpell("power word: fortitude", bot);
}

static bool ExecuteDivineSpirit(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("divine spirit", bot))
        return false;
    return botAI->CastSpell("divine spirit", bot);
}

static bool ExecuteInnerFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("inner fire", bot))
        return false;
    return botAI->CastSpell("inner fire", bot);
}

static bool ExecuteShadowProtection(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("shadow protection", bot))
        return false;
    return botAI->CastSpell("shadow protection", bot);
}

static bool ExecuteFearWard(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("fear ward", bot))
        return false;
    return botAI->CastSpell("fear ward", bot);
}

static bool ExecuteShadowform(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("shadowform", bot))
        return false;
    return botAI->CastSpell("shadowform", bot);
}

// ============================================================================
// HOLY HEALS
// ============================================================================

static bool ExecuteHeal(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Target lowest health party member or self
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("heal", bot))
        return false;
    return botAI->CastSpell("heal", bot);
}

static bool ExecuteFlashHeal(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("flash heal", bot))
        return false;
    return botAI->CastSpell("flash heal", bot);
}

static bool ExecuteGreaterHeal(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("greater heal", bot))
        return false;
    return botAI->CastSpell("greater heal", bot);
}

static bool ExecuteRenew(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("renew", bot))
        return false;
    return botAI->CastSpell("renew", bot);
}

static bool ExecuteHolyNova(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("holy nova", bot))
        return false;
    return botAI->CastSpell("holy nova", bot);
}

static bool ExecuteBindingHeal(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("binding heal", bot))
        return false;
    return botAI->CastSpell("binding heal", bot);
}

static bool ExecuteCircleOfHealing(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("circle of healing", bot))
        return false;
    return botAI->CastSpell("circle of healing", bot);
}

static bool ExecutePrayerOfHealing(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("prayer of healing", bot))
        return false;
    return botAI->CastSpell("prayer of healing", bot);
}

static bool ExecutePrayerOfMending(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("prayer of mending", bot))
        return false;
    return botAI->CastSpell("prayer of mending", bot);
}

static bool ExecuteDivineHymn(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("divine hymn", bot))
        return false;
    return botAI->CastSpell("divine hymn", bot);
}

static bool ExecuteLightwell(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("lightwell", bot))
        return false;
    return botAI->CastSpell("lightwell", bot);
}

static bool ExecuteGuardianSpirit(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("guardian spirit", bot))
        return false;
    return botAI->CastSpell("guardian spirit", bot);
}

// ============================================================================
// DISCIPLINE
// ============================================================================

static bool ExecutePowerWordShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("power word: shield", bot))
        return false;
    return botAI->CastSpell("power word: shield", bot);
}

static bool ExecutePenance(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("penance", target))
        return false;
    return botAI->CastSpell("penance", target);
}

static bool ExecuteInnerFocus(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("inner focus", bot))
        return false;
    return botAI->CastSpell("inner focus", bot);
}

static bool ExecutePowerInfusion(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("power infusion", bot))
        return false;
    return botAI->CastSpell("power infusion", bot);
}

static bool ExecutePainSuppression(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("pain suppression", bot))
        return false;
    return botAI->CastSpell("pain suppression", bot);
}

// ============================================================================
// SHADOW DAMAGE
// ============================================================================

static bool ExecuteMindBlast(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mind blast", target))
        return false;
    return botAI->CastSpell("mind blast", target);
}

static bool ExecuteShadowWordPain(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadow word: pain", target))
        return false;
    return botAI->CastSpell("shadow word: pain", target);
}

static bool ExecuteShadowWordDeath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadow word: death", target))
        return false;
    return botAI->CastSpell("shadow word: death", target);
}

static bool ExecuteMindFlay(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mind flay", target))
        return false;
    return botAI->CastSpell("mind flay", target);
}

static bool ExecuteVampiricTouch(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("vampiric touch", target))
        return false;
    return botAI->CastSpell("vampiric touch", target);
}

static bool ExecuteVampiricEmbrace(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("vampiric embrace", bot))
        return false;
    return botAI->CastSpell("vampiric embrace", bot);
}

static bool ExecuteDevouringPlague(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("devouring plague", target))
        return false;
    return botAI->CastSpell("devouring plague", target);
}

static bool ExecuteMindSear(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mind sear", target))
        return false;
    return botAI->CastSpell("mind sear", target);
}

static bool ExecuteDispersion(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("dispersion", bot))
        return false;
    return botAI->CastSpell("dispersion", bot);
}

static bool ExecuteShadowfiend(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shadowfiend", target))
        return false;
    return botAI->CastSpell("shadowfiend", target);
}

// ============================================================================
// UTILITY
// ============================================================================

static bool ExecuteDispelMagic(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("dispel magic", target))
        return false;
    return botAI->CastSpell("dispel magic", target);
}

static bool ExecuteMassDispel(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mass dispel", target))
        return false;
    return botAI->CastSpell("mass dispel", target);
}

static bool ExecuteShackleUndead(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("shackle undead", target))
        return false;
    return botAI->CastSpell("shackle undead", target);
}

static bool ExecutePsychicScream(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("psychic scream", bot))
        return false;
    return botAI->CastSpell("psychic scream", bot);
}

static bool ExecuteFade(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("fade", bot))
        return false;
    return botAI->CastSpell("fade", bot);
}

static bool ExecuteMindControl(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("mind control", target))
        return false;
    return botAI->CastSpell("mind control", target);
}

static bool ExecuteSmite(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("smite", target))
        return false;
    return botAI->CastSpell("smite", target);
}

static bool ExecuteHolyFire(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("holy fire", target))
        return false;
    return botAI->CastSpell("holy fire", target);
}

static bool ExecuteResurrection(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Needs dead party member targeting
    return false;
}

static bool ExecuteAbolishDisease(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("abolish disease", bot))
        return false;
    return botAI->CastSpell("abolish disease", bot);
}

static bool ExecuteCureDisease(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("cure disease", bot))
        return false;
    return botAI->CastSpell("cure disease", bot);
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterPriestActions(FSMEngine* engine)
{
    // Basic actions
    engine->RegisterAction({PriestActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({PriestActions::Follow, "follow", ExecuteFollow});

    // Buffs
    engine->RegisterAction({PriestActions::PowerWordFortitude, "power word: fortitude", ExecutePowerWordFortitude});
    engine->RegisterAction({PriestActions::DivineSpirit, "divine spirit", ExecuteDivineSpirit});
    engine->RegisterAction({PriestActions::InnerFire, "inner fire", ExecuteInnerFire});
    engine->RegisterAction({PriestActions::ShadowProtection, "shadow protection", ExecuteShadowProtection});
    engine->RegisterAction({PriestActions::FearWard, "fear ward", ExecuteFearWard});
    engine->RegisterAction({PriestActions::Shadowform, "shadowform", ExecuteShadowform});

    // Holy heals
    engine->RegisterAction({PriestActions::Heal, "heal", ExecuteHeal});
    engine->RegisterAction({PriestActions::FlashHeal, "flash heal", ExecuteFlashHeal});
    engine->RegisterAction({PriestActions::GreaterHeal, "greater heal", ExecuteGreaterHeal});
    engine->RegisterAction({PriestActions::Renew, "renew", ExecuteRenew});
    engine->RegisterAction({PriestActions::HolyNova, "holy nova", ExecuteHolyNova});
    engine->RegisterAction({PriestActions::BindingHeal, "binding heal", ExecuteBindingHeal});
    engine->RegisterAction({PriestActions::CircleOfHealing, "circle of healing", ExecuteCircleOfHealing});
    engine->RegisterAction({PriestActions::PrayerOfHealing, "prayer of healing", ExecutePrayerOfHealing});
    engine->RegisterAction({PriestActions::PrayerOfMending, "prayer of mending", ExecutePrayerOfMending});
    engine->RegisterAction({PriestActions::DivineHymn, "divine hymn", ExecuteDivineHymn});
    engine->RegisterAction({PriestActions::Lightwell, "lightwell", ExecuteLightwell});
    engine->RegisterAction({PriestActions::GuardianSpirit, "guardian spirit", ExecuteGuardianSpirit});

    // Discipline
    engine->RegisterAction({PriestActions::PowerWordShield, "power word: shield", ExecutePowerWordShield});
    engine->RegisterAction({PriestActions::Penance, "penance", ExecutePenance});
    engine->RegisterAction({PriestActions::InnerFocus, "inner focus", ExecuteInnerFocus});
    engine->RegisterAction({PriestActions::PowerInfusion, "power infusion", ExecutePowerInfusion});
    engine->RegisterAction({PriestActions::PainSuppression, "pain suppression", ExecutePainSuppression});

    // Shadow damage
    engine->RegisterAction({PriestActions::MindBlast, "mind blast", ExecuteMindBlast});
    engine->RegisterAction({PriestActions::ShadowWordPain, "shadow word: pain", ExecuteShadowWordPain});
    engine->RegisterAction({PriestActions::ShadowWordDeath, "shadow word: death", ExecuteShadowWordDeath});
    engine->RegisterAction({PriestActions::MindFlay, "mind flay", ExecuteMindFlay});
    engine->RegisterAction({PriestActions::VampiricTouch, "vampiric touch", ExecuteVampiricTouch});
    engine->RegisterAction({PriestActions::VampiricEmbrace, "vampiric embrace", ExecuteVampiricEmbrace});
    engine->RegisterAction({PriestActions::DevouringPlague, "devouring plague", ExecuteDevouringPlague});
    engine->RegisterAction({PriestActions::MindSear, "mind sear", ExecuteMindSear});
    engine->RegisterAction({PriestActions::Dispersion, "dispersion", ExecuteDispersion});
    engine->RegisterAction({PriestActions::Shadowfiend, "shadowfiend", ExecuteShadowfiend});

    // Utility
    engine->RegisterAction({PriestActions::DispelMagic, "dispel magic", ExecuteDispelMagic});
    engine->RegisterAction({PriestActions::MassDispel, "mass dispel", ExecuteMassDispel});
    engine->RegisterAction({PriestActions::ShackleUndead, "shackle undead", ExecuteShackleUndead});
    engine->RegisterAction({PriestActions::PsychicScream, "psychic scream", ExecutePsychicScream});
    engine->RegisterAction({PriestActions::Fade, "fade", ExecuteFade});
    engine->RegisterAction({PriestActions::MindControl, "mind control", ExecuteMindControl});
    engine->RegisterAction({PriestActions::Smite, "smite", ExecuteSmite});
    engine->RegisterAction({PriestActions::HolyFire, "holy fire", ExecuteHolyFire});
    engine->RegisterAction({PriestActions::Resurrection, "resurrection", ExecuteResurrection});
    engine->RegisterAction({PriestActions::AbolishDisease, "abolish disease", ExecuteAbolishDisease});
    engine->RegisterAction({PriestActions::CureDisease, "cure disease", ExecuteCureDisease});
}
