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
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1,
	DAC_ALIGN_12B_R, 0);

	// Start comparator
	HAL_COMP_Start(&hcomp1);
}

void feedback_uninit(void) {
	HAL_COMP_Stop(&hcomp1);
}

void fb_set_vout(float vout) {
	float scaled_voltage = vout * FB_OUT_RATIO/1000;
	uint16_t dac_value = fb_get_dac_level(scaled_voltage);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
}

void fb_set_ref_out(float ref_out) {
	ref_out /= 1000;
	uint16_t dac_value = fb_get_dac_level(ref_out);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
}

int fb_get_dac_level(float voltage){
	float ela;
	ela = voltage*4095;
	ela = ela / 3.3;
	int level = round(ela);

	if(level > 4095)
		level = 4095;

	return(level);
}

// Handler feedback for on-off
void fb_handler_oo(void) {
	// If higher than threshold stop converter
	if (HAL_COMP_GetOutputLevel(&hcomp1) == COMP_OUTPUT_LEVEL_HIGH) {
		pc_stop();
	}

	// If lower start converter again
	else {
		pc_start();
	}
}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {
	// Output
	if (hcomp->Instance == COMP1) {
		if (fb_mode == ON_OFF) {
			fb_handler_oo();
		}

//		char *string[20];
//		sprintf(string, "STATO CMP: %d", HAL_COMP_GetOutputLevel(&hcomp2) == COMP_OUTPUT_LEVEL_HIGH);
//		serial_print(string);
//		serial_nl();
	}
}
