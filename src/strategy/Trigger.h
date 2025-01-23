/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_TRIGGER_H
#define _PLAYERBOT_TRIGGER_H

#include "Action.h"
#include "Common.h"
#include <memory>
#include <string_view>
#include <chrono>

class PlayerbotAI;
class Unit;

class Trigger : public AiNamedObject {
public:
    explicit Trigger(PlayerbotAI* botAI, std::string_view name = "trigger", std::chrono::milliseconds checkInterval = std::chrono::milliseconds(1));
    virtual ~Trigger() = default;

    // Core functionality
    virtual Event Check();
    virtual bool IsActive() { return false; }
    virtual void Reset() {}
    bool NeedsCheck() const;
    void Update() {}

    // Event handling
    virtual void ExternalEvent(std::string_view param, Player* owner = nullptr) {}
    virtual void ExternalEvent(WorldPacket& packet, Player* owner = nullptr) {}

    // Target management
    virtual Unit* GetTarget();
    virtual Value<Unit*>* GetTargetValue();
    virtual std::string_view GetTargetName() const { return "self target"; }
    virtual NextAction** GetHandlers() { return nullptr; }

protected:
    const std::chrono::milliseconds checkInterval;
    std::chrono::steady_clock::time_point lastCheckTime;
};

class TriggerNode {
public:
    explicit TriggerNode(std::string_view name, NextAction** handlers = nullptr);
    ~TriggerNode();

    // Disable copy operations
    TriggerNode(const TriggerNode&) = delete;
    TriggerNode& operator=(const TriggerNode&) = delete;

    // Accessors
    Trigger* GetTrigger() const { return trigger.get(); }
    void SetTrigger(std::unique_ptr<Trigger> newTrigger) { trigger = std::move(newTrigger); }
    std::string_view GetName() const { return name; }
    NextAction** GetHandlers() const;
    float GetFirstRelevance() const;

private:
    std::unique_ptr<Trigger> trigger;
    NextAction** handlers;
    const std::string name;
};

#endif
