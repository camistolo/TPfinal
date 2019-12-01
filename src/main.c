/* Copyright 2017, Pablo Ridolfi, Juan Esteban Alarcón, Juan Manuel Cruz
 * All rights reserved.
 *
 * This file is part of Workspace.
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
 
/** @brief This is a simple statechart example using Yakindu Statechart Tool
 * Plug-in (update site: http://updates.yakindu.org/sct/mars/releases/).
 */

/** \addtogroup statechart Simple UML Statechart example.
 ** @{ */

/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"       // <= sAPI header

/* Include statechart header file. Be sure you run the statechart C code
 * generation tool!
 */
#include "Prefix.h"
#include "TimerTicks.h"
#include "hx711.h"

/* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;


/*==================[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */

/*==================[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false;

/*! This is a state machine */
static Prefix statechart;


/* Select a TimeEvents choise	*/
//#define __USE_TIME_EVENTS (false)	/* "false" without TimeEvents */
#define __USE_TIME_EVENTS (true)	/* or "true" with TimerEvents */

/*! This is a timed state machine that requires timer services */
#if (__USE_TIME_EVENTS == true)
	#define NOF_TIMERS (sizeof(PrefixTimeEvents)/sizeof(sc_boolean))
#else
	#define NOF_TIMERS 0
#endif

TimerTicks ticks[NOF_TIMERS];


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*! \file This header defines prototypes for all functions that are required by the state machine implementation.

This is a state machine uses time events which require access to a timing service. Thus the function prototypes:
	- prefix_setTimer and
	- prefix_unsetTimer
are defined.

This state machine makes use of operations declared in the state machines interface or internal scopes. Thus the function prototypes:
	- prefixIface_opLED
are defined.

These functions will be called during a 'run to completion step' (runCycle) of the statechart.
There are some constraints that have to be considered for the implementation of these functions:
	- never call the statechart API functions from within these functions.
	- make sure that the execution time is as short as possible.

*/
/** state machine user-defined external function (action)
 *
 * @param handle state machine instance
 * @param LEDNumber number of LED
 * @param onoff state machine operation parameter
 */
void prefixIface_aPesar(Prefix* handle, sc_integer cPeso)
{
	tare(10);

	//if(get_units(10) != -1){
		cPeso = get_units(10);
		debugPrintInt(cPeso);
		debugPrintString("\r\n");
	//}

	prefixIface_raise_evTermino(&statechart);

}


/*!
 * This is a timed state machine that requires timer services
 */

#if (__USE_TIME_EVENTS == true)
/*! This function has to set up timers for the time events that are required by the state machine. */
/*!
	This function will be called for each time event that is relevant for a state when a state will be entered.
	\param evid An unique identifier of the event.
	\time_ms The time in milli seconds
	\periodic Indicates the the time event must be raised periodically until the timer is unset
*/
void prefix_setTimer(Prefix* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);
}

/*! This function has to unset timers for the time events that are required by the state machine. */
/*!
	This function will be called for each time event taht is relevant for a state when a state will be left.
	\param evid An unique identifier of the event.
*/
void prefix_unsetTimer(Prefix* handle, const sc_eventid evid)
{
	UnsetTimerTick(ticks, NOF_TIMERS, evid);
}
#endif

void SysTick_Handler(void)
{
	SysTick_Time_Flag = true;
}


/**
 * @brief	Hook on Handle interrupt from SysTick timer
 * @return	Nothing
 */
void myTickHook( void *ptr ){
	SysTick_Time_Flag = true;
}


int main(void)
{
	#if (__USE_TIME_EVENTS == true)
	uint32_t i;
	#endif

	/* Generic Initialization */
	boardConfig();
	gpioConfig(GPIO7,GPIO_INPUT);
	gpioConfig(GPIO8,GPIO_OUTPUT);
	owInit();

	/* UART for debug messages. */
	debugPrintConfigUart( UART_USB, 9600 );
	debugPrintString( "DEBUG c/sAPI\r\n" );

	/* Init Ticks counter => TICKRATE_MS */
	tickConfig( TICKRATE_MS );

	/* Add Tick Hook */
	tickCallbackSet( myTickHook, (void*)NULL );

	/* Statechart Initialization */
	#if (__USE_TIME_EVENTS == true)
	InitTimerTicks(ticks, NOF_TIMERS);
	#endif

	prefix_init(&statechart);
	prefix_enter(&statechart);

	/* LEDs toggle in main */
	while (1) {
		__WFI();

		if (SysTick_Time_Flag == true) {
			SysTick_Time_Flag = false;
			//delay (1000);
			#if (__USE_TIME_EVENTS == true)
			UpdateTimers(ticks, NOF_TIMERS);
			for (i = 0; i < NOF_TIMERS; i++) {
				if (IsPendEvent(ticks, NOF_TIMERS, ticks[i].evid) == true) {

					prefix_raiseTimeEvent(&statechart, ticks[i].evid);	// Event -> Ticks.evid => OK
					MarkAsAttEvent(ticks, NOF_TIMERS, ticks[i].evid);
				}
			}
			#else
			prefixIface_raise_evTick(&statechart);					// Event -> evTick => OK
			#endif

			prefix_runCycle(&statechart);							// Run Cycle of Statechart
		}
	}
}


