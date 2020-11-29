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
  HAL_PCD_MspInit_FS_no_cb();//so that HAL_PCD_MspInit can be ignored as it cannot link from a library
  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);//AUDIO_Desc extern from usbd_desc

  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);//USBD_AUDIO_CLASS from usbd_audio

  USBD_AUDIO_RegisterInterface(&USBD_Device, &audio_class_interface);
  
  USBD_Start(&USBD_Device);

  return true;
}

//called from lib\pio_cubemx_stm32f4\cubemx\Core\Src\stm32f4xx_it.c
void UsbApp_HAL_PCD_IRQHandler()
{
  HAL_PCD_IRQHandler(static_cast<PCD_HandleTypeDef*>(USBD_Device.pData));// == &hpcd
}

void USBD_error_handler(void)
{
  while(1){
  }
}

void HAL_PCD_MspInit_FS_no_cb()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  //if(hpcd->Instance==USB_OTG_FS)//not needed as FS used unconditionally

  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**USB_OTG_FS GPIO Configuration
  PA8     ------> USB_OTG_FS_SOF
  PA11     ------> USB_OTG_FS_DM
  PA12     ------> USB_OTG_FS_DP
  */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral clock enable */
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  //added these two as not yet generated from cubemx
  /* Set USBFS Interrupt priority */
  HAL_NVIC_SetPriority(OTG_FS_IRQn, USB_IRQ_PREPRIO, 0);
  
  /* Enable USBFS Interrupt */
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
