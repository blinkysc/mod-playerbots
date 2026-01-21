/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#include "ShamanExecutors.h"
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
// TOTEMS
// ============================================================================

static bool ExecuteStrengthOfEarthTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("strength of earth totem", bot))
        return false;
    return botAI->CastSpell("strength of earth totem", bot);
}

static bool ExecuteStoneskinTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("stoneskin totem", bot))
        return false;
    return botAI->CastSpell("stoneskin totem", bot);
}

static bool ExecuteTremorTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("tremor totem", bot))
        return false;
    return botAI->CastSpell("tremor totem", bot);
}

static bool ExecuteEarthbindTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("earthbind totem", bot))
        return false;
    return botAI->CastSpell("earthbind totem", bot);
}

static bool ExecuteSearingTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("searing totem", bot))
        return false;
    return botAI->CastSpell("searing totem", bot);
}

static bool ExecuteMagmaTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("magma totem", bot))
        return false;
    return botAI->CastSpell("magma totem", bot);
}

static bool ExecuteFlametongueTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("flametongue totem", bot))
        return false;
    return botAI->CastSpell("flametongue totem", bot);
}

static bool ExecuteHealingStreamTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("healing stream totem", bot))
        return false;
    return botAI->CastSpell("healing stream totem", bot);
}

static bool ExecuteManaSpringTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("mana spring totem", bot))
        return false;
    return botAI->CastSpell("mana spring totem", bot);
}

static bool ExecuteTotemOfWrath(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("totem of wrath", bot))
        return false;
    return botAI->CastSpell("totem of wrath", bot);
}

static bool ExecuteWindfuryTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("windfury totem", bot))
        return false;
    return botAI->CastSpell("windfury totem", bot);
}

static bool ExecuteWrathOfAirTotem(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("wrath of air totem", bot))
        return false;
    return botAI->CastSpell("wrath of air totem", bot);
}

// ============================================================================
// BUFFS
// ============================================================================

static bool ExecuteLightningShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("lightning shield", bot))
        return false;
    return botAI->CastSpell("lightning shield", bot);
}

static bool ExecuteWaterShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("water shield", bot))
        return false;
    return botAI->CastSpell("water shield", bot);
}

static bool ExecuteEarthShield(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("earth shield", bot))
        return false;
    return botAI->CastSpell("earth shield", bot);
}

static bool ExecuteBloodlust(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("bloodlust", bot))
        return false;
    return botAI->CastSpell("bloodlust", bot);
}

static bool ExecuteHeroism(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("heroism", bot))
        return false;
    return botAI->CastSpell("heroism", bot);
}

// ============================================================================
// ELEMENTAL DAMAGE
// ============================================================================

static bool ExecuteLightningBolt(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("lightning bolt", target))
        return false;
    return botAI->CastSpell("lightning bolt", target);
}

static bool ExecuteChainLightning(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("chain lightning", target))
        return false;
    return botAI->CastSpell("chain lightning", target);
}

static bool ExecuteLavaBurst(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("lava burst", target))
        return false;
    return botAI->CastSpell("lava burst", target);
}

static bool ExecuteFlameShock(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("flame shock", target))
        return false;
    return botAI->CastSpell("flame shock", target);
}

static bool ExecuteFrostShock(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("frost shock", target))
        return false;
    return botAI->CastSpell("frost shock", target);
}

static bool ExecuteEarthShock(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("earth shock", target))
        return false;
    return botAI->CastSpell("earth shock", target);
}

static bool ExecuteThunderstorm(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("thunderstorm", bot))
        return false;
    return botAI->CastSpell("thunderstorm", bot);
}

static bool ExecuteFireNova(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("fire nova", bot))
        return false;
    return botAI->CastSpell("fire nova", bot);
}

// ============================================================================
// ENHANCEMENT ABILITIES
// ============================================================================

static bool ExecuteStormstrike(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("stormstrike", target))
        return false;
    return botAI->CastSpell("stormstrike", target);
}

static bool ExecuteLavaLash(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("lava lash", target))
        return false;
    return botAI->CastSpell("lava lash", target);
}

