#pragma once

/* Includes ------------------------------------------------------------------*/
#include <ch32v00x.h>

#ifdef __cplusplus
extern "C" {
#endif

#define millis() getCurrentMillis()


uint32_t getCurrentMillis(void);
uint32_t getCurrentMicros(void);
void systick_init(void);
void delay(int32_t ms);



#ifdef __cplusplus
}
#endif
