/*
 * dbgPrintLog.c
 *
 *  Created on: Aug 20, 2024
 *      Author: minisentry
 */
#include "dbgPrintLog.h"

int _write(int32_t file, uint8_t *ptr, int32_t len)
{
    for (int i = 0; i < len; i++)
    {
        ITM_SendChar(*ptr++);
    }
    return len;
}
