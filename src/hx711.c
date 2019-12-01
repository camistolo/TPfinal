/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-04-26
 */



/*==================[inclusions]=============================================*/

#include "board.h"
#include "chip.h"
#include "sapi.h"       // <= sAPI header

/* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;


/*==================[macros and definitions]=================================*/

#define dataport	2
#define datapin		8
#define clockport	3
#define clockpin	7

#define owOUT(port,pin)		Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, port, pin)
#define owIN(port,pin)		Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, port, pin)
#define owREAD(port,pin)	Chip_GPIO_GetPinState(LPC_GPIO_PORT, port, pin)
#define owLOW(port,pin)		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, port, pin)
#define owHIGH(port,pin)	Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, port, pin)

#define SCALE		17000

/*==================[internal data declaration]==============================*/

volatile unsigned long OFFSET = 0;
//volatile bool SysTick_Time_Flag = false;
static volatile uint32_t * DWT_CTRL = (uint32_t *)0xE0001000;
static volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/



/*==================[internal functions definition]==========================*/
/*
void SysTick_Handler(void)
{
	SysTick_Time_Flag = true;
}*/

static void pauses(uint32_t t)
{
    *DWT_CTRL |= 1;
	*DWT_CYCCNT = 0;
	t *= (SystemCoreClock/1000000);
	//t *= (SystemCoreClock / 1000);
	while(*DWT_CYCCNT < t);

	//delayUs(t);
	//gpioWrite( LED2, true);
}

void owInit(void)
{
	/* Init cycle counter */
   *DWT_CTRL |= 1;

    /* Init pin P0.20 */
 /*   Chip_IOCON_PinMux(LPC_IOCON, 0, 20,
    		IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN,
			IOCON_FUNC0);*/
    //Chip_SCU_PinMux(pin,port,SCU_MODE_PULLUP | SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_ZIF_DIS, SCU_MODE_FUNC0 );
    Chip_SCU_PinMux(dataport,datapin,SCU_MODE_INACT  | SCU_MODE_ZIF_DIS, SCU_MODE_FUNC0 );

    owIN(dataport,datapin);

    Chip_SCU_PinMux(clockport,clockpin,SCU_MODE_INACT  | SCU_MODE_ZIF_DIS, SCU_MODE_FUNC0 );

    owOUT(clockport,clockpin);
}


void wait_ready(void) {
	// Wait for the chip to become ready.
	// This is a blocking implementation and will
	// halt the sketch until a load cell is connected.
	while (owREAD(dataport,datapin)) {
		pauses(1);
	}
}



unsigned long read_count(void)
{
	unsigned long Count;
	unsigned char i;

	owHIGH(dataport,datapin);
	owLOW(clockport,clockpin);
	Count=0;

	wait_ready();

	for (i=0;i<24;i++)
	{
		owHIGH(clockport,clockpin);
		Count=Count<<1;
		pauses(1);
		owLOW(clockport,clockpin);
		pauses(1);
		if(owREAD(dataport,datapin)){
			Count++;
		}
	}
	owHIGH(clockport,clockpin);
	Count=Count^0x800000;
	pauses(1);
	owLOW(clockport,clockpin);
	return(Count);
}


unsigned long read_average(int times) {
	unsigned long sum = 0;
	for (int i = 0; i < times; i++) {
		sum += read_count();
		// Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
		// https://github.com/bogde/HX711/issues/73
		//delay(0);
		pauses(0);
	}
	return sum / times;
}

double get_value(int times) {
	return read_average(times) - OFFSET;
}

float get_units(int times) {
	float scale = get_value(times) / SCALE;

	if (scale >=300){
		return -1;
	}
	else{
		return scale;
	}
}

void tare(int times) {
	double sum = read_average(times);
	set_offset(sum);
}

void set_offset(double offset) {
	OFFSET = offset;
}


/*
int main (void){

	/* Inicializar la placa */
/*	boardConfig();
	gpioConfig(GPIO7,GPIO_INPUT);
	gpioConfig(GPIO8,GPIO_OUTPUT);

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000000);

	owInit();

	/* UART for debug messages. */
/*	debugPrintConfigUart( UART_USB, 9600 );
	debugPrintString( "DEBUG c/sAPI\r\n" );

	tare(10);

	while(1){
		if(get_units(10) != -1){
			debugPrintInt(get_units(10));
			debugPrintString("\r\n");
		}

	}
	return 0;
}
*/
