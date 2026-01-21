/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_PRIESTEXECUTORS_H
#define _PLAYERBOT_PRIESTEXECUTORS_H

class FSMEngine;

/**
 * Register all Priest action executors with the FSM engine.
 * Uses spell names for level-appropriate rank resolution.
 */
void RegisterPriestActions(FSMEngine* engine);

#endif
