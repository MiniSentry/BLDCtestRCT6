#include "bldcRtosPort.h"
#include "stm32f1xx_hal.h"

extern volatile uint8_t dbg_send_cnt;   //get this to increase by 1 each 1ms in a timer
// below is the place where you add more motor instances
runStateStruct runStateM1;
PIDstruct PIDM1struct;
MotorType Motor1 = {&runStateM1, &PIDM1struct};

void vMotorPID(void * pvParameters)
{
  static uint8_t StallFlag;
  MotorType* pMotor = (MotorType*) pvParameters;
  runStateStruct* runState = pMotor->runState;
  PIDstruct* PIDstruct = pMotor->PIDstructure;
  for(;;)
  {
    if (runState->curSpd == 0.0f)
      StallFlag++;
    runState->curSpd = getVelocity(runState);
    if ((runState->curSpd == 0.0f) && StallFlag >= 200)
    {
      motorAlign(&runStateM1);
      StallFlag = 0;
    }
    if(runState->dir == MOTOR_DIR_CCW)
      runState->pulse = (uint16_t)(133.0f * PIDoperator(runState->targetSpd - runState->curSpd, PIDstruct));
    else
      runState->pulse = (uint16_t)(133.0f * PIDoperator(runState->curSpd - runState->targetSpd, PIDstruct));
    if(dbg_send_cnt > 50)
    {
      dbg_send_cnt = 0;
      //printf("%d", (uint16_t)(runStateM1.curSpd*100));
      ITM_SendChar((uint8_t)runStateM1.curSpd);
    }
  }
}
