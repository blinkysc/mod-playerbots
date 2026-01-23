/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef PLAYERBOT_COMPAT_H
#define PLAYERBOT_COMPAT_H

#include "ObjectGuid.h"
#include "Group.h"

// NOTE: Group::GetTargetIcon doesn't exist in standard AzerothCore
// This compatibility helper returns empty guid, meaning raid icons can be
// set but not queried. Bots will always update markers instead of checking
// if already set.
inline ObjectGuid PlayerbotGetTargetIconCompat(Group* /*group*/, uint8 /*iconId*/)
{
    return ObjectGuid::Empty;
}

// Macro to make replacement easier - use this in place of group->GetTargetIcon(x)
#define COMPAT_GET_TARGET_ICON(group, iconId) PlayerbotGetTargetIconCompat(group, iconId)

// NOTE: VEHICLE_FLAG_FIXED_POSITION doesn't exist in standard AzerothCore
// Define as 0 so the check always fails (treats all vehicles as moving vehicles)
#ifndef VEHICLE_FLAG_FIXED_POSITION
#define VEHICLE_FLAG_FIXED_POSITION 0
#endif

#endif // PLAYERBOT_COMPAT_H
