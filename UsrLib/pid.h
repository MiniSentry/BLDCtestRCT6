/*
 * pid.h
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "sysTickManipulator.h"

typedef struct
{
	float P;
	float I;
	float D;
	float ramp;	// output derivative limit [volts/second]
	float limit; // output supply limit     [volts]
	float error_prev;
	float output_prev;
	float integral_prev;
	uint64_t timestamp_prev;
}PIDstruct;

void PIDstructInit(PIDstruct* PIDstruct);

float PIDoperator(float error, PIDstruct* PIDstruct);

void PIDreset(PIDstruct* PIDstruct);

#endif /* INC_PID_H_ */
