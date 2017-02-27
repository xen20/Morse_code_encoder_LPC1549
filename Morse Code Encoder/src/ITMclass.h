/*
 * DigitalIoPin.h
 *
 *  Created on: 8 Nov 2016
 *      Author: Obj.M19-Gloria
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_
#include <stdint.h>
#include "board.h"

class DigitalIoPin {
public:
    DigitalIoPin(int port, int pin, bool input = true, bool pullup = true, bool invert = false);
    virtual ~DigitalIoPin();
    bool read();
    void write(bool value);
private:
    int pin_num;
    int port_num;
    uint8_t operation;
};

#endif /* DIGITALIOPIN_H_ */
