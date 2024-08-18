/*
 * motorDrv.c
 *
 *  Created on: Jul 11, 2024
 *      Author: minisentry
 */
#include "motorDrv.h"

void resetState(runStateStruct* runState)
{
	runState->tPerStep = 0;
	runState->curStep = 1;
	runState->prevStep = 1;
	runState->pulse = 0;
	runState->curSpd = 0;
	runState->targetSpd = 0;
	runState->dir = MOTOR_DIR_CCW;
	runState->midFreqTaskFlag = 0;
	runState->controlMode = CTRL_MODE_DUTY;
}

void resetMotor(void)
{
	HAL_TIM_PWM_Stop(&MOTOR_TIM, MOTOR_U_CHANNEL);
	HAL_TIM_PWM_Stop(&MOTOR_TIM, MOTOR_V_CHANNEL);
	HAL_TIM_PWM_Stop(&MOTOR_TIM, MOTOR_W_CHANNEL);
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
{	// sets pulse and EN of each phase
	setPWM(pulseA, pulseB, pulseC);
	HAL_GPIO_WritePin(EN_U_GPIO_Port, EN_U_Pin, enA);
	HAL_GPIO_WritePin(EN_V_GPIO_Port, EN_V_Pin, enB);
	HAL_GPIO_WritePin(EN_W_GPIO_Port, EN_W_Pin, enC);
}

void readHall(runStateStruct* runState)
{
	uint8_t measure_flag=1;
	uint8_t bufStep = HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_PIN) + (HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_PIN) << 1) + (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_PIN) << 2);
	runState->prevStep = runState->curStep;

	while(measure_flag)
	{
		switch(bufStep)
		{
			// step number is labeled according to ST's UM2788 page 12 "Hall sensor algorithm" Table 1
			// for this motor 1 -> 6 is CCW
			case 1:
				runState->curStep = 1;
				break;
			case 2:
				runState->curStep = 3;
				break;
			case 3:
				runState->curStep = 2;
				break;
			case 4:
				runState->curStep = 5;
				break;
			case 5:
				runState->curStep = 6;
				break;
			case 6:
				runState->curStep = 4;
				break;
			default:
				measure_flag = measure_flag + 2;	//each loop does flag+2-1
		}
		--measure_flag;
		if (measure_flag > 3)
		{
			//TODO: add error logger here
			//	also add step loss identifier
			measure_flag = 0;
		}
	}
}

void doPulse(runStateStruct* runState)
{	//	sets where current flows according to recorded rotor position
	//	this is incomplete, read BLDC basics
	if(runState->dir == MOTOR_DIR_CCW)
	{
		switch(runState->curStep)
		{
			case 1:
				setMotor(runState->pulse, 0, 0, 1, 0, 1);
				break;
			case 2:
				setMotor(0, runState->pulse, 0, 0, 1, 1);
				break;
			case 3:
				setMotor(0, runState->pulse, 0, 1, 1, 0);
				break;
			case 4:
				setMotor(0, 0, runState->pulse, 1, 0, 1);
				break;
			case 5:
				setMotor(0, 0, runState->pulse, 0, 1, 1);
				break;
			case 6:
				setMotor(runState->pulse, 0, 0, 1, 1, 0);
				break;
		}
	}
	else
	{
		/*switch(runState->curStep)
		{
			case 1:
				setMotor(runState->pulse, 0, 0, 1, 0, 1);
				break;
			case 2:
				setMotor(0, runState->pulse, 0, 0, 1, 1);
				break;
			case 3:
				setMotor(0, runState->pulse, 0, 1, 1, 0);
				break;
			case 4:
				setMotor(0, 0, runState->pulse, 1, 0, 1);
				break;
			case 5:
				setMotor(0, 0, runState->pulse, 0, 1, 1);
				break;
			case 6:
				setMotor(runState->pulse, 0, 0, 1, 1, 0);
				break;
		}*/
	}
}

void moveMotor(runStateStruct* runState)
{

}
