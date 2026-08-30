/*
 * ChibiOS HAL Configuration for KBD8x_mk3 (STM32F103)
 */

#pragma once

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_8_4_

// Minimal HAL for KBD8x
// Guarded: several of these are already injected via -D on the command
// line based on rules.mk feature flags (e.g. EFL_ENABLE), so a bare
// #define here would be a duplicate-definition error under -Werror.
#ifndef HAL_USE_PAL
#define HAL_USE_PAL        TRUE   // GPIO (essential for matrix)
#endif
#ifndef HAL_USE_I2C
#define HAL_USE_I2C        FALSE  // not used by this board
#endif
#ifndef HAL_USE_USB
#define HAL_USE_USB        TRUE   // USB HID
#endif
#ifndef HAL_USE_ADC
#define HAL_USE_ADC        FALSE
#endif
#ifndef HAL_USE_CAN
#define HAL_USE_CAN        FALSE
#endif
#ifndef HAL_USE_CRY
#define HAL_USE_CRY        FALSE
#endif
#ifndef HAL_USE_DAC
#define HAL_USE_DAC        FALSE
#endif
#ifndef HAL_USE_EFL
#define HAL_USE_EFL        FALSE
#endif
#ifndef HAL_USE_GPT
#define HAL_USE_GPT        FALSE
#endif
#ifndef HAL_USE_I2S
#define HAL_USE_I2S        FALSE
#endif
#ifndef HAL_USE_ICU
#define HAL_USE_ICU        FALSE
#endif
#ifndef HAL_USE_MAC
#define HAL_USE_MAC        FALSE
#endif
#ifndef HAL_USE_PWM
#define HAL_USE_PWM        TRUE   // WS2812 chain (TIM1 PWM+DMA, see mcuconf.h)
#endif
#ifndef HAL_USE_RTC
#define HAL_USE_RTC        FALSE
#endif
#ifndef HAL_USE_SDC
#define HAL_USE_SDC        FALSE
#endif
#ifndef HAL_USE_SERIAL
#define HAL_USE_SERIAL     FALSE
#endif
#ifndef HAL_USE_SPI
#define HAL_USE_SPI        FALSE
#endif
#ifndef HAL_USE_TRNG
#define HAL_USE_TRNG       FALSE
#endif
#ifndef HAL_USE_UART
#define HAL_USE_UART       FALSE
#endif
#ifndef HAL_USE_WDG
#define HAL_USE_WDG        FALSE
#endif
#ifndef HAL_USE_WSPI
#define HAL_USE_WSPI       FALSE
#endif

#include_next <halconf.h>
