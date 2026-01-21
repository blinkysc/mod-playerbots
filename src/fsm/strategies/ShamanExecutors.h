/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_SHAMANEXECUTORS_H
#define _PLAYERBOT_SHAMANEXECUTORS_H

class FSMEngine;

/**
 * Register all Shaman action executors with the FSM engine.
 * Uses spell names for level-appropriate rank resolution.
 */
void RegisterShamanActions(FSMEngine* engine);

#endif
