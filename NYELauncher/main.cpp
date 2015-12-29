/*
 * main.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: xasin
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "DualShift.h"

#define TIME(MS, SEC, MIN, HOUR) MS + SEC * 100 + MIN * 60 * 100 + HOUR * 60 * 60 * 100

DualShift output(&PORTB, 0);

uint8_t prsc=0;

uint16_t currentStep = 0;
static uint8_t scedule[] = {
		0
};

uint32_t current10MS = 0;

#define X 5
#define Y 5

void setOutput(uint8_t n) {
	if(n < (X*Y)) {
		output.REG_A = (1 << (n % X));
		output.REG_B = (1 << (n / X));
		output.update();
	}
}

ISR(TIM0_COMPA_vect) {
	if(++prsc == 10) {
		prsc = 0;

		current10MS++;

		if(scedule[currentStep * 2] == current10MS)
			setOutput(scedule[currentStep * 2 +1]);
	}
}

int main() {

	TCCR0A |= (1<< WGM02 | 1<< CS01);
	OCR0A = 250 -1;

	sei();

	while(true) {

	}

	return 0;
}
