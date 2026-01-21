/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_HUNTEREXECUTORS_H
#define _PLAYERBOT_HUNTEREXECUTORS_H

class FSMEngine;

/**
 * Register all Hunter action executors with the FSM engine.
 * Uses spell names for level-appropriate rank resolution.
 */
void RegisterHunterActions(FSMEngine* engine);

#endif
