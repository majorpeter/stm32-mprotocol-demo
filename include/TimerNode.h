/*
 * TimerNode.h
 *
 *  Created on: 2016. szept. 16.
 *      Author: peti
 */

#ifndef TIMERNODE_H_
#define TIMERNODE_H_

#include <mprotocol-nodes/Node.h>
#include <stm32f4xx_hal.h>
#include "PwmInterface.h"

class TimerNode: public Node {
public:
	class Channel: public PwmInterface {
	public:
		Channel(TimerNode* node, uint8_t index);
		virtual ~Channel() {}
		virtual uint8_t getPwmPercent();
		virtual void setPwmPercent(uint8_t value);
	private:
		TimerNode* node;
		uint8_t index;
	};

	TimerNode(const char* name, TIM_TypeDef* timer);
	virtual ~TimerNode();
	void init();
	Channel* getChannel(uint8_t index);

	DECLARE_PROP_BOOL_RW(Enabled);
	DECLARE_PROP_UINT32_RW(MaxCount);
	DECLARE_PROP_UINT32_RW(Pulse1);
	DECLARE_PROP_UINT32_RW(Pulse2);
	DECLARE_PROP_UINT32_RW(Pulse3);
	DECLARE_PROP_UINT32_RW(Pulse4);
private:
	TIM_HandleTypeDef timer;
};

#endif /* TIMERNODE_H_ */
