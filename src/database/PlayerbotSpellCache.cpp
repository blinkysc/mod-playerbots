#include "PlayerbotSpellCache.h"
#include "SpellMgr.h"

void PlayerbotSpellCache::Initialize()
{
    LOG_INFO("playerbots",
        "Playerbots: ListSpellsAction caches initialized");
    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        if (SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j))
            skillSpells[skillLine->Spell] = skillLine;
    }

    // Fill the vendorItems cache once from the world database.
    QueryResult results = WorldDatabase.Query("SELECT item FROM npc_vendor WHERE maxcount = 0");
    if (results)
    {
        do
        {
            Field* fields = results->Fetch();
            int32 entry = fields[0].Get<int32>();
            if (entry <= 0)
                continue;

            vendorItems.insert(static_cast<uint32>(entry));
        }
        while (results->NextRow());
    }

    // Build spell name -> ID cache for fast GetAura lookups
    // This replaces O(320 * auras) string iteration with O(1) hash lookup
    uint32 spellCount = sSpellMgr->GetSpellInfoStoreSize();
    for (uint32 spellId = 1; spellId < spellCount; ++spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            continue;

        char const* spellName = spellInfo->SpellName[0];
        if (!spellName || !spellName[0])
            continue;

        // Convert to lowercase for case-insensitive matching
        std::string nameLower = spellName;
        for (char& c : nameLower)
            c = std::tolower(c);

        // Only store if not already present (keeps lowest/first rank)
        if (spellNameToId.find(nameLower) == spellNameToId.end())
            spellNameToId[nameLower] = spellId;
    }

    LOG_INFO("playerbots",
        "Playerbots: initialized caches (skillSpells={}, vendorItems={}, spellNames={}).",
        skillSpells.size(), vendorItems.size(), spellNameToId.size());
}

SkillLineAbilityEntry const* PlayerbotSpellCache::GetSkillLine(uint32 spellId) const
{
    auto itr = skillSpells.find(spellId);
    if (itr != skillSpells.end())
        return itr->second;
    return nullptr;
}

bool PlayerbotSpellCache::IsItemBuyable(uint32 itemId) const
{
    return vendorItems.find(itemId) != vendorItems.end();
}

uint32 PlayerbotSpellCache::GetSpellIdByName(std::string const& name) const
{
    // Convert to lowercase for lookup
    std::string nameLower = name;
    for (char& c : nameLower)
        c = std::tolower(c);

    auto it = spellNameToId.find(nameLower);
    return (it != spellNameToId.end()) ? it->second : 0;
}
