#include "usb_app.h"
#include "stm32f4xx_hal.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"

USBD_HandleTypeDef USBD_Device;//USBD_Device.pData <= (PCD_HandleTypeDef *hpcd)
extern USBD_AUDIO_InterfaceCallbacksfTypeDef audio_class_interface;//from usbd_audio_if.c

void HAL_PCD_MspInit_FS_no_cb();

bool UsbApp::init()
{
  //USBD_Init -> USBD_LL_Init -> HAL_PCD_Init ->(cb) HAL_PCD_MspInit
  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);//AUDIO_Desc extern from usbd_desc

  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);//USBD_AUDIO_CLASS from usbd_audio

  USBD_AUDIO_RegisterInterface(&USBD_Device, &audio_class_interface);
  
  USBD_Start(&USBD_Device);

  return true;
}

void USBD_error_handler(void)
{
  while(1){
  }
}
