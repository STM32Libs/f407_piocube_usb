#include "usb_app.h"
#include "stm32f4xx_hal.h"

#include "usbd_core.h"
#include "usbd_desc.h"

USBD_HandleTypeDef USBD_Device;
PCD_HandleTypeDef hpcd;


bool UsbApp::init()
{

  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);

  return true;
}

void UsbApp_HAL_PCD_IRQHandler()
{
  HAL_PCD_IRQHandler(&hpcd);
}
