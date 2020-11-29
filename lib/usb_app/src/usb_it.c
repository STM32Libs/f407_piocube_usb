#include "stm32f4xx_hal.h"

extern PCD_HandleTypeDef hpcd;

void UsbApp_HAL_PCD_IRQHandler()
{
  HAL_PCD_IRQHandler(&hpcd);
}
