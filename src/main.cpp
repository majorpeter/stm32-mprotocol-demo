#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include <system_stm32f4xx.h>
#include <stm32f4xx_hal.h>

#include <mprotocol-server/ProtocolParser.h>
#include <mprotocol-server/StdioSerialInterface.h>
#include <mprotocol-nodes/RootNode.h>
#include "LedNode.h"
#include "TimerNode.h"

int main() {
  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  LedNode::globalInit();

  RootNode* root = RootNode::getInstance();
  LedNode* led;

  __HAL_RCC_TIM4_CLK_ENABLE();
  TimerNode* tim4 = new TimerNode("TIM4", TIM4);
  tim4->init();
  root->addChild(tim4);

  led = new LedNode(LedNode::LedType_Green, tim4->getChannel(0));
  led->init(GPIO_AF2_TIM4);
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Orange, tim4->getChannel(1));
  led->init(GPIO_AF2_TIM4);
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Red, tim4->getChannel(2));
  led->init(GPIO_AF2_TIM4);
  root->addChild(led);
  led = new LedNode(LedNode::LedType_Blue, tim4->getChannel(3));
  led->init(GPIO_AF2_TIM4);
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
