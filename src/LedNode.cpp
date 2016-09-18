/*
 * LedNode.cpp
 *
 *  Created on: 2016. szept. 15.
 *      Author: peti
 */

#include "LedNode.h"
#include "PwmInterface.h"

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
MK_PROP_UINT32_RW(LedNode, Pwm, "PWM pulse width for this LED.");

PROP_ARRAY(props) = {
		PROP_ADDRESS(LedNode, Enabled),
		PROP_ADDRESS(LedNode, Pwm)
};

LedNode::LedNode(LedType type, PwmInterface* pwm):
		Node(ledNames[type], "PWM controller for the Discovery board's LED's.") {
	this->gpio_port = ledPorts[type];
	this->gpio_pin = ledPins[type];
	this->pwm = pwm;
	NODE_SET_PROPS(props);
}

LedNode::~LedNode() {
}

void LedNode::globalInit() {
	// All debug led's are on GPIOD on the STM32F4 Discovery board
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

void LedNode::init(uint32_t alternateFunction) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = alternateFunction;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Pin = this->gpio_pin;
	HAL_GPIO_Init(this->gpio_port, &GPIO_InitStruct);
}

ProtocolResult_t LedNode::getEnabled(bool* dest) const {
	*dest = (pwm->getPwmPercent() > 0);
	return ProtocolResult_Ok;
}

ProtocolResult_t LedNode::setEnabled(const bool value) {
	pwm->setPwmPercent(value ? 100 : 0);
	this->invalidateProperty(&prop_Pwm);
	return ProtocolResult_Ok;
}

ProtocolResult_t LedNode::getPwm(uint32_t* dest) const {
	*dest = pwm->getPwmPercent();
	return ProtocolResult_Ok;
}

ProtocolResult_t LedNode::setPwm(uint32_t value) {
	uint8_t origValue = pwm->getPwmPercent();
	pwm->setPwmPercent(value);

	if ((origValue == 0 && value != 0) || (origValue != 0 && value == 0)) {
		this->invalidateProperty(&prop_Enabled);
	}
	return ProtocolResult_Ok;
}
