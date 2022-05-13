/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Compute.c
 *
 * Code generated for Simulink model 'Compute'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sun Nov 21 19:55:16 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Compute.h"
#include "Compute_private.h"

/* External inputs (root inport signals with default storage) */
ExtU_Compute_T Compute_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Compute_T Compute_Y;

/* Real-time model */
RT_MODEL_Compute_T Compute_M_;
RT_MODEL_Compute_T *const Compute_M = &Compute_M_;
static void rate_scheduler(void);

/*
 *   This function updates active task flag for each subrate.
 * The function is called at model base rate, hence the
 * generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (Compute_M->Timing.TaskCounters.TID[1])++;
  if ((Compute_M->Timing.TaskCounters.TID[1]) > 1) {/* Sample time: [0.2s, 0.0s] */
    Compute_M->Timing.TaskCounters.TID[1] = 0;
  }
}

/* Model step function */
void Compute_step(void)
{
  if (Compute_M->Timing.TaskCounters.TID[1] == 0) {
    /* Outputs for Atomic SubSystem: '<Root>/Compute' */
    /* Outport: '<Root>/out' incorporates:
     *  Gain: '<S1>/Gain'
     *  Inport: '<Root>/in'
     */
    Compute_Y.out = 2.0 * Compute_U.in;

    /* End of Outputs for SubSystem: '<Root>/Compute' */
  }

  rate_scheduler();
}

/* Model initialize function */
void Compute_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void Compute_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
