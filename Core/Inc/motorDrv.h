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

#define MOTOR_DIR_CW 0
#define MOTOR_DIR_CCW 1

#define MID_FREQ_TASK_INTERVAL 10

#define CTRL_MODE_DUTY 0
#define CTRL_MODE_SPEED 1
#define CTRL_MODE_ANGLE 2

typedef struct
{
	uint32_t cntMFtask;
	uint32_t tPerStep[10];
	uint8_t curStep;
	uint8_t prevStep;
	uint16_t pulse;
	uint32_t curSpd;
	uint32_t targetSpd;
	uint8_t dir;	// motor direction, CW is 0 and CCW is 1
	uint8_t midFreqTaskFlag;
	uint8_t controlMode;
}runStateStruct;

//static runStateStruct runStateM1;

void resetState(runStateStruct* runState);
void resetMotor();
void setPWM(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC);
void setMotor(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC, uint8_t enA, uint8_t enB, uint8_t enC);
void readHall(runStateStruct* runState);
void doPulse(runStateStruct* runState);

#endif /* INC_MOTORDRV_H_ */
