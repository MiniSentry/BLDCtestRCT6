/*
 * motorDrv.c
 *
 *  Created on: Jul 11, 2024
 *      Author: minisentry
 */
#include "motorDrv.h"

void resetState(runStateStruct* runState)
{

	runState->curStep = 1;
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

void setMotor(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC, uint8_t enA, uint8_t enB, uint8_t enC)
{	// sets pulse and EN of each phase
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_U_CHANNEL, pulseA);
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_V_CHANNEL, pulseB);
	__HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_W_CHANNEL, pulseC);
	HAL_GPIO_WritePin(EN_U_GPIO_Port, EN_U_Pin, enA);
	HAL_GPIO_WritePin(EN_V_GPIO_Port, EN_V_Pin, enB);
	HAL_GPIO_WritePin(EN_W_GPIO_Port, EN_W_Pin, enC);
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
		switch(runState->curStep)
		{
			case 1:
				setMotor(0, 0, runState->pulse, 1, 0, 1);
				break;
			case 2:
				setMotor(0, 0, runState->pulse, 0, 1, 1);
				break;
			case 3:
				setMotor(runState->pulse, 0, 0, 1, 1, 0);
				break;
			case 4:
				setMotor(runState->pulse, 0, 0, 1, 0, 1);
				break;
			case 5:
				setMotor(0, runState->pulse, 0, 0, 1, 1);
				break;
			case 6:
				setMotor(0, runState->pulse, 0, 1, 1, 0);
				break;
		}
	}
}

void motorAlign(runStateStruct* runState)
{
	runState->pulse = 100;
	if(runState->dir == MOTOR_DIR_CCW)
	{
		for(uint8_t i=1; i<7; i++)
		{
			HAL_Delay(10);
			runState->curStep = i;
			doPulse(runState);
		}
	}
	else
	{
		for(uint8_t i=6; i>0; i--)
		{
			HAL_Delay(10);
			runState->curStep = i;
			doPulse(runState);
		}
	}
}


