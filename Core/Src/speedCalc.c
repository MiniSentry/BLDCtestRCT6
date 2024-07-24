/*
 * speedCalc.c
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */
#include "speedCalc.h"

volatile uint8_t HallReadISRcache = 1;
volatile uint8_t HallISRflag = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HallReadISRcache = HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_PIN)
	+ (HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_PIN) << 1)
	+ (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_PIN) << 2);
	HallISRflag = 1;
}

