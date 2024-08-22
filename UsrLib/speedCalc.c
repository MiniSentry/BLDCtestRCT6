/*
 * speedCalc.c
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */
#include "speedCalc.h"
#define _2PI 6.28318530718f
#define PP 3
volatile uint8_t Hall_A_Status;
volatile uint8_t Hall_B_Status;
volatile uint8_t Hall_C_Status;
extern runStateStruct runStateM1;			//so the ISR will be able to locate your motor's properties

void speedCalcInit(void)
{
	Hall_A_Status = 1;
	Hall_B_Status = 1;
	Hall_C_Status = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)	//this is clearly non-portable, u will need to edit this if u want to add a second motor
{
	switch(GPIO_Pin)
	{
		case HALL_A_PIN:
			//Hall_A_Status = HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_PIN);
			updateState(&runStateM1);
			break;
		case HALL_B_PIN:
			//Hall_B_Status = HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_PIN);
			updateState(&runStateM1);
			break;
		case HALL_C_PIN:
			//Hall_C_Status = HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_PIN);
			updateState(&runStateM1);
			break;
		default:
			break;
	}
}

void updateState(runStateStruct* runState)
{
	uint32_t new_pulse_timestamp = getCurrentMicros();
	//int8_t bufStep = Hall_A_Status + (Hall_B_Status << 1) + (Hall_C_Status << 2);
	int8_t bufStep = HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_PIN) + (HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_PIN) << 1) + (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_PIN) << 2);
	const int8_t GET_STEP[8] = {-1, 1, 3, 2, 5, 6, 4, -1};
	// This magic array will allow you to get current step number labeled according to ST's UM2788 page 12 "Hall sensor algorithm" Table 1
	//	using just raw results from your hall sensor.
	bufStep = GET_STEP[bufStep];
	printf("[%ld]step: %d\n", new_pulse_timestamp, bufStep);
	if(bufStep == -1)
		return;
	if (bufStep == runState->curStep)
		return;		//bloody step did not even change

	if (bufStep - runState->curStep < -3)
	{
		runState->electric_rotations++;	// CCW rotation increase
	}
	else if (bufStep - runState->curStep > 3)
	{
		runState->electric_rotations--;	//CW rotation increase
	}
	else if(runState->curStep != 6)
	{
		if (bufStep > runState->curStep)
			runState->ActualDir = MOTOR_DIR_CCW;
		else
			runState->ActualDir = -1;	//cw
	}

	runState->curStep = bufStep;

    // glitch avoidance #2 changes in direction can cause velocity spikes.  Possible improvements needed in this area
	if (runState->ActualDir == runState->lastActualDir)
	{
		//not oscillating or having a dir change
		for (int i = 5; i > 0; i--)
		{
			runState->tPerStep[i] = runState->tPerStep[i-1];
		}
		runState->tPerStep[0] = new_pulse_timestamp - runState->pulse_timestamp;
	}
	else
	{
		for (int i = 5; i >= 0; i--)
		{
			runState->tPerStep[i] = 0;
		}
	}
	runState->pulse_timestamp = new_pulse_timestamp;
	runState->lastActualDir = runState->ActualDir;	//TODO: u might want to remove a dir from the struct as well and edit the implementation here

}

float getVelocity(runStateStruct* runState)
{
	//__disable_irq();
	uint32_t last_pulse_timestamp = runState->pulse_timestamp;
	uint32_t last_pulse_diff = (runState->tPerStep[0] + runState->tPerStep[1] + runState->tPerStep[2] + runState->tPerStep[3] + runState->tPerStep[4]
	+ runState->tPerStep[5]) / 6;
	//__enable_irq();
	if (last_pulse_diff == 0 || ((getCurrentMicros() - last_pulse_timestamp) > last_pulse_diff*4) )
	{ // last velocity isn't accurate if too old
		return 0.0f;
	}
	else
	{
		float dbg_velocity_result = (runState->ActualDir * (_2PI / (float)(PP*6)) / (last_pulse_diff / 1000000.0f));
		return dbg_velocity_result;	// rad/s
	}
}



