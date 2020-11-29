#include "cube_main.h"
#include "led_app.h"
#include "usb_app.h"

LedApp led;

UsbApp usb;

void setup()
{
  led.init(GPIOA,GPIO_PIN_1);
  usb.init();
}

void loop()
{
  led.on();
  HAL_Delay(100);
  led.off();
  HAL_Delay(900);
}
