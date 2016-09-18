/*
 * LedNode.h
 *
 *  Created on: 2016. szept. 15.
 *      Author: peti
 */

#ifndef LEDNODE_H_
#define LEDNODE_H_

#include <mprotocol-nodes/Node.h>
#include <stm32f4xx_hal.h>

class PwmInterface;

class LedNode: public Node {
public:
	enum LedType {
		LedType_Green,
		LedType_Orange,
		LedType_Red,
		LedType_Blue
	};

	LedNode(LedType type, PwmInterface* pwm);
	virtual ~LedNode();

	static void globalInit();
	void init(uint32_t alternateFunction);

	DECLARE_PROP_BOOL_RW(Enabled);
	DECLARE_PROP_UINT32_RW(Pwm);
private:
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_pin;
	PwmInterface* pwm;
};

#endif /* LEDNODE_H_ */
