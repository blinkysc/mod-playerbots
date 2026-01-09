/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license
 */

#ifndef _PLAYERBOT_WARRIOREXECUTORS_H
#define _PLAYERBOT_WARRIOREXECUTORS_H

class FSMEngine;

/**
 * Register all Warrior action executors with the FSM engine.
 * Also adds the WarriorFuryStrategy table.
 */
void RegisterWarriorActions(FSMEngine* engine);

#endif // _PLAYERBOT_WARRIOREXECUTORS_H
