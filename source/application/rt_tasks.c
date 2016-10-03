/*
 * tasks.c
 *
 *  Created on: 27/08/2016
 *      Author: ses
 */

/*
 * **************************************************
 * APPLICATION INCLUDE FILES						*
 * **************************************************
 */
#include "rt_tasks.h"


/*
 * **************************************************
 * LOCAL DECLARATIONS								*
 * **************************************************
 */



/*
 * **************************************************
 * GLOBAL VARIABLES (extern)						*
 * **************************************************
 */
task_T 	  task1, task2, task3;
real32_T  baseTs;	// Base rate sample time


/*
 * **************************************************
 * FILE SCOPE VARIABLES (static)					*
 * **************************************************
 */
struct timespec basePeriod;


/*
 * **************************************************
 * LOCAL PROTOTYPES									*
 * **************************************************
 */



/*
 * **************************************************
 * PUBLIC FUNCTIONS									*
 * **************************************************
 */
/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      INITRTTASK()
*      Initialize real-time tasks.
*
*  Inputs:
*
*  Outputs:
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*  -------------------------------------------------------  *
*/
void InitRTTask(void)
{
	basePeriod.tv_sec  = 1U;
	basePeriod.tv_nsec = 0U;

	baseTs = basePeriod.tv_sec + basePeriod.tv_nsec / 1000 / 1000 / 1000;

} // END: InitRTTask()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      CREATERTTASK()
*      Create a real-time task.
*
*  Inputs:
*      *task 		: Pointer to a task structure
*      *attr		: Pointer to a ask attribute
*      prio	   		: Task priority
*      schedParam   : Scheduling parameters
*      TaskRoutine 	: Task function
*      threadArg    : Task argument
*
*  Outputs:
*	   Return 0 when succeeded.
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*  -------------------------------------------------------  *
*/
pf_T CreateRTTask(task_T task,
				  pthread_attr_t attr,
				  int16_T prio,
				  struct sched_param schedParam,
				  uint16_T period,
				  void *(*TaskRoutine) (void *),
				  void *threadArg
				 )
{
	sem_init(&task.sem, 0, 0);

	task.period = period;

	schedParam.sched_priority = prio;

	if (pthread_attr_setschedparam(&attr, &schedParam) != 0)
	{
		fprintf(stderr, "pthread_attr_setschedparam failed for task %u.\n", task1.period);
		return FAILED;
	}

	if (pthread_create(&task.thread, &attr, TaskRoutine, threadArg) != 0)
	{
		fprintf(stderr, "cannot start the real-time task\n");
		return FAILED;
	}

	return PASSED;

} // END: CreateRTTask()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      BASERATE()
*      The base task period.
*
*  Inputs:
*
*  Outputs:
*
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*
*  -------------------------------------------------------  *
*/
void *BaseRate(void *arg)
{
	struct timespec now, next;

	boolean_T task1Flag, task2Flag, task3Flag;
	int32_T   task1Counter = 0, task2Counter = 0, task3Counter = 0;
	int32_T   semVal;

	clock_gettime(CLOCK_MONOTONIC, &next);

	// Main loop, running until all the threads are terminated
	while (1)
	{
		// Set task1 flag
		if (task1Counter)
		{
			if (task1Flag)
			{
				// Sampling too fast
				fprintf(stderr, "Overrun, task1 did not run.\n");
				return NULL;
			}

			task1Flag = TRUE;
		}

		// Set task1 flag
		if (task2Counter)
		{
			if (task2Flag)
			{
				// Sampling too fast
				fprintf(stderr, "Overrun, task2 did not run.\n");
				return NULL;
			}

			task2Flag = TRUE;
		}

		// Set task1 flag
		if (task3Counter)
		{
			if (task3Flag)
			{
				// Sampling too fast
				fprintf(stderr, "Overrun, task3 did not run.\n");
				return NULL;
			}

			task3Flag = TRUE;
		}

		// Check task1 period
		task1Counter++;
	    if (task1Counter == task1.period)
	    {
	    	task1Counter = 0;
	    }

	    task2Counter++;
	    if (task2Counter == task2.period)
	    {
	    	task2Counter= 0;
	    }

	    task3Counter++;
	    if (task3Counter == task3.period)
	    {
	    	task3Counter= 0;
	    }

	    // Trigger task1
	    if (task1Flag)
	    {
	    	task1Flag = FALSE;
	    	sem_getvalue(&task1.sem, &semVal);
	        if (semVal)
	        {
	        	printf("Task1 overrun.\n");
	        	break;
	        }

	        sem_post(&task1.sem);
	        sem_post(&task1.sem);
	    }

	    // Trigger task2
	    if (task2Flag)
	    {
	    	task2Flag = FALSE;
	    	sem_getvalue(&task2.sem, &semVal);
	        if (semVal)
	        {
	        	printf("Task2 overrun.\n");
	        	break;
	        }

	        sem_post(&task2.sem);
	        sem_post(&task2.sem);
	    }

	    // Trigger task3
	    if (task3Flag)
	    {
	    	task3Flag = FALSE;
	    	sem_getvalue(&task3.sem, &semVal);
	        if (semVal)
	        {
	        	printf("Task3 overrun.\n");
	        	break;
	        }

	        sem_post(&task3.sem);
	        sem_post(&task3.sem);
	    }

		do
		{
			next.tv_sec  += basePeriod.tv_sec;
			next.tv_nsec += basePeriod.tv_nsec;
			if (next.tv_nsec >= 1000000000)
			{
				next.tv_sec++;
				next.tv_nsec -= 1000000000;
			}

			clock_gettime(CLOCK_MONOTONIC, &now);

			if (now.tv_sec > next.tv_sec ||	(now.tv_sec == next.tv_sec && now.tv_nsec > next.tv_nsec))
			{
				uint32_T usec = (now.tv_sec - next.tv_sec) * 1000000 + (now.tv_nsec - next.tv_nsec) / 1000;
				fprintf(stderr, "Base rate (0.01s) overrun by %u us\n", usec);
				next = now;
				continue;
			}
		} while (0);

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
	}

	return NULL;

} // END: BaseRate()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      TASKROUTINE1()
*      Task1 periodic call.
*
*  Inputs:
*
*  Outputs:
*
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*
*  -------------------------------------------------------  *
*/
void *TaskRoutine1(void *arg)
{
  while (1) {
    sem_wait(&task1.sem);        /* sem_val = 1 */
    ExecTask1();
    sem_wait(&task1.sem);        /* sem_val = 0 */
  }

  return NULL;

} // END: Task1()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      TASKROUTINE2()
*      Task2 periodic call.
*
*  Inputs:
*
*  Outputs:
*
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*
*  -------------------------------------------------------  *
*/
void *TaskRoutine2(void *arg)
{
  while (1) {
    sem_wait(&task2.sem);        /* sem_val = 1 */
    ExecTask2();
    sem_wait(&task2.sem);        /* sem_val = 0 */
  }

  return NULL;

} // END: Task2()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      TASKROUTINE3()
*      Task3 periodic call.
*
*  Inputs:
*
*  Outputs:
*
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*
*  -------------------------------------------------------  *
*/
void *TaskRoutine3(void *arg)
{
  while (1) {
    sem_wait(&task3.sem);        /* sem_val = 1 */
    ExecTask3();
    sem_wait(&task3.sem);        /* sem_val = 0 */
  }

  return NULL;

} // END: Task3()


/*
 * **************************************************
 * LOCAL FUNCTIONS									*
 * **************************************************
 */
/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      PUBLICFUCTION()
*      What this function is doing.
*
*  Inputs:
*      x : Input
*
*  Outputs:
*      y : Return 0 when succeeded.
*
*  Author: Ehsan Shafiei
*  		   Aug 2016
*  -------------------------------------------------------  *
*/
//static type LocalFunction(type x)
//{
//    return y;
//} // END: LocalFunction()

// EOF: rt_tasks.c
