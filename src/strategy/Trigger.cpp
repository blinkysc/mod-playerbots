/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "Trigger.h"
#include "Event.h"
#include "Playerbots.h"

using namespace std::chrono;

Trigger::Trigger(PlayerbotAI* botAI, std::string_view name, milliseconds interval)
    : AiNamedObject(botAI, std::string(name)),
      checkInterval(interval < milliseconds(100) ? interval * 1000 : interval),
      lastCheckTime(steady_clock::now())
{
}

Event Trigger::Check() {
    if (IsActive()) {
        return Event(getName());
    }
    return Event();
}

Value<Unit*>* Trigger::GetTargetValue() {
    return context->GetValue<Unit*>(std::string(GetTargetName()));
}

Unit* Trigger::GetTarget() {
    if (Value<Unit*>* value = GetTargetValue()) {
        return value->Get();
    }
    return nullptr;
}

bool Trigger::NeedsCheck() const {
    if (checkInterval < milliseconds(2)) {
        return true;
    }

    auto now = steady_clock::now();
    if (now - lastCheckTime >= checkInterval) {
        lastCheckTime = now;
        return true;
    }
    return false;
}

TriggerNode::TriggerNode(std::string_view name, NextAction** handlers)
    : trigger(nullptr),
      handlers(handlers),
      name(std::string(name))
{
}

TriggerNode::~TriggerNode() {
    NextAction::destroy(handlers);
}

NextAction** TriggerNode::GetHandlers() const {
    return NextAction::merge(NextAction::clone(handlers), trigger->GetHandlers());
}

float TriggerNode::GetFirstRelevance() const {
    return handlers && handlers[0] ? handlers[0]->getRelevance() : -1.0f;
}
