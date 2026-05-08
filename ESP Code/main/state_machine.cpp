#include "state_machine.h"

static SystemState_t currentState = SYS_INIT;

void StateMachine_Init(void)
{
    currentState = SYS_INIT;
}

void StateMachine_Update(void)
{
    switch(currentState)
    {
        case SYS_INIT:

            currentState = SYS_NORMAL;

            break;

        case SYS_NORMAL:

            break;

        case SYS_ALERT:

            break;

        case SYS_ERROR:

            break;

        case SYS_IDLE:

            break;
    }
}

SystemState_t StateMachine_GetState(void)
{
    return currentState;
}