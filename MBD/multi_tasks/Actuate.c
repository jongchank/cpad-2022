/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Actuate.c
 *
 * Code generated for Simulink model 'Actuate'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sun Nov 21 20:18:47 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Actuate.h"
#include "Actuate_private.h"

/* External inputs (root inport signals with default storage) */
ExtU_Actuate_T Actuate_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Actuate_T Actuate_Y;

/* Real-time model */
RT_MODEL_Actuate_T Actuate_M_;
RT_MODEL_Actuate_T *const Actuate_M = &Actuate_M_;

/* Model step function */
void Actuate_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/Actuate' */
  /* Abs: '<S1>/Abs' incorporates:
   *  Inport: '<Root>/in'
   */
  Actuate_Y.out = fabs(Actuate_U.in);

  /* Saturate: '<S1>/Saturation' */
  if (Actuate_Y.out > 1.5) {
    /* Abs: '<S1>/Abs' incorporates:
     *  Outport: '<Root>/out'
     */
    Actuate_Y.out = 1.5;
  }

  /* End of Saturate: '<S1>/Saturation' */
  /* End of Outputs for SubSystem: '<Root>/Actuate' */
}

/* Model initialize function */
void Actuate_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void Actuate_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
