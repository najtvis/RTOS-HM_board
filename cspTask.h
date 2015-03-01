/*
 * cspTask.h
 *
 * Created: 11.10.2014 20:33:36
 *  Author: Tomas Baca
 */ 

#include "system.h"
#include "queue.h"

#ifndef CSPTASK_H_
#define CSPTASK_H_

extern QueueHandle_t * xCSPEventQueue;

typedef enum {
	
	echoBackEvent = 0,
	freeHeapEvent = 1,
	housKeepingEvent = 2,
	processAllEvent = 3,
	sendSignalEvent = 4,
	sendFFTEvent = 5,
	sendSummaryInfoEvent = 6,
	sendAttenuationEvent = 7
	
} eCSPEvent_t;

typedef struct CSP_TASK_COMMANDS
{
	eCSPEvent_t eEventType; /* Tells the receiving task what the event is. */
	void *pvData; /* Holds or points to any data associated with the event. */

} xCSPStackEvent_t;

/* -------------------------------------------------------------------- */
/*	Task that handles CSP incoming packets								*/
/* -------------------------------------------------------------------- */
void cspTask(void *p);

#endif /* CSPTASK_H_ */