#include "usb_app.h"
#include "stm32f4xx_hal.h"

#include "usbd_core.h"

USBD_HandleTypeDef USBD_Device;


bool UsbApp::init()
{

  //USBD_Init(&USBD_Device, &AUDIO_Desc, 0);
  USBD_Init(&USBD_Device, NULL, 0);

  return true;
}
