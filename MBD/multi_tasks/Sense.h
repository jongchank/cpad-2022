/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Sense.h
 *
 * Code generated for Simulink model 'Sense'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sat Nov 20 13:56:23 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Sense_h_
#define RTW_HEADER_Sense_h_
#include <math.h>
#ifndef Sense_COMMON_INCLUDES_
# define Sense_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Sense_COMMON_INCLUDES_ */

#include "Sense_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T lastSin;                      /* '<S1>/Sine Wave' */
  real_T lastCos;                      /* '<S1>/Sine Wave' */
  int32_T systemEnable;                /* '<S1>/Sine Wave' */
} DW_Sense_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T out;                          /* '<Root>/out' */
} ExtY_Sense_T;

/* Real-time Model Data Structure */
struct tag_RTM_Sense_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
  } Timing;
};

/* Block states (default storage) */
extern DW_Sense_T Sense_DW;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Sense_T Sense_Y;

/* Model entry point functions */
extern void Sense_initialize(void);
extern void Sense_step(void);
extern void Sense_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Sense_T *const Sense_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('example/Sense')    - opens subsystem example/Sense
 * hilite_system('example/Sense/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'example'
 * '<S1>'   : 'example/Sense'
 */
#endif                                 /* RTW_HEADER_Sense_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