static bool ExecuteFeralSpirit(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("feral spirit", bot))
        return false;
    return botAI->CastSpell("feral spirit", bot);
}

static bool ExecuteShamanisticRage(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("shamanistic rage", bot))
        return false;
    return botAI->CastSpell("shamanistic rage", bot);
}

// ============================================================================
// WEAPON ENHANCEMENTS
// ============================================================================

static bool ExecuteWindfuryWeapon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("windfury weapon", bot))
        return false;
    return botAI->CastSpell("windfury weapon", bot);
}

static bool ExecuteFlametongueWeapon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("flametongue weapon", bot))
        return false;
    return botAI->CastSpell("flametongue weapon", bot);
}

static bool ExecuteEarthlivingWeapon(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("earthliving weapon", bot))
        return false;
    return botAI->CastSpell("earthliving weapon", bot);
}

// ============================================================================
// RESTORATION HEALS
// ============================================================================

static bool ExecuteHealingWave(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("healing wave", bot))
        return false;
    return botAI->CastSpell("healing wave", bot);
}

static bool ExecuteLesserHealingWave(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("lesser healing wave", bot))
        return false;
    return botAI->CastSpell("lesser healing wave", bot);
}

static bool ExecuteChainHeal(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("chain heal", bot))
        return false;
    return botAI->CastSpell("chain heal", bot);
}

static bool ExecuteRiptide(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("riptide", bot))
        return false;
    return botAI->CastSpell("riptide", bot);
}

static bool ExecuteTidalForce(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("tidal force", bot))
        return false;
    return botAI->CastSpell("tidal force", bot);
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

static bool ExecutePurge(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("purge", target))
        return false;
    return botAI->CastSpell("purge", target);
}

static bool ExecuteWindShear(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("wind shear", target))
        return false;
    return botAI->CastSpell("wind shear", target);
}

static bool ExecuteHex(PlayerbotAI* botAI, const BotStateVector& state)
{
    Unit* target = GET_CURRENT_TARGET();
    if (!target)
        return false;
    if (!botAI->CanCastSpell("hex", target))
        return false;
    return botAI->CastSpell("hex", target);
}

static bool ExecuteGhostWolf(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("ghost wolf", bot))
        return false;
    return botAI->CastSpell("ghost wolf", bot);
}

static bool ExecuteAncestralSpirit(PlayerbotAI* botAI, const BotStateVector& state)
{
    // Needs dead party member targeting
    return false;
}

static bool ExecuteCleanseSpirit(PlayerbotAI* botAI, const BotStateVector& state)
{
    Player* bot = botAI->GetBot();
    if (!botAI->CanCastSpell("cleanse spirit", bot))
        return false;
    return botAI->CastSpell("cleanse spirit", bot);
}

// ============================================================================
// REGISTRATION
// ============================================================================

