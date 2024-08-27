#include "uartProtocol.h"
#include "stm32f103xe.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"

/*
  This uart protocol is designed to be looped in a RTOS environment as a task
  it roughly implements a MODBUS like protocol, allowing a more robust transfer
*/
#define DEVICE_ADDR 0xAA
uint8_t uartRxBuffer[32] = {0};
volatile uint8_t uartRxFinishFlag = 0;

void vUartProtocol(void * pvParameters)
{
  for(;;)
  {
    HAL_UART_Receive_DMA(&huart4, uartRxBuffer, sizeof(uartRxBuffer));

    if(uartRxFinishFlag == 1)
    {
      uartRxFinishFlag = 0;
      if(uartRxBuffer[0] == DEVICE_ADDR)
      { //TODO: add CRC check here
        uartRxFinishFlag = 0;

        //debug chunk
        for(uint8_t i=0; i<sizeof(uartRxBuffer); i++)
        {
          ITM_SendChar(uartRxBuffer[i]);
        }
        ITM_SendChar('\n');
        //debug chunk end

      }
      else  // data fucked
      {
        printf("data corrupt\n");
        HAL_UART_AbortReceive(&huart4);
      }
      for(uint8_t i=0; i<sizeof(uartRxBuffer); i++)
      {
        uartRxBuffer[i] = 0;
      }
    }
    else if(uartRxBuffer[0]!= 0 && uartRxBuffer[0] != DEVICE_ADDR)
    { // when DMA pushes garbage into buffer
      HAL_UART_AbortReceive(&huart4);
      for(uint8_t i=0; i<sizeof(uartRxBuffer); i++)
      {
        uartRxBuffer[i] = 0;
      }
      
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart4)
    uartRxFinishFlag = 1;
}

