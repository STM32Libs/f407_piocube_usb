#include "cube_main.h"
#include "led_app.h"

LedApp led;

void setup()
{
  led.init(GPIOA,GPIO_PIN_1);
}

void loop()
{
  led.on();
  HAL_Delay(100);
  led.off();
  HAL_Delay(900);
}
