
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "Prefix.h"
#include "PrefixRequired.h"
/*! \file Implementation of the state machine 'prefix'
*/

/* prototypes of all internal functions */
static sc_boolean prefix_check_PESAJE_ESPERA_tr0_tr0(const Prefix* handle);
static sc_boolean prefix_check_PESAJE_PESANDO_tr0_tr0(const Prefix* handle);
static void prefix_effect_PESAJE_ESPERA_tr0(Prefix* handle);
static void prefix_effect_PESAJE_PESANDO_tr0(Prefix* handle);
static void prefix_enact_PESAJE_ESPERA(Prefix* handle);
static void prefix_enact_PESAJE_PESANDO(Prefix* handle);
static void prefix_exact_PESAJE_ESPERA(Prefix* handle);
static void prefix_enseq_PESAJE_ESPERA_default(Prefix* handle);
static void prefix_enseq_PESAJE_PESANDO_default(Prefix* handle);
static void prefix_enseq_PESAJE_default(Prefix* handle);
static void prefix_exseq_PESAJE_ESPERA(Prefix* handle);
static void prefix_exseq_PESAJE_PESANDO(Prefix* handle);
static void prefix_exseq_PESAJE(Prefix* handle);
static void prefix_react_PESAJE_ESPERA(Prefix* handle);
static void prefix_react_PESAJE_PESANDO(Prefix* handle);
static void prefix_react_PESAJE__entry_Default(Prefix* handle);
static void prefix_clearInEvents(Prefix* handle);
static void prefix_clearOutEvents(Prefix* handle);


void prefix_init(Prefix* handle)
{
	sc_integer i;

	for (i = 0; i < PREFIX_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = Prefix_last_state;
	}
	
	
	handle->stateConfVectorPosition = 0;

	prefix_clearInEvents(handle);
	prefix_clearOutEvents(handle);

	/* Default init sequence for statechart prefix */
	handle->iface.cPESO = 0;

}

void prefix_enter(Prefix* handle)
{
	/* Default enter sequence for statechart prefix */
	prefix_enseq_PESAJE_default(handle);
}

void prefix_exit(Prefix* handle)
{
	/* Default exit sequence for statechart prefix */
	prefix_exseq_PESAJE(handle);
}

sc_boolean prefix_isActive(const Prefix* handle)
{
	sc_boolean result;
	if (handle->stateConfVector[0] != Prefix_last_state)
	{
		result =  bool_true;
	}
	else
	{
		result = bool_false;
	}
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean prefix_isFinal(const Prefix* handle)
{
   return bool_false;
}

static void prefix_clearInEvents(Prefix* handle)
{
	handle->iface.evTermino_raised = bool_false;
	handle->timeEvents.prefix_PESAJE_ESPERA_tev0_raised = bool_false;
}

static void prefix_clearOutEvents(Prefix* handle)
{
}

void prefix_runCycle(Prefix* handle)
{
	
	prefix_clearOutEvents(handle);
	
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < PREFIX_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Prefix_PESAJE_ESPERA :
		{
			prefix_react_PESAJE_ESPERA(handle);
			break;
		}
		case Prefix_PESAJE_PESANDO :
		{
			prefix_react_PESAJE_PESANDO(handle);
			break;
		}
		default:
			break;
		}
	}
	
	prefix_clearInEvents(handle);
}

void prefix_raiseTimeEvent(const Prefix* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + sizeof(PrefixTimeEvents))
		{
		*(sc_boolean*)evid = bool_true;
	}		
}

