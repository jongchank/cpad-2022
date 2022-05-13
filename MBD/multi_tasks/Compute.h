/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Compute.h
 *
 * Code generated for Simulink model 'Compute'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
 * C/C++ source code generated on : Sat Nov 20 13:56:48 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Compute_h_
#define RTW_HEADER_Compute_h_
#ifndef Compute_COMMON_INCLUDES_
# define Compute_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Compute_COMMON_INCLUDES_ */

#include "Compute_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T in;                           /* '<Root>/in' */
} ExtU_Compute_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T out;                          /* '<Root>/out' */
} ExtY_Compute_T;

/* Real-time Model Data Structure */
struct tag_RTM_Compute_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_Compute_T Compute_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Compute_T Compute_Y;

/* Model entry point functions */
extern void Compute_initialize(void);
extern void Compute_step(void);
extern void Compute_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Compute_T *const Compute_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Gain' : Eliminated nontunable gain of 1
 */

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
 * hilite_system('example/Compute')    - opens subsystem example/Compute
 * hilite_system('example/Compute/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'example'
 * '<S1>'   : 'example/Compute'
 */
#endif                                 /* RTW_HEADER_Compute_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
