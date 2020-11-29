#include "usb_app.h"
#include "stm32f4xx_hal.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"

USBD_HandleTypeDef USBD_Device;
PCD_HandleTypeDef hpcd;


bool UsbApp::init()
{

  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);//AUDIO_Desc extern from usbd_desc

  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);//USBD_AUDIO_CLASS from usbd_audio

  return true;
}

void UsbApp_HAL_PCD_IRQHandler()
{
  HAL_PCD_IRQHandler(&hpcd);
}

void USBD_error_handler(void)
{
  while(1){
  }
}
