/*
 * motorDrv.h
 *
 *  Created on: Jul 11, 2024
 *      Author: minisentry
 */

#ifndef INC_MOTORDRV_H_
#define INC_MOTORDRV_H_

void resetMotor();
void setPWM(uint16_t pulseA, uint16_t pulseB, uint16_t pulseC);

#endif /* INC_MOTORDRV_H_ */
