/*
 * dbgPrintLog.h
 *
 *  Created on: Aug 20, 2024
 *      Author: minisentry
 */

#ifndef DBGPRINTLOG_H_
#define DBGPRINTLOG_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>

int _write(int32_t file, uint8_t *ptr, int32_t len);

#endif /* DBGPRINTLOG_H_ */
