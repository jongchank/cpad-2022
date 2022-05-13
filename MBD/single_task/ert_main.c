/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'Actuate'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sat Nov 20 13:57:12 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>              /* This ert_main.c example uses printf/fflush */
#include "Actuate.h"                   /* Model's header file */
#include "rtwtypes.h"

#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "Sense.h"
#include "Compute.h"
#include "Actuate.h"
#include <unistd.h>
#include <time.h>

static timer_t timer_100ms;

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep_Sense(void);
void rt_OneStep_Compute(void);
void rt_OneStep_Actuate(void);

void rt_OneStep_Sense(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(Sense_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model for base rate */
  Sense_step();

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void rt_OneStep_Compute(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(Compute_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model for base rate */
  Compute_step();

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void rt_OneStep_Actuate(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(Actuate_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model for base rate */
  Actuate_step();

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/* Solver Fixed-step size : 100ms */
/* Calculate the model every 100ms */
/* -----------------------------------------------------------------
/* func: void handler(int sig, siginfo_t *info, void *ucontext)
 * arguments 
 *  - sig: number of signal that invocate handler
 *  - *info: pointer to siginfo
 *  - *ucontext: pointer to ucontext_t
 * ---------------------------------------------------------------- */
static void handler_100ms(int sig, siginfo_t *si, void *uc)
{
    static int c = 0;

    if (*(timer_t *)(si->si_value.sival_ptr) != timer_100ms || sig != SIGRTMIN) {
        printf("Wrong handler\n");
        return;
    }

    /* --------- Write below your code ----------------*/




    /* --------------------- end --------------------- */

    return;
}

/*  ----------------------------------------------------------------------------------------------------------
 * func: int sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact);
 * arguments 
 *  - sig: a signal to process
 *  - act: Structure address to store a method of processing a signal
 *  - oact:  old act
 * 
 * return value
 *  - success: 0 
 *  - error: -1
 *  ------------------------------------------------------------------------------------------------------- */
static int register_handler(timer_t *timer,
                            int signo,
                            int sec, int msec,      /* time to first */
                            int isec, int imsec,    /* periodic interval */
                            void (*handler)(int, siginfo_t *, void *))
{
    struct sigevent se;
    struct itimerspec its;
    struct sigaction sa;  

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;                 // can use 'sa_sigaction' function with three arguments
    sa.sa_sigaction = handler;                // signal handler designate
    sigemptyset(&sa.sa_mask);                 // clear all blocked signal set (sa_mask : signal to block set when sig_handler is operating)
    if (sigaction(signo, &sa, NULL) == -1) {  // signal process 
        perror("sigaction");
    }

    /* Set and enable alarm */
    se.sigev_notify = SIGEV_SIGNAL;           // Notification method (send signal in sigev_signo)
    se.sigev_signo = signo;                   // Notification signal
    se.sigev_value.sival_ptr = timer;         // Data passed with notification (Pointer value)
    timer_create(CLOCK_REALTIME, &se, timer); // (timer clock id : real-time, time event, generated time id);

    its.it_value.tv_sec = sec;                // it_value: the first expiration date
    its.it_value.tv_nsec = msec * 1000000;
    its.it_interval.tv_sec = isec;            // it_interval: period of timer
    its.it_interval.tv_nsec = imsec * 1000000;   
    /* Set the timer interval time for the returned timer id. */
    timer_settime(*timer, 0, &its, NULL); // (time id, feature flag, new_value(setting value), old_value);

    return 0;
}

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustrates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
  /* Unused arguments */
  (void)(argc);
  (void)(argv);

  /* Initialize model */
  Sense_initialize();
  Compute_initialize();
  Actuate_initialize();

  register_handler(&timer_100ms, SIGRTMIN, 1, 0, 0, 100, &handler_100ms); //100ms
  
  printf("System runs...\n");
  fflush((NULL));

  while (rtmGetErrorStatus(Sense_M) == (NULL) &&
         rtmGetErrorStatus(Compute_M) == (NULL) &&
         rtmGetErrorStatus(Actuate_M) == (NULL))
  {
      /*  Perform other application tasks here */
  }

  /* Disable rt_OneStep() here */

  /* Terminate model */
  Sense_terminate();
  Compute_terminate();
  Actuate_terminate();
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */