/*
 * DigitalIoPin.cpp
 *
 *  Created on: 8 Nov 2016
 *      Author: Obj.M19-Gloria
 */

#include "ITMclass.h"
#include "board.h"
#include "chip.h"
#include <cstdio>

DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert) {
	pin_num = pin;
	port_num = port;
	operation = IOCON_DIGMODE_EN;
	if (invert) {
			operation = operation | IOCON_INV_EN;
		}
	if (pullup) {
			operation = operation | IOCON_MODE_PULLUP;
	}
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, operation);
	if (input) {
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	} else {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
	/*define general template for activating pins*/
}

bool DigitalIoPin::read(){
	Chip_GPIO_GetPinState(LPC_GPIO, port_num, pin_num);
}

void DigitalIoPin::write(bool value){
	Chip_GPIO_SetPinState(LPC_GPIO, port_num, pin_num, value);
}


DigitalIoPin::~DigitalIoPin() {
	//useless without dynamic memory
}
