/*
 * main.c
 *
 *  Created on: 22/08/2016
 *      Author: ses
 */

/*
 * **************************************************
 * SYSTEM INCLUDE FILES								*
 * **************************************************
 */
#include <stdio.h>
#include <sys/mman.h>

/*
 * **************************************************
 * APPLICATION INCLUDE FILES						*
 * **************************************************
 */
#include "portable.h"
#include "rt_tasks.h"

int main()
{
	pthread_t baseRateThread;
	pthread_attr_t attr;
	struct sched_param schedParam;

	if (mlockall(MCL_CURRENT | MCL_FUTURE) < 0)
	{
		fprintf(stderr, "Error: mlockall failed - cannot lock application in memory\n");
		return FAILED;
	}

	// Initializations
	InitRTTask();
	InitTask1();
	InitTask2();
	InitTask3();

	// Prepare tasks attributes
	if (pthread_attr_init(&attr) != 0)
	{
		fprintf(stderr, "pthread_attr_init failed\n");
		return FAILED;
	}

	if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
	{
		fprintf(stderr, "pthread_attr_setinheritsched failed\n");
		return FAILED;
	}

	if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0)
	{
		fprintf(stderr, "pthread_attr_setschedpolicy SCHED_FIFO failed\n");
		return FAILED;
	}

	// Task 1
	CreateRTTask(task1, attr, (int16_T) MAX_PRIO - 1, schedParam, 1, TaskRoutine1, NULL);

	// Task 2
	CreateRTTask(task2, attr, (int16_T) MAX_PRIO - 2, schedParam, 2, TaskRoutine2, NULL);

	// Task 3
	CreateRTTask(task3, attr, (int16_T) MAX_PRIO - 3, schedParam, 4, TaskRoutine3, NULL);

	// Base rate
	schedParam.sched_priority = MAX_PRIO;

	if (pthread_attr_setschedparam(&attr, &schedParam) != 0)
	{
		fprintf(stderr, "pthread_attr_setschedparam failed for base rate\n");
		return FAILED;
	}

	if (pthread_create(&baseRateThread, &attr, BaseRate, NULL) != 0)
	{
		fprintf(stderr, "cannot start the real-time task\n");
		return FAILED;
	}

	pthread_attr_destroy(&attr);

	// Wait for threads to finish
	pthread_join(baseRateThread, NULL);
	pthread_join(task1.thread, NULL);
	pthread_join(task2.thread, NULL);
	pthread_join(task3.thread, NULL);

	return PASSED;

} // END: main()

// EOF: main.c
