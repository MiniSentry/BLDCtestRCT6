/*
 * sysTickManipulator.c
 *
 *  Created on: Jul 22, 2024
 *      Author: minisentry
 */
#include "sysTickManipulator.h"

/**
  * @brief  Function called to read the current micro second
  * yes i am aware that this is a hack so you WILL need to edit
  * this each time you port this bloody code
  * @param  None
  * @retval None
  */
uint32_t getCurrentMicros(void)
{
  uint32_t m0 = HAL_GetTick();
  __IO uint32_t u0 = TIM8->CNT;
  uint32_t m1 = HAL_GetTick();
  __IO uint32_t u1 = TIM8->CNT;
  //const uint32_t tms = TIM8->ARR + 1;

  if (m1 != m0) {
    return (m1 * 1000 + u1);
  } else {
    return (m0 * 1000 + u0);
  }
}

