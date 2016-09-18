#ifndef _PWM_INTERFACE_H
#define _PWM_INTERFACE_H

class PwmInterface {
public:
	virtual uint8_t getPwmPercent() = 0;
	virtual void setPwmPercent(uint8_t value) = 0;
	virtual ~PwmInterface() {}
};

#endif /*_PWM_INTERFACE_H*/
