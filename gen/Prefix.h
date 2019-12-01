
#ifndef PREFIX_H_
#define PREFIX_H_

#include "sc_types.h"
		
#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'prefix'.
*/

/*! Enumeration of all states */ 
typedef enum
{
	Prefix_PESAJE_ESPERA,
	Prefix_PESAJE_PESANDO,
	Prefix_last_state
} PrefixStates;

/*! Type definition of the data structure for the PrefixIface interface scope. */
typedef struct
{
	sc_integer cPESO;
	sc_boolean evTermino_raised;
} PrefixIface;

/*! Type definition of the data structure for the PrefixTimeEvents interface scope. */
typedef struct
{
	sc_boolean prefix_PESAJE_ESPERA_tev0_raised;
} PrefixTimeEvents;


/*! Define dimension of the state configuration vector for orthogonal states. */
#define PREFIX_MAX_ORTHOGONAL_STATES 1

/*! 
 * Type definition of the data structure for the Prefix state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	PrefixStates stateConfVector[PREFIX_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	PrefixIface iface;
	PrefixTimeEvents timeEvents;
} Prefix;

/*! Initializes the Prefix state machine data structures. Must be called before first usage.*/
extern void prefix_init(Prefix* handle);

/*! Activates the state machine */
extern void prefix_enter(Prefix* handle);

/*! Deactivates the state machine */
extern void prefix_exit(Prefix* handle);

/*! Performs a 'run to completion' step. */
extern void prefix_runCycle(Prefix* handle);

/*! Raises a time event. */
extern void prefix_raiseTimeEvent(const Prefix* handle, sc_eventid evid);

/*! Gets the value of the variable 'cPESO' that is defined in the default interface scope. */ 
extern sc_integer prefixIface_get_cPESO(const Prefix* handle);
/*! Sets the value of the variable 'cPESO' that is defined in the default interface scope. */ 
extern void prefixIface_set_cPESO(Prefix* handle, sc_integer value);
/*! Raises the in event 'evTermino' that is defined in the default interface scope. */ 
extern void prefixIface_raise_evTermino(Prefix* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean prefix_isActive(const Prefix* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean prefix_isFinal(const Prefix* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean prefix_isStateActive(const Prefix* handle, PrefixStates state);

#ifdef __cplusplus
}
#endif 

#endif /* PREFIX_H_ */
