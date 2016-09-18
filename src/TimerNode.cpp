/*
 * TimerNode.cpp
 *
 *  Created on: 2016. szept. 16.
 *      Author: peti
 */

#include "TimerNode.h"

MK_PROP_BOOL_RW(TimerNode, Enabled, "Enable counter.");
MK_PROP_UINT32_RW(TimerNode, MaxCount, "Maximum counter value (auto reset).");
MK_PROP_UINT32_RW(TimerNode, Pulse1, "PWM pulse width for TIM channel 1.");
MK_PROP_UINT32_RW(TimerNode, Pulse2, "PWM pulse width for TIM channel 2.");
MK_PROP_UINT32_RW(TimerNode, Pulse3, "PWM pulse width for TIM channel 3.");
MK_PROP_UINT32_RW(TimerNode, Pulse4, "PWM pulse width for TIM channel 4.");

PROP_ARRAY(props) = {
		PROP_ADDRESS(TimerNode, Enabled),
		PROP_ADDRESS(TimerNode, MaxCount),
		PROP_ADDRESS(TimerNode, Pulse1),
		PROP_ADDRESS(TimerNode, Pulse2),
		PROP_ADDRESS(TimerNode, Pulse3),
		PROP_ADDRESS(TimerNode, Pulse4)
};

TimerNode::TimerNode(const char* name, TIM_TypeDef* timer): Node(name) {
	this->timer.Instance = timer;
	NODE_SET_PROPS(props);
}

TimerNode::~TimerNode() {
}

void TimerNode::init() {
	timer.Init.Prescaler = 0;
	timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer.Init.Period = 10000;
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timer.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&timer);

	TIM_OC_InitTypeDef OC_InitStruct;
	OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
	OC_InitStruct.Pulse = 100;
	OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
	OC_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&timer, &OC_InitStruct, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&timer, &OC_InitStruct, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&timer, &OC_InitStruct, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&timer, &OC_InitStruct, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_4);
}

TimerNode::Channel* TimerNode::getChannel(uint8_t index) {
	if (index >= 4) {
		return NULL;
	}
	return new TimerNode::Channel(this, index);
}

/**
 * @see __HAL_TIM_ENABLE(__HANDLE__) macro
 */
ProtocolResult_t TimerNode::getEnabled(bool* dest) const {
	*dest = timer.Instance->CR1 & TIM_CR1_CEN;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setEnabled(bool value) {
	if (value) {
		timer.Instance->CR1 |= TIM_CR1_CEN;
	} else {
		timer.Instance->CR1 &= ~TIM_CR1_CEN;
	}
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::getMaxCount(uint32_t* dest) const {
	*dest = timer.Instance->ARR;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setMaxCount(uint32_t value) {
	timer.Instance->ARR = value;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::getPulse1(uint32_t* dest) const {
	*dest = timer.Instance->CCR1;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setPulse1(uint32_t value) {
	timer.Instance->CCR1 = value;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::getPulse2(uint32_t* dest) const {
	*dest = timer.Instance->CCR2;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setPulse2(uint32_t value) {
	timer.Instance->CCR2 = value;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::getPulse3(uint32_t* dest) const {
	*dest = timer.Instance->CCR3;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setPulse3(uint32_t value) {
	timer.Instance->CCR3 = value;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::getPulse4(uint32_t* dest) const {
	*dest = timer.Instance->CCR4;
	return ProtocolResult_Ok;
}

ProtocolResult_t TimerNode::setPulse4(uint32_t value) {
	timer.Instance->CCR4 = value;
	return ProtocolResult_Ok;
}

TimerNode::Channel::Channel(TimerNode* node, uint8_t index) {
	this->node = node;
	this->index = index;
}

uint8_t TimerNode::Channel::getPwmPercent() {
	uint32_t value = 0, max = 0;
	node->getMaxCount(&max);
	switch (index) {
	case 0: node->getPulse1(&value); break;
	case 1: node->getPulse2(&value); break;
	case 2: node->getPulse3(&value); break;
	case 3: node->getPulse4(&value); break;
	}
	if (max == 0) {
		return 0; // avoid division by zero
	}
	return (value * 100) / max;
}

void TimerNode::Channel::setPwmPercent(uint8_t value) {
	uint32_t max = 0;
	node->getMaxCount(&max);
	uint32_t pulse = (value * max) / 100;
	switch (index) {
	case 0:
		node->setPulse1(pulse);
		node->invalidateProperty(&prop_Pulse1);
		break;
	case 1:
		node->setPulse2(pulse);
		node->invalidateProperty(&prop_Pulse2);
		break;
	case 2:
		node->setPulse3(pulse);
		node->invalidateProperty(&prop_Pulse3);
		break;
	case 3:
		node->setPulse4(pulse);
		node->invalidateProperty(&prop_Pulse4);
		break;
	}
}
