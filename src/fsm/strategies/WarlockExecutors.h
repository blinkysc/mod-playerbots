/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARLOCKEXECUTORS_H
#define _PLAYERBOT_WARLOCKEXECUTORS_H

class FSMEngine;

/**
 * Register all Warlock action executors with the FSM engine.
 * Uses spell names for level-appropriate rank resolution.
 */
void RegisterWarlockActions(FSMEngine* engine);

#endif
