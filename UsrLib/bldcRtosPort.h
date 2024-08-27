#ifndef _BLDC_RTOS_PORT_H
#define _BLDC_RTOS_PORT_H

#include "motorDrv.h"
#include "speedCalc.h"
#include "pid.h"
#include "dbgPrintLog.h"
#include "sysTickManipulator.h"
#include "FreeRTOS.h"
#include "task.h"

void vMotorPID(void * pvParameters);

typedef struct
{
  runStateStruct* runState;
  PIDstruct* PIDstructure;
}MotorType;


#endif

