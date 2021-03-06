/*
 * tasks.h
 *
 *  Created on: 27/08/2016
 *      Author: ses
 */

#ifndef _RT_TASKS_H_
#define _RT_TASKS_H_

/*
 * **************************************************
 * SYSTEM INCLUDE FILES								*
 * **************************************************
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/*
 * **************************************************
 * APPLICATION INCLUDE FILES						*
 * **************************************************
 */
#include "portable.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

/*
 * **************************************************
 * DEFINITIONS										*
 * **************************************************
 */
// Maximal priority used by base rate thread.
#define MAX_PRIO	(sched_get_priority_min(SCHED_FIFO) + 4)


/*
 * **************************************************
 * MACROS											*
 * **************************************************
 */



/*
 * **************************************************
 * TYPE DEFINITIONS									*
 * **************************************************
 */
typedef struct
{
	sem_t 	  sem;
	pthread_t thread;
	uint16_T  period;	// Coefficient of the base period.
} task_T;

typedef struct
{
	uint32_T clockTickL;
	uint32_T clockTickH;
} timing_T;


/*
 * **************************************************
 * External VARIABLES       						*
 * **************************************************
 */
extern task_T 	 task1, task2, task3;
extern real32_T  baseTs;


/*
 * **************************************************
 * PROTOTYPES										*
 * **************************************************
 */
void InitRTTask(void);

pf_T CreateRTTask(task_T task,
				  pthread_attr_t attr,
				  int16_T prio,
				  struct sched_param schedParam,
				  uint16_T period,
				  void *(*TaskRoutine) (void *),
				  void *threadArg
				 );

void *BaseRate(void *arg);

void *TaskRoutine1(void *arg);
void *TaskRoutine2(void *arg);
void *TaskRoutine3(void *arg);


#endif // _RT_TASKS_H_

// EOF: rt_tasks.h