void RegisterShamanActions(FSMEngine* engine)
{
    // Basic actions
    engine->RegisterAction({ShamanActions::Melee, "melee", ExecuteMelee});
    engine->RegisterAction({ShamanActions::Follow, "follow", ExecuteFollow});

    // Totems
    engine->RegisterAction({ShamanActions::StrengthOfEarthTotem, "strength of earth totem", ExecuteStrengthOfEarthTotem});
    engine->RegisterAction({ShamanActions::StoneskinTotem, "stoneskin totem", ExecuteStoneskinTotem});
    engine->RegisterAction({ShamanActions::TremorTotem, "tremor totem", ExecuteTremorTotem});
    engine->RegisterAction({ShamanActions::EarthbindTotem, "earthbind totem", ExecuteEarthbindTotem});
    engine->RegisterAction({ShamanActions::SearingTotem, "searing totem", ExecuteSearingTotem});
    engine->RegisterAction({ShamanActions::MagmaTotem, "magma totem", ExecuteMagmaTotem});
    engine->RegisterAction({ShamanActions::FlametongueTotem, "flametongue totem", ExecuteFlametongueTotem});
    engine->RegisterAction({ShamanActions::HealingStreamTotem, "healing stream totem", ExecuteHealingStreamTotem});
    engine->RegisterAction({ShamanActions::ManaSpringTotem, "mana spring totem", ExecuteManaSpringTotem});
    engine->RegisterAction({ShamanActions::TotemOfWrath, "totem of wrath", ExecuteTotemOfWrath});
    engine->RegisterAction({ShamanActions::WindfuryTotem, "windfury totem", ExecuteWindfuryTotem});
    engine->RegisterAction({ShamanActions::WrathOfAirTotem, "wrath of air totem", ExecuteWrathOfAirTotem});

    // Buffs
    engine->RegisterAction({ShamanActions::LightningShield, "lightning shield", ExecuteLightningShield});
    engine->RegisterAction({ShamanActions::WaterShield, "water shield", ExecuteWaterShield});
    engine->RegisterAction({ShamanActions::EarthShield, "earth shield", ExecuteEarthShield});
    engine->RegisterAction({ShamanActions::Bloodlust, "bloodlust", ExecuteBloodlust});
    engine->RegisterAction({ShamanActions::Heroism, "heroism", ExecuteHeroism});

    // Elemental damage
    engine->RegisterAction({ShamanActions::LightningBolt, "lightning bolt", ExecuteLightningBolt});
    engine->RegisterAction({ShamanActions::ChainLightning, "chain lightning", ExecuteChainLightning});
    engine->RegisterAction({ShamanActions::LavaBurst, "lava burst", ExecuteLavaBurst});
    engine->RegisterAction({ShamanActions::FlameShock, "flame shock", ExecuteFlameShock});
    engine->RegisterAction({ShamanActions::FrostShock, "frost shock", ExecuteFrostShock});
    engine->RegisterAction({ShamanActions::EarthShock, "earth shock", ExecuteEarthShock});
    engine->RegisterAction({ShamanActions::Thunderstorm, "thunderstorm", ExecuteThunderstorm});
    engine->RegisterAction({ShamanActions::FireNova, "fire nova", ExecuteFireNova});

    // Enhancement abilities
    engine->RegisterAction({ShamanActions::Stormstrike, "stormstrike", ExecuteStormstrike});
    engine->RegisterAction({ShamanActions::LavaLash, "lava lash", ExecuteLavaLash});
    engine->RegisterAction({ShamanActions::FeralSpirit, "feral spirit", ExecuteFeralSpirit});
    engine->RegisterAction({ShamanActions::ShamanisticRage, "shamanistic rage", ExecuteShamanisticRage});

    // Weapon enhancements
    engine->RegisterAction({ShamanActions::WindfuryWeapon, "windfury weapon", ExecuteWindfuryWeapon});
    engine->RegisterAction({ShamanActions::FlametongueWeapon, "flametongue weapon", ExecuteFlametongueWeapon});
    engine->RegisterAction({ShamanActions::EarthlivingWeapon, "earthliving weapon", ExecuteEarthlivingWeapon});

    // Restoration heals
    engine->RegisterAction({ShamanActions::HealingWave, "healing wave", ExecuteHealingWave});
    engine->RegisterAction({ShamanActions::LesserHealingWave, "lesser healing wave", ExecuteLesserHealingWave});
    engine->RegisterAction({ShamanActions::ChainHeal, "chain heal", ExecuteChainHeal});
    engine->RegisterAction({ShamanActions::Riptide, "riptide", ExecuteRiptide});
    engine->RegisterAction({ShamanActions::TidalForce, "tidal force", ExecuteTidalForce});
    engine->RegisterAction({ShamanActions::NaturesSwiftness, "nature's swiftness", ExecuteNaturesSwiftness});

    // Utility
    engine->RegisterAction({ShamanActions::PurgeAction, "purge", ExecutePurge});
    engine->RegisterAction({ShamanActions::WindShear, "wind shear", ExecuteWindShear});
    engine->RegisterAction({ShamanActions::Hex, "hex", ExecuteHex});
    engine->RegisterAction({ShamanActions::GhostWolf, "ghost wolf", ExecuteGhostWolf});
    engine->RegisterAction({ShamanActions::AncestralSpirit, "ancestral spirit", ExecuteAncestralSpirit});
    engine->RegisterAction({ShamanActions::CleanseSpiritAction, "cleanse spirit", ExecuteCleanseSpirit});
}
