/*
 * Sender.cpp
 *
 *  Created on: 14 Nov 2016
 *      Author: Obj.M19-Gloria
 */

#include "Sender.h"
#include "ITMclass.h"
#include "chip.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

Sender::Sender(DigitalIoPin *onboard_ptr, DigitalIoPin *gpio_ptr, int wpm_main ) {
	onboard_led = onboard_ptr;
	gpio_led = gpio_ptr;
	wpm_current = wpm_main;
	time_unit = 1000/wpm_current;
}

Sender::~Sender() {
}

void Sender::echo(char *rcvd){
	int len = strlen(rcvd);
	bool space_last = false;
	for (int i=0; i<len; i++) {
		if (rcvd[i] == ' ') {
			if (space_last) {
				continue;
			} else {
				space_last = true;
				Sleep(time_unit*4);
			}
		} else {
			space_last = false;
			lookup_table(rcvd[i]);
			Sleep(time_unit*2);
		}
	}
}

void Sender::echo(std::string echoed){
	std::string::iterator idx;
		bool space_last = false;
		for (idx = echoed.begin(); idx < echoed.end(); idx++) {
			if (isspace(*idx)) {
				if (space_last) {
					continue;
				} else {
					space_last = true;
					Sleep(time_unit*4);
				}
			} else {
				space_last = false;
				lookup_table(*idx);
				Sleep(time_unit*2);
			}
		}
}

void Sender::lookup_table(char rcvd){
		int k, i;
		const int DOT = 1;
		const int DASH = 3;
		struct MorseCode {
			char symbol;
			unsigned char code[7];
		};
		const MorseCode ITU_morse[] = {
			{ 'A', { DOT, DASH } }, // A
			{ 'B', { DASH, DOT, DOT, DOT } }, // B
			{ 'C', { DASH, DOT, DASH, DOT } }, // C
			{ 'D', { DASH, DOT, DOT } }, // D
			{ 'E', { DOT } }, // E
			{ 'F', { DOT, DOT, DASH, DOT } }, // F
			{ 'G', { DASH, DASH, DOT } }, // G
			{ 'H', { DOT, DOT, DOT, DOT } }, // H
			{ 'I', { DOT, DOT } }, // I
			{ 'J', { DOT, DASH, DASH, DASH } }, // J
			{ 'K', { DASH, DOT, DASH } }, // K
			{ 'L', { DOT, DASH, DOT, DOT } }, // L
			{ 'M', { DASH, DASH } }, // M
			{ 'N', { DASH, DOT } }, // N
			{ 'O', { DASH, DASH, DASH } }, // O
			{ 'P', { DOT, DASH, DASH, DOT } }, // P
			{ 'Q', { DASH, DASH, DOT, DASH } }, // Q
			{ 'R', { DOT, DASH, DOT } }, // R
			{ 'S', { DOT, DOT, DOT } }, // S
			{ 'T', { DASH } }, // T
			{ 'U', { DOT, DOT, DASH } }, // U
			{ 'V', { DOT, DOT, DOT, DASH } }, // V
			{ 'W', { DOT, DASH, DASH } }, // W
			{ 'X', { DASH, DOT, DOT, DASH } }, // X
			{ 'Y', { DASH, DOT, DASH, DASH } },  // Y
			{ 'Z', { DASH, DASH, DOT, DOT } }, // Z
			{ '1', { DOT, DASH, DASH, DASH, DASH } }, // 1
			{ '2', { DOT, DOT, DASH, DASH, DASH } }, // 2
			{ '3', { DOT, DOT, DOT, DASH, DASH } }, // 3
			{ '4', { DOT, DOT, DOT, DOT, DASH } }, // 4
			{ '5', { DOT, DOT, DOT, DOT, DOT } }, // 5
			{ '6', { DASH, DOT, DOT, DOT, DOT } }, // 6
			{ '7', { DASH, DASH, DOT, DOT, DOT } }, // 7
			{ '8', { DASH, DASH, DASH, DOT, DOT } }, // 8
			{ '9', { DASH, DASH, DASH, DASH, DOT } }, // 9
			{ '0', { DASH, DASH, DASH, DASH, DASH } }, // 0
			{ 0, { 0 } } // terminating entry - Do not remove!
		};

		if (rcvd < '0' || (rcvd > '9' && rcvd < 'A') || rcvd > 'Z') rcvd = 'X';

		for (k=0;ITU_morse[k].symbol!=0; k++) {
			if (ITU_morse[k].symbol == rcvd) {
				for (i=0; ITU_morse[k].code[i]!=0; i++) {
					Chip_GPIO_SetPinState(LPC_GPIO, 0, 25, false);
					Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, true);
					Sleep(time_unit * ITU_morse[k].code[i]);
					Chip_GPIO_SetPinState(LPC_GPIO, 0, 25, true);
					Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, false);
					Sleep(time_unit);
				}
			}
}
}

void Sender::wpm_custom(std::string wpm_custom){
	wpm_current = atoi(wpm_custom.c_str());
	time_unit = 1000/wpm_current;
}

void Sender::set(char set[50]){
	snprintf(set, 50, "WPM set at %d, dot length %dms", wpm_current, time_unit);
}
