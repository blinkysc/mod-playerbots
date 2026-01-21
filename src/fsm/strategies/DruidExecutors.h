/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_DRUIDEXECUTORS_H
#define _PLAYERBOT_DRUIDEXECUTORS_H

class FSMEngine;

/**
 * Register all Druid action executors with the FSM engine.
 * Uses spell names for level-appropriate rank resolution.
 */
void RegisterDruidActions(FSMEngine* engine);

#endif
