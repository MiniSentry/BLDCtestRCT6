/*
 * motorDrv.c
 *
 *  Created on: Jul 11, 2024
 *      Author: minisentry
 */
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include <stdint.h>

#define MOTOR_TIM htim1
#define MOTOR_U_CHANNEL TIM_CHANNEL_1
#define MOTOR_V_CHANNEL TIM_CHANNEL_2
#define MOTOR_W_CHANNEL TIM_CHANNEL_3

#define EN_U_Pin GPIO_PIN_7
#define EN_U_GPIO_Port GPIOC
#define EN_V_Pin GPIO_PIN_8
#define EN_V_GPIO_Port GPIOC
#define EN_W_Pin GPIO_PIN_9
#define EN_W_GPIO_Port GPIOC

#define HALL_A_PIN GPIO_PIN_12
#define HALL_A_GPIO_Port GPIOB
#define HALL_B_PIN GPIO_PIN_13
#define HALL_B_GPIO_Port GPIOB
#define HALL_C_PIN GPIO_PIN_14
#define HALL_C_GPIO_Port GPIOB

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

static runStateStruct runState;

void resetState()
{
	runState.cntMFtask = 0;
	runState.tPerStep[0] = 0;
	runState.curStep = 0;
	runState.prevStep = 0;
	runState.pulse = 0;
	runState.curSpd = 0;
	runState.targetSpd = 0;
}

void resetMotor()
{
	//turn all phases off
	HAL_GPIO_WritePin(EN_U_GPIO_Port, EN_U_Pin, 0);
	HAL_GPIO_WritePin(EN_V_GPIO_Port, EN_V_Pin, 0);
	HAL_GPIO_WritePin(EN_W_GPIO_Port, EN_W_Pin, 0);

	HAL_TIM_PWM_MspInit(&MOTOR_TIM);
	HAL_TIM_PWM_Start(&MOTOR_TIM, MOTOR_U_CHANNEL);
	HAL_TIM_PWM_Start(&MOTOR_TIM, MOTOR_V_CHANNEL);
	HAL_TIM_PWM_Start(&MOTOR_TIM, MOTOR_W_CHANNEL);

}

void setPWM(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC)	//period is 1600
{
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_U_CHANNEL, pulseA);
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_V_CHANNEL, pulseB);
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_W_CHANNEL, pulseC);
}

void setMotor(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC, uint8_t enA, uint8_t enB, uint8_t enC)
{
	setPWM(pulseA, pulseB, pulseC);
	HAL_GPIO_WritePin(EN_U_GPIO_Port, EN_U_Pin, enA);
	HAL_GPIO_WritePin(EN_V_GPIO_Port, EN_V_Pin, enB);
	HAL_GPIO_WritePin(EN_W_GPIO_Port, EN_W_Pin, enC);
}

void readHall()
{
	uint8_t measure_flag=1;
	uint8_t bufStep = HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_PIN) + (HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_PIN) << 1) + (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_PIN) << 2);
	runState.prevStep = runState.curStep;

	while(measure_flag)
	{
			switch(bufStep)
			{
				// step number is labeled according to ST's UM2788 page 12 "Hall sensor algorithm" Table 1
				case 1:
					runState.curStep = 1;
					break;
				case 2:
					runState.curStep = 3;
					break;
				case 3:
					runState.curStep = 2;
					break;
				case 4:
					runState.curStep = 5;
					break;
				case 5:
					runState.curStep = 6;
					break;
				case 6:
					runState.curStep = 4;
					break;
				default:
					measure_flag = measure_flag + 2;	//each loop does flag+2-1
			}
			--measure_flag;
			if (measure_flag > 3)
			{
				//TODO: add error logger here
				measure_flag = 0;
			}
	}
}

void doPulse()
{
	switch(runState.curStep)
	{
		case 1:
			setMotor(runState.pulse, 0, 0, 1, 0, 1);
			break;
		case 2:
			setMotor(0, runState.pulse, 0, 0, 1, 1);
			break;
		case 3:
			setMotor(0, runState.pulse, 0, 1, 1, 0);
			break;
		case 4:
			setMotor(0, 0, runState.pulse, 1, 0, 1);
			break;
		case 5:
			setMotor(0, 0, runState.pulse, 0, 1, 1);
			break;
		case 6:
			setMotor(runState.pulse, 0, 0, 1, 1, 0);
			break;
	}
}
