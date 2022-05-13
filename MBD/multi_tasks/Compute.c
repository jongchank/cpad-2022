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
 * C/C++ source code generated on : Sun Nov 21 20:18:20 2021
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

/* Model step function */
void Compute_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/Compute' */
  /* Outport: '<Root>/out' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/in'
   */
  Compute_Y.out = 2.0 * Compute_U.in;

  /* End of Outputs for SubSystem: '<Root>/Compute' */
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
