/*
 * LedNode.cpp
 *
 *  Created on: 2016. szept. 15.
 *      Author: peti
 */

#include "LedNode.h"

static const char* ledNames[] = {
		"GREEN", "ORANGE", "RED", "BLUE"
};

GPIO_TypeDef* ledPorts[] = {
		GPIOD, GPIOD, GPIOD, GPIOD // :)
};

uint16_t ledPins[] = {
		GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
};

MK_PROP_BOOL_RW(LedNode, Enabled, "LED is On or Off.");

PROP_ARRAY(props) = {
		PROP_ADDRESS(LedNode, Enabled),
};

LedNode::LedNode(LedType type): Node(ledNames[type]) {
	this->gpio_port = ledPorts[type];
	this->gpio_pin = ledPins[type];
	NODE_SET_PROPS(props);
}

LedNode::~LedNode() {
}

void LedNode::init() {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Pin = this->gpio_pin;
	HAL_GPIO_Init(this->gpio_port, &GPIO_InitStruct);
}

ProtocolResult_t LedNode::getEnabled(bool* dest) const {
	GPIO_PinState state = HAL_GPIO_ReadPin(this->gpio_port, this->gpio_pin);
	*dest = (state == GPIO_PIN_SET);
	return ProtocolResult_Ok;
}

ProtocolResult_t LedNode::setEnabled(const bool value) {
	HAL_GPIO_WritePin(this->gpio_port, this->gpio_pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
	return ProtocolResult_Ok;
}
