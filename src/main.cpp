#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include <system_stm32f4xx.h>
#include <stm32f4xx_hal.h>

#include <mprotocol-server/ProtocolParser.h>
#include <mprotocol-server/StdioSerialInterface.h>
#include <mprotocol-nodes/RootNode.h>
#include "LedNode.h"

int main() {
  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  // All debug led's are on GPIOD
  __HAL_RCC_GPIOD_CLK_ENABLE();

  RootNode* root = RootNode::getInstance();
  LedNode* led;

  led = new LedNode(LedNode::LedType_Green);
  led->init();
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Orange);
  led->init();
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Red);
  led->init();
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Blue);
  led->init();
  root->addChild(led);

  AbstractSerialInterface* serialInterface = new StdioSerialInterface();
  ProtocolParser* pparser = new ProtocolParser(serialInterface);

  serialInterface->listen();

  delete pparser;
  delete serialInterface;

  return 0;
}

extern "C"
void SysTick_Handler(void) {
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
}
