/*
 * StateMachine.c
 *
 *  Created on: Jul 17, 2024
 *      Author: minisentry
 */
#include "StateMachine.h"

void MF_Task(runStateStruct* runState)
{

}

void stateMachine(runStateStruct* runState)
{
	if(runState->midFreqTaskFlag == 1)
	{
		MF_Task(runState);
		runState->midFreqTaskFlag = 0;
	}

}