sc_boolean prefix_isStateActive(const Prefix* handle, PrefixStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Prefix_PESAJE_ESPERA :
			result = (sc_boolean) (handle->stateConfVector[0] == Prefix_PESAJE_ESPERA
			);
			break;
		case Prefix_PESAJE_PESANDO :
			result = (sc_boolean) (handle->stateConfVector[0] == Prefix_PESAJE_PESANDO
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

void prefixIface_raise_evTermino(Prefix* handle)
{
	handle->iface.evTermino_raised = bool_true;
}


sc_integer prefixIface_get_cPESO(const Prefix* handle)
{
	return handle->iface.cPESO;
}
void prefixIface_set_cPESO(Prefix* handle, sc_integer value)
{
	handle->iface.cPESO = value;
}

/* implementations of all internal functions */

static sc_boolean prefix_check_PESAJE_ESPERA_tr0_tr0(const Prefix* handle)
{
	return handle->timeEvents.prefix_PESAJE_ESPERA_tev0_raised;
}

static sc_boolean prefix_check_PESAJE_PESANDO_tr0_tr0(const Prefix* handle)
{
	return handle->iface.evTermino_raised;
}

static void prefix_effect_PESAJE_ESPERA_tr0(Prefix* handle)
{
	prefix_exseq_PESAJE_ESPERA(handle);
	prefix_enseq_PESAJE_PESANDO_default(handle);
}

static void prefix_effect_PESAJE_PESANDO_tr0(Prefix* handle)
{
	prefix_exseq_PESAJE_PESANDO(handle);
	prefix_enseq_PESAJE_ESPERA_default(handle);
}

/* Entry action for state 'ESPERA'. */
static void prefix_enact_PESAJE_ESPERA(Prefix* handle)
{
	/* Entry action for state 'ESPERA'. */
	prefix_setTimer(handle, (sc_eventid) &(handle->timeEvents.prefix_PESAJE_ESPERA_tev0_raised) , 1, bool_false);
}

/* Entry action for state 'PESANDO'. */
static void prefix_enact_PESAJE_PESANDO(Prefix* handle)
{
	/* Entry action for state 'PESANDO'. */
	prefixIface_aPesar(handle, handle->iface.cPESO);
}

/* Exit action for state 'ESPERA'. */
static void prefix_exact_PESAJE_ESPERA(Prefix* handle)
{
	/* Exit action for state 'ESPERA'. */
	prefix_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.prefix_PESAJE_ESPERA_tev0_raised) );		
}

/* 'default' enter sequence for state ESPERA */
static void prefix_enseq_PESAJE_ESPERA_default(Prefix* handle)
{
	/* 'default' enter sequence for state ESPERA */
	prefix_enact_PESAJE_ESPERA(handle);
	handle->stateConfVector[0] = Prefix_PESAJE_ESPERA;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state PESANDO */
static void prefix_enseq_PESAJE_PESANDO_default(Prefix* handle)
{
	/* 'default' enter sequence for state PESANDO */
	prefix_enact_PESAJE_PESANDO(handle);
	handle->stateConfVector[0] = Prefix_PESAJE_PESANDO;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region PESAJE */
static void prefix_enseq_PESAJE_default(Prefix* handle)
{
	/* 'default' enter sequence for region PESAJE */
	prefix_react_PESAJE__entry_Default(handle);
}

/* Default exit sequence for state ESPERA */
static void prefix_exseq_PESAJE_ESPERA(Prefix* handle)
{
	/* Default exit sequence for state ESPERA */
	handle->stateConfVector[0] = Prefix_last_state;
	handle->stateConfVectorPosition = 0;
	prefix_exact_PESAJE_ESPERA(handle);
}

/* Default exit sequence for state PESANDO */
static void prefix_exseq_PESAJE_PESANDO(Prefix* handle)
{
	/* Default exit sequence for state PESANDO */
	handle->stateConfVector[0] = Prefix_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region PESAJE */
static void prefix_exseq_PESAJE(Prefix* handle)
{
	/* Default exit sequence for region PESAJE */
	/* Handle exit of all possible states (of prefix.PESAJE) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Prefix_PESAJE_ESPERA :
		{
			prefix_exseq_PESAJE_ESPERA(handle);
			break;
		}
		case Prefix_PESAJE_PESANDO :
		{
			prefix_exseq_PESAJE_PESANDO(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state ESPERA. */
static void prefix_react_PESAJE_ESPERA(Prefix* handle)
{
	/* The reactions of state ESPERA. */
	if (prefix_check_PESAJE_ESPERA_tr0_tr0(handle) == bool_true)
	{ 
		prefix_effect_PESAJE_ESPERA_tr0(handle);
	} 
}

/* The reactions of state PESANDO. */
static void prefix_react_PESAJE_PESANDO(Prefix* handle)
{
	/* The reactions of state PESANDO. */
	if (prefix_check_PESAJE_PESANDO_tr0_tr0(handle) == bool_true)
	{ 
		prefix_effect_PESAJE_PESANDO_tr0(handle);
	} 
}

/* Default react sequence for initial entry  */
static void prefix_react_PESAJE__entry_Default(Prefix* handle)
{
	/* Default react sequence for initial entry  */
	prefix_enseq_PESAJE_ESPERA_default(handle);
}


