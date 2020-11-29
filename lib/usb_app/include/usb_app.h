#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void UsbApp_HAL_PCD_IRQHandler(void);


#ifdef __cplusplus
}
#endif


class UsbApp{
    public:
        bool init();
};
