/*
 * main.cpp
 *
 *  Created on: Dec 31, 2015
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

DualShift output(&PORTB, 0);

void setOutput(uint8_t n) {

#define X 6
#define Y 6

	if(n < (X*Y)) {
		uint16_t REG = (1 << (n / X) | 1 << (6 + n % X) | 1 << 12);
		output.REG_B = REG & 0x00ff;
		output.REG_A = (REG & 0xff00) >> 8;
		output.update();
	}
	else {
		output.REG_A = 0;
		output.REG_B = 0;
		output.update();
	}
}

uint8_t prscL = 0;
uint16_t prsc=0;
uint8_t currentStep = 0;
int32_t current10MS = 0;

#define PRE_TIME 20 _M

static int32_t scedule[] = {
		MID( - 19 _M - 30 _S),
		MID( - 15 _M),
		MID( - 10 _M),
		MID( - 5 _M),
		MID( - 2 _M - 30 _S),
		MID( - 1 _M),
		MID( - 30 _S),
		MID( - 15 _S),

		//Countdown!!
		MID( - 5 _S),
		MID( - 4 _S),
		MID( - 3 _S),
		MID( - 2 _S),

		//Happy new year owo
		MID( 0 ),

		STOP
};

ISR(TIMER0_COMPA_vect) {
	if(++prsc == 2000) {
		prsc = 0;

		if(++prscL == 200 _MS) {
			PORTD ^= (1<< 2);
			prscL = 0;
		}

		current10MS++;

		if(scedule[currentStep] == current10MS)
			setOutput(currentStep++);
		else if(scedule[currentStep] == -1) {
			PORTD &=~ (1<< 2);
			setOutput(0xff);
		}
		else
			setOutput(0xff);
	}
}

int main() {

	DDRD |= (1<< 2);
	PORTD |= (1<< 1);

	// /8 prescaler + CTC Mode
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01);
	OCR0A = 250 -1;
	TIMSK0 |= (1<< OCIE0A);

	setOutput(0xff);

	while((PIND & (1<< 1)) != 0) {
		_delay_ms(25);
		PORTD ^= (1<< 2);
	}

	sei();

	while(true) {
	}

	return 0;
}
