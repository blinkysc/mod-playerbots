/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PLAYERBOTAIBASE_H
#define _PLAYERBOT_PLAYERBOTAIBASE_H

#include "Define.h"
#include "PlayerbotAIConfig.h"
#include <atomic>

class PlayerbotAIBase
{
public:
    PlayerbotAIBase(bool isBotAI);

    bool CanUpdateAI();
    virtual void SetNextCheckDelay(uint32 const delay);
    void IncreaseNextCheckDelay(uint32 delay);
    void YieldThread(uint32 delay = sPlayerbotAIConfig->reactDelay);
    virtual void UpdateAI(uint32 elapsed, bool minimal = false);
    virtual void UpdateAIInternal(uint32 elapsed, bool minimal = false) = 0;
    bool IsActive();
    bool IsBotAI() const;

protected:
    std::atomic<uint32> nextAICheckDelay;
    class PerformanceMonitorOperation* totalPmo = nullptr;

private:
    bool _isBotAI;
};

#endif
