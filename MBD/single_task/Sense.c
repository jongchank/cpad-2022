/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Sense.c
 *
 * Code generated for Simulink model 'Sense'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sun Nov 21 19:54:57 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Sense.h"
#include "Sense_private.h"

/* Block states (default storage) */
DW_Sense_T Sense_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Sense_T Sense_Y;

/* Real-time model */
RT_MODEL_Sense_T Sense_M_;
RT_MODEL_Sense_T *const Sense_M = &Sense_M_;

/* Model step function */
void Sense_step(void)
{
  real_T lastSin_tmp;

  /* Outputs for Atomic SubSystem: '<Root>/Sense' */
  /* Sin: '<S1>/Sine Wave' */
  if (Sense_DW.systemEnable != 0) {                     
    lastSin_tmp = ((Sense_M->Timing.clockTick0) * 0.1);
    Sense_DW.lastSin = sin(lastSin_tmp);
    Sense_DW.lastCos = cos(lastSin_tmp);
    Sense_DW.systemEnable = 0;
  }

  /* Outport: '<Root>/out' incorporates:
   *  Sin: '<S1>/Sine Wave'
   */
  Sense_Y.out = (Sense_DW.lastSin * 0.99500416527802571 + Sense_DW.lastCos *
                 -0.099833416646828155) * 0.99500416527802571 +
    (Sense_DW.lastCos * 0.99500416527802571 - Sense_DW.lastSin *
     -0.099833416646828155) * 0.099833416646828155;

  /* Update for Sin: '<S1>/Sine Wave' */
  lastSin_tmp = Sense_DW.lastSin;
  Sense_DW.lastSin = Sense_DW.lastSin * 0.99500416527802571 + Sense_DW.lastCos *
    0.099833416646828155;
  Sense_DW.lastCos = Sense_DW.lastCos * 0.99500416527802571 - lastSin_tmp *
    0.099833416646828155;

  /* End of Outputs for SubSystem: '<Root>/Sense' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.1, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  Sense_M->Timing.clockTick0++;
}

/* Model initialize function */
void Sense_initialize(void)
{
  /* Enable for Atomic SubSystem: '<Root>/Sense' */
  /* Enable for Sin: '<S1>/Sine Wave' */
  Sense_DW.systemEnable = 1;

  /* End of Enable for SubSystem: '<Root>/Sense' */
}

/* Model terminate function */
void Sense_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
