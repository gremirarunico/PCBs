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
#include <stdbool.h>

// Values stored for the ADC reads
uint16_t fb_adc_out = 0;
uint16_t fb_adc_in = 0;

// Values stored for the ADC calibration offset
int fb_adc_cal_off_out = 0;
int fb_adc_cal_off_in = 0;

// Target value for control of the output (calibration error inserted)
uint16_t fb_adc_out_target = 0;

// For on-off control hysteresis in mV
uint16_t fb_adc_out_hysteresis = 100 * FB_ADC_MAX_VALUE / FB_ADC_MAX_VOLTAGE
		* FB_OUT_RATIO / 1000;

fb_mode_t fb_mode = OPEN_LOOP;

// HAL_ADC_Stop_IT(ADC_HandleTypeDef *hadc)
// HAL_ADC_IRQHandler
// REconfigure watchdog HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef *hadc, ADC_AnalogWDGConfTypeDef *AnalogWDGConfig)
// void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)

void feedback_init(void) {
	// Calibration ADC ADC_CALIB_OFFSET
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
		/* Calibration Error */
		Error_Handler();
	}
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK) {
		/* Calibration Error */
		Error_Handler();
	}

	// Start ADC conversion
	HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_IT(&hadc2);
//	HAL_ADC_Start_DMA(&hadc2, (uint32_t *) fb_adc_in , 1);

// start pwm generation
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == &hadc1) {
		fb_adc_out = HAL_ADC_GetValue(&hadc1);
	}
	if (hadc == &hadc2) {
		fb_adc_in = HAL_ADC_GetValue(&hadc2);
	}

	// If feedback enabled manage it
	if (fb_mode == ON_OFF) {
		fb_handler_oo();
	}
}

// Handler feedback for on-off
void fb_handler_oo(void) {
	static bool converter_status = 1;
	// If higher than treshold stop converter
	if (fb_adc_out > fb_adc_out_target + fb_adc_out_hysteresis && converter_status) {
		pc_stop();
		converter_status = 0;
	}

	// If lower start converter again
	if (fb_adc_out < fb_adc_out_target - fb_adc_out_hysteresis && !converter_status) {
		pc_start();
		converter_status = 1;
	}
}

float fb_get_adc_out(void) {
	return ((fb_adc_out + fb_adc_cal_off_out) * FB_ADC_MAX_VOLTAGE
			/ FB_OUT_RATIO / FB_ADC_MAX_VALUE);
}

float fb_get_adc_in(void) {
	return ((fb_adc_in + fb_adc_cal_off_in) * FB_ADC_MAX_VOLTAGE / FB_IN_RATIO
			/ FB_ADC_MAX_VALUE);
}
