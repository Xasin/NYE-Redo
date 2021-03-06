/*
 * main.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: xasin
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "DualShift.h"

#define STOP -1

#define _MS / 200
#define _S  * 5
#define _M _S * 60
#define _H _M * 60

#define MID(T) PRE_TIME + (T)

DualShift output(&PORTA, 0);

void setOutput(uint8_t n) {

#define X 8
#define Y 3

	if(n < (X*Y)) {
		output.REG_A = (1 << (n % X));
		output.REG_B = ~(1 << (n / X));
		output.update();
	}
	else {
		output.REG_A = 0;
		output.REG_B = 0xff;
		output.update();
	}
}

uint8_t prscL = 0;
uint8_t prsc=0;
uint8_t currentStep = 0;
int32_t current10MS = 0;

#define PRE_TIME 16 _M

static int32_t scedule[] = {
		5 _S,

		STOP
};

ISR(TIM0_COMPA_vect) {
	if(++prsc == 200) {
		prsc = 0;

		if(++prscL == 200 _MS) {
			PORTA ^= (1<< 7);
			prscL = 0;
		}

		current10MS++;

		if(scedule[currentStep] == current10MS)
			setOutput(currentStep++);
		else if(scedule[currentStep] == -1) {
			PORTA &=~ (1<< 7);
			setOutput(0xff);
		}
		else
			setOutput(0xff);
	}
}

int main() {

	DDRA |= (1<< 7);
	PORTA |= (1<< 3);

	// /8 prescaler + CTC Mode
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01);
	OCR0A = 250 -1;
	TIMSK0 |= (1<< OCIE0A);

	while((PINA & (1<< 3)) != 0) {
		_delay_ms(25);
		PORTA ^= (1<< 7);
	}

	sei();

	while(true) {
	}

	return 0;
}
