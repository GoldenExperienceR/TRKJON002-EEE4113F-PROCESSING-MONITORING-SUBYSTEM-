#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "firmware_types.h"




void StateMachine_Init(void);

void StateMachine_Update(void);

SystemState_t StateMachine_GetState(void);

#endif

