/*
 * motorDrv.h
 *
 *  Created on: Jul 11, 2024
 *      Author: minisentry
 */

#ifndef INC_MOTORDRV_H_
#define INC_MOTORDRV_H_

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include <stdint.h>

typedef struct
{
	uint32_t cntMFtask;
	uint32_t tPerStep[10];
	uint8_t curStep;
	uint8_t prevStep;
	uint16_t pulse;
	uint32_t curSpd;
	uint32_t targetSpd;
}runStateStruct;

//static runStateStruct runStateM1;

void resetState(runStateStruct* runState);
void resetMotor();
void setPWM(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC);
void setMotor(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC, uint8_t enA, uint8_t enB, uint8_t enC);
void readHall(runStateStruct* runState);
void doPulse(runStateStruct* runState);

#endif /* INC_MOTORDRV_H_ */
