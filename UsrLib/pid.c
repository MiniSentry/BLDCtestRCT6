/*
 * pid.c
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */
#include "pid.h"

void PIDstructInit(PIDstruct* PIDstruct)
{
	PIDstruct->P = 0.03f;
	PIDstruct->I = 1.0f;
	PIDstruct->D = 0.0f;
	PIDstruct->ramp = 100;
	PIDstruct->limit = 4;
	PIDstruct->error_prev = 0;
	PIDstruct->output_prev = 0;
	PIDstruct->integral_prev = 0;
	PIDstruct->timestamp_prev = 0;
}

float PIDoperator(float error, PIDstruct* PIDstruct)
{
	// get time elapsed from last call
	uint32_t timestamp_now = getCurrentMicros();
	float Ts = (timestamp_now - PIDstruct->timestamp_prev) * 1e-6f;
	// in case of overflow
	if(Ts <= 0 || Ts > 0.5f)
		Ts = 1e-3f;

    // u(s) = (P + I/s + Ds)e(s)
    // Discrete implementations
    // proportional part
    // u_p  = P *e(k)
    float proportional = PIDstruct->P * error;
    // Tustin transform of the integral part
    // u_ik = u_ik_1  + I*Ts/2*(ek + ek_1)
    float integral = PIDstruct->integral_prev + PIDstruct->I*Ts*0.5f*(error + PIDstruct->error_prev);
    // antiwindup - limit the output
    if(integral > PIDstruct->limit)
    	integral = PIDstruct->limit;
    else if(integral < -PIDstruct->limit)
    	integral = -PIDstruct->limit;
    // Discrete derivation
    // u_dk = D(ek - ek_1)/Ts
    float derivative = PIDstruct->D*(error - PIDstruct->error_prev)/Ts;

    // sum all the components
    float output = proportional + integral + derivative;

    // antiwindup - limit the output variable
    if(output > PIDstruct->limit)
    	output = PIDstruct->limit;
    else if(output < -PIDstruct->limit)
    	output = -PIDstruct->limit;

    // if output ramp defined
    if(PIDstruct->ramp > 0)
    {
        // limit the acceleration by ramping the output
        float output_rate = (output - PIDstruct->output_prev)/Ts;
        if (output_rate > PIDstruct->ramp)
            output = PIDstruct->output_prev + PIDstruct->ramp*Ts;
        else if (output_rate < -PIDstruct->ramp)
            output = PIDstruct->output_prev - PIDstruct->ramp*Ts;
    }

    // saving for the next pass
    PIDstruct->integral_prev = integral;
    PIDstruct->output_prev = output;
    PIDstruct->error_prev = error;
    PIDstruct->timestamp_prev = timestamp_now;
    return output;
}

void PIDreset(PIDstruct* PIDstruct)
{
	PIDstruct->integral_prev = 0.0f;
	PIDstruct->output_prev = 0.0f;
	PIDstruct->error_prev = 0.0f;
}
