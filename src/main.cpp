#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include <system_stm32f4xx.h>
#include <stm32f4xx_hal.h>

int main() {
  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  while(1);

  return 0;
}

extern "C"
void SysTick_Handler(void) {
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
}
