# pio_cubemx_stm32f4
CubeMX project integration in platformio as a library for the Black pill board with STM32F4
The goal is to habe a .ioc project that can regenerate the library and easily keep using it from a pio project

# steps required after regeneration
although the goal would be to have no manual steps, currently still some actions are required
* delete Drivers folder
* merge `main.c`
* revert `main.h`

TODO take the main outside and generate without main
TODO check generation with Makefile project

# generated HAL vs pio HAL
* `stm32f4xx_hal_conf.h` in `.platformio\packages\framework-stm32cube\f4\Drivers\STM32F4xx_HAL_Driver\Inc\`
* pio HAL has everything enabled
* `main()` -> `HAL_Init()` -> __weak callback `HAL_MspInit()`
* required generated files :
    * `main.c`
    * `stm32f4xx_hal_msp.c`
    * `stm32f4xx_it.c`

# SysTick_Handler issue
## issue
* did not manage to get `SysTick_Handler` to get called
## solution
* despite the file `stm32f4xx_it.c` getting compiled and the source containing `SysTick_Handler()` function, the function was beaving like non existing without any complaints
* placing a duplicate `SysTick_Handler()` in `main.c` resolves the problem
## problem
* unsolved mysteries
    * why does the linker not complains about multiple instances ?
    * why is the instance of `stm32f4xx_it.c` not considered ?
