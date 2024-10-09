/**
 ******************************************************************************
 * @file         feedback.c
 * @brief        This file provides code for sensing
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Marco Guerrini.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "feedback.h"
#include "power_converter.h"
#include <stdbool.h>
#include <math.h>

fb_mode_t fb_mode = OPEN_LOOP;

void feedback_init(void) {
	// Start DAC
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2,
	DAC_ALIGN_12B_R, 0);

	// Start comparator
	HAL_COMP_Start(&hcomp2);
}

void feedback_uninit(void) {
	HAL_COMP_Stop(&hcomp2);
}

void fb_set_vout(float vout) {
	/* USER CODE BEGIN 3 */
	float scaled_voltage = vout * FB_OUT_RATIO;
	int dac_value = fb_get_dac_level(scaled_voltage);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
}

int fb_get_dac_level(float voltage){
	int level = round(voltage*4095/3.3);
	return(level);
}

// Handler feedback for on-off
void fb_handler_oo(void) {
	static bool converter_status = 1;
	// If higher than threshold stop converter
	if (HAL_COMP_GetOutputLevel(&hcomp2) == COMP_OUTPUT_LEVEL_HIGH) {
		pc_stop();
		converter_status = 0;
	}

	// If lower start converter again
	else {
		pc_start();
		converter_status = 1;
	}
}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {
	if (fb_mode == ON_OFF) {
		fb_handler_oo();
	}
}
