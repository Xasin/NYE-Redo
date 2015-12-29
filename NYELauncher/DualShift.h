/*
 * DualShift.h
 *
 *  Created on: Dec 29, 2015
 *      Author: xasin
 */

#ifndef DUALSHIFT_H_
#define DUALSHIFT_H_

#include <avr/io.h>

class DualShift {
private:

	volatile uint8_t *IOREG;
	uint8_t PINS;

	void bitOut(uint8_t b);
	void activate();
	void shiftOut(uint8_t data);

public:
	uint8_t REG_A=0;
	uint8_t REG_B=0;

	DualShift(volatile uint8_t *port, uint8_t pins);

	void update();
};

#endif /* DUALSHIFT_H_ */
