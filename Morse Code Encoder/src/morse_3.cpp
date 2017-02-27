/*
===============================================================================
 Name        : main.c
 Author      : Konstantin Mishin
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "Sender.h"
#include "ITMclass.h"
#include <cctype>
#include <string>

static volatile int counter;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Handle interrupt from SysTick timer
 * @return Nothing
 */
void SysTick_Handler(void)
{
 if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
 counter = ms;
 while(counter > 0) {
  __WFI();
 }
}

// TODO: insert other include files here
#define TICKRATE_HZ1 (1000)

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
#endif
#endif

    uint32_t sysTickRate;
    Chip_Clock_SetSysTickClockDiv(1);
    sysTickRate = Chip_Clock_GetSysTickClockRate();
    SysTick_Config(sysTickRate/TICKRATE_HZ1);

    DigitalIoPin onboard(0,25, false, false, false);
    DigitalIoPin gpio(0,8, false, false, false); //for some reason LED's only work properly only if init = false

    int wpm_initial = 10;
    char set[50] = {0};
    std::string echoed = "";
    Sender operate(&onboard, &gpio, wpm_initial); //adress of objects pushed to Sender class constructor
    int rcvd = 0;

    Board_UARTPutSTR("\n\rWPM (N) for new WPM\n\rSEND (STR) to send\n\rSET to get WPM and dot length\n\r");
    while(1) {
    	rcvd = '0'; //initalizer - otherwise program hangs
    	while(rcvd != '\r' && echoed.length() <= 80){
    		rcvd = Board_UARTGetChar();
    		rcvd = toupper(rcvd);
    		if(rcvd != EOF){
    			Board_UARTPutChar(rcvd);
    			echoed.push_back(rcvd);
    		}
    		if(rcvd == '\r' || echoed.length() == 80 ){
    			if(echoed.find("SET") == 0){
    				echoed.erase(0,3);
    	    		operate.set(set);
    	    		Board_UARTPutSTR(set);
    	    	}
    	    	if(echoed.find("WPM ") == 0){
    	    		echoed.erase(0,4);
    	    		operate.wpm_custom(echoed);
    	    	}
    	    	if(echoed.find("SEND ") == 0){
    	    		echoed.erase(0,5);
        			Board_UARTPutChar('\r');
        			operate.echo(echoed);
        			echoed.erase(0,80);
    	    	}
    	    	echoed.erase(0,80);
    	    	Board_UARTPutSTR("\n\n\rWPM (N) for new WPM\n\rSEND (STR) to send\n\rSET to get WPM and dot length\n\r");
    		}
    	}

    }
    return 0 ;
}
