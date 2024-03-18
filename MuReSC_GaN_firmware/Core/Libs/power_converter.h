/*
 * power_converter.h
 *
 *  Created on: Jan 19, 2024
 *      Author: guerrinm
 */

#ifndef LIBS_POWER_CONVERTER_H_
#define LIBS_POWER_CONVERTER_H_

/**
 * @def PC_MINIMUM_COUNTER
 * @brief the minimum value the comparator counter can manage (it depends on the micro)
 *
 */
#define PC_MINIMUM_COUNTER 96
#define PC_MAXIMUS_COUNTER 65503
#define PC_HRTIM_EQ_CLK_FRQ 5.44e9 // 5.44G Hz
#define PC_MINIMUM_DT 20

#include "main.h"
#include <stdbool.h>

/**
 * Handler for high resolution timer
 */
extern HRTIM_HandleTypeDef hhrtim1;
extern bool pc_output_status;

typedef enum {
	MONO_RESONANT, MULTI_RESONANT
} pc_mode_t;

extern pc_mode_t pc_mode;

/*************************
 * @struct
 * @brief contains all the important informations about waveforms for the MuReSC converter
 *
 ************************/
struct WaveformParams {
	unsigned int frequency;
	unsigned int deadTime;
	unsigned int dutyCycle;
	unsigned int aDeadTime;
};

/**
 * @struct ComparatorHRTIM
 * @brief contains all the values for the comparators of the HRTIM
 *
 */
struct RgstrPrmHRTIM {
	unsigned int period;
	unsigned int A1;
	unsigned int A2;
	unsigned int B1;
	unsigned int B2;
	unsigned int C1;
	unsigned int C2;
	unsigned int D1;
	unsigned int D2;
};

/**
 * @fn void pc_set_cmps(struct ComparatorHRTIM*)
 * @brief set the comparators of the HRTIM, pass a pointer of a data struct params
 *
 * @pre
 * @post
 * @param params
 */
void pc_set_cmps(struct RgstrPrmHRTIM *params);

/**
 * @fn void pc_start(void)
 * @brief start the hrtim output (enable power conversion)
 *
 * @pre
 * @post
 */
void pc_start(void);

/**
 * @fn void pc_stop(void)
 * @brief stop the hrtim output (disable power conversion)
 *
 * @pre
 * @post
 */
void pc_stop(void);

/**
 * @fn void pc_update(struct RgstrPrmHRTIM*)
 * @brief
 *
 * @pre
 * @post
 * @param params
 */
void pc_update(struct RgstrPrmHRTIM *params);

void pc_calculator_cmp_mono_resonant(struct WaveformParams *waveform,
		struct RgstrPrmHRTIM *params);

void pc_calculator_cmp_multi_resonant(struct WaveformParams *waveform,
		struct RgstrPrmHRTIM *params);

#endif /* LIBS_POWER_CONVERTER_H_ */
