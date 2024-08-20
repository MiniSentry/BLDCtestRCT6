/*
 * speedCalc.h
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */

#ifndef INC_SPEEDCALC_H_
#define INC_SPEEDCALC_H_

#include "motorDrv.h"
#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "sysTickManipulator.h"

#include "dbgPrintLog.h"

void speedCalcInit();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void updateState(runStateStruct* runState);
float getVelocity(runStateStruct* runState);

#endif /* INC_SPEEDCALC_H_ */
