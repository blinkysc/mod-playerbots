/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PLAYERBOTAIBASE_H
#define _PLAYERBOT_PLAYERBOTAIBASE_H

#include "Define.h"
#include "PlayerbotAIConfig.h"

class PerformanceMonitorOperation;

/**
 * @class PlayerbotAIBase
 * @brief Base class for playerbot AI functionality
 * 
 * This class provides core functionality for managing AI updates and timing
 * for playerbots. It handles delay management and thread yielding to prevent
 * excessive CPU usage.
 */
class PlayerbotAIBase
{
public:
    explicit PlayerbotAIBase(bool isBotAI = false);
    virtual ~PlayerbotAIBase() = default;

    // Prevent copying
    PlayerbotAIBase(const PlayerbotAIBase&) = delete;
    PlayerbotAIBase& operator=(const PlayerbotAIBase&) = delete;

    /**
     * @brief Checks if the AI can be updated
     * @return true if AI can be updated, false otherwise
     */
    [[nodiscard]] bool CanUpdateAI() const;

    /**
     * @brief Sets the delay until the next AI check
     * @param delay The delay in milliseconds
     */
    void SetNextCheckDelay(uint32 const delay);

    /**
     * @brief Increases the current delay until next AI check
     * @param delay Additional delay in milliseconds to add
     */
    void IncreaseNextCheckDelay(uint32 delay);

    /**
     * @brief Yields the AI thread for a specified duration
     * @param delay The delay duration in milliseconds (default: config react delay)
     */
    void YieldThread(uint32 delay = sPlayerbotAIConfig->reactDelay);

    /**
     * @brief Updates the AI state
     * @param elapsed Time elapsed since last update
     * @param minimal Whether to perform minimal update only
     */
    virtual void UpdateAI(uint32 elapsed, bool minimal = false);

    /**
     * @brief Internal AI update implementation (must be overridden by derived classes)
     * @param elapsed Time elapsed since last update
     * @param minimal Whether to perform minimal update only
     */
    virtual void UpdateAIInternal(uint32 elapsed, bool minimal = false) = 0;

    /**
     * @brief Checks if the AI is currently active
     * @return true if AI is active, false otherwise
     */
    [[nodiscard]] bool IsActive() const;

    /**
     * @brief Checks if this is a bot AI
     * @return true if this is a bot AI, false otherwise
     */
    [[nodiscard]] bool IsBotAI() const;

protected:
    uint32 nextAICheckDelay;                      // Delay until next AI check
    PerformanceMonitorOperation* totalPmo;        // Performance monitoring operation

private:
    const bool _isBotAI;                         // Flag indicating if this is a bot AI
};

#endif // _PLAYERBOT_PLAYERBOTAIBASE_H
