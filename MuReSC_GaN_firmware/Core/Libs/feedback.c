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

uint16_t fb_adc_out = 0;
uint16_t fb_adc_in = 0;

// HAL_ADC_Stop_IT(ADC_HandleTypeDef *hadc)
// HAL_ADC_IRQHandler
// REconfigure watchdog HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef *hadc, ADC_AnalogWDGConfTypeDef *AnalogWDGConfig)
// void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)


void feedback_init(void) {
	// Calibration ADC
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
//	HAL_ADC_Start_DMA(&hadc2, (uint32_t *) fb_adc_in , 1);

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	fb_adc_out = HAL_ADC_GetValue(&hadc1);
	serial_print("ADC READ");
	serial_nl();
	fb_adc_in = 0;
}
