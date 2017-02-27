/*
 * Sender.h
 *
 *  Created on: 14 Nov 2016
 *      Author: Obj.M19-Gloria
 */

#ifndef SENDER_H_
#define SENDER_H_
#include <string>
#include "ITMclass.h"

void Sleep(int ms);

class Sender {
public:
	Sender(DigitalIoPin *onboard_ptr, DigitalIoPin *gpio_ptr, int wpm_main);
	virtual ~Sender();
	void echo(char *rcvd);
	void echo(std::string echoed);
	void wpm_custom(std::string wpm_input);
	void set(char set[50]);
private:
	DigitalIoPin *onboard_led;
	DigitalIoPin *gpio_led;
	void lookup_table(char rcvd);
	int time_unit;
	int wpm_current;

};
#endif /* SENDER_H_ */
