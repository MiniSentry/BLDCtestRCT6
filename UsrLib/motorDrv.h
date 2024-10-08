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
#include "sysTickManipulator.h"

//motor PWM timer pins
#define MOTOR_TIM htim1
#define MOTOR_U_CHANNEL TIM_CHANNEL_1
#define MOTOR_V_CHANNEL TIM_CHANNEL_2
#define MOTOR_W_CHANNEL TIM_CHANNEL_3

//motor driver IC EN pins
#define EN_U_Pin GPIO_PIN_7
#define EN_U_GPIO_Port GPIOC
#define EN_V_Pin GPIO_PIN_8
#define EN_V_GPIO_Port GPIOC
#define EN_W_Pin GPIO_PIN_9
#define EN_W_GPIO_Port GPIOC

//Hall sensor pins
#define HALL_A_PIN GPIO_PIN_12
#define HALL_A_GPIO_Port GPIOB
#define HALL_B_PIN GPIO_PIN_13
#define HALL_B_GPIO_Port GPIOB
#define HALL_C_PIN GPIO_PIN_14
#define HALL_C_GPIO_Port GPIOB

#define HALL_READ_POLLING 0
#define HALL_READ_USE_INTERRUPT 1

//motor rotate direction define
#define MOTOR_DIR_CW 0
#define MOTOR_DIR_CCW 1

//task scheduler config
#define MID_FREQ_TASK_INTERVAL 10

//motor control mode
#define CTRL_MODE_DUTY 0
#define CTRL_MODE_SPEED 1
#define CTRL_MODE_ANGLE 2

typedef struct
{
	// u might have noticed that this should be split into 2 structs, one for moving the motor and the other for hall sensor
	uint32_t cntMFtask;
	uint32_t tPerStep[6];
	int8_t curStep;
	uint16_t pulse;
	float curSpd;
	float targetSpd;
	uint8_t dir;	// motor direction, CW is 0 and CCW is 1
	uint8_t midFreqTaskFlag;
	uint8_t controlMode;
	int32_t electric_rotations;		//CCW counts up and CW counts down, you will probably see this staying in negative in CW mode
	int8_t ActualDir;
	int8_t lastActualDir;
	uint32_t pulse_timestamp;
}runStateStruct;

//static runStateStruct runStateM1;

void resetState(runStateStruct* runState);
void resetMotor(void);
void setPWM(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC);
void setMotor(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC, uint8_t enA, uint8_t enB, uint8_t enC);
void doPulse(runStateStruct* runState);
void motorAlign(runStateStruct* runState);

#endif /* INC_MOTORDRV_H_ */


