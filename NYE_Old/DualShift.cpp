/*
 * DualShift.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: xasin
 */

#include "DualShift.h"

void DualShift::bitOut(uint8_t b) {
	if(b != 0)
		*(IOREG) |= (1<<PINS);
	*(IOREG) |= (1 << (PINS + 1));

	*(IOREG) &= ~(0b11 << PINS);
}

void DualShift::activate() {
	*(IOREG) |= (1 << (PINS +2));
	*(IOREG) &= ~(1 << (PINS +2));
}

void DualShift::shiftOut(uint8_t data) {
	for(uint8_t i=0; i<8; i++) {
		this->bitOut((data & 0b10000000));
		data = (data << 1);
	}
}

DualShift::DualShift(volatile uint8_t *port, uint8_t pins) {
	this->IOREG = port;
	*(port -1) |= (0b111 << pins);

	this->PINS = pins;
}

void DualShift::update() {
	this->shiftOut(this->REG_B);
	this->shiftOut(this->REG_A);
	this->activate();
}
