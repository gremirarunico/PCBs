/**
 ******************************************************************************
 * @file           : feedback.h
 * @brief          : Header for feedback.c file.
 *                   This file contains the common defines for sensing.
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
#include "main.h"

#ifndef LIBS_FEEDBACK_H_
#define LIBS_FEEDBACK_H_

/**
 * @def FB_IN_RATIO
 * @brief The resistor partitor ratio R1/(R1+R2), so Vadc = FB_IN_RATIO * Vin => Vin = Vadc/FB_IN_RATIO
 * In our circuit R1 = 5.76 kOhm, R2 = 100 kOhm
 *
 */
#define FB_IN_RATIO 36/661
/**
 * @def FB_OUT_RATIO
 * @brief The resistor partitor ratio R1/(R1+R2), so Vadc = FB_OUT_RATIO * Vin => Vin = Vadc/FB_IN_RATIO
 * In our circuit R1 = 7.41 kOhm, R2 = 100 kOhm
 *
 */
#define FB_OUT_RATIO 741/10741

#define FB_ADC_MAX_VALUE 4095
#define FB_ADC_MAX_VOLTAGE 3.3

//extern ADC_HandleTypeDef hadc1;
//extern ADC_HandleTypeDef hadc2;

//extern COMP_HandleTypeDef hcomp1;

extern DAC_HandleTypeDef hdac1;
extern COMP_HandleTypeDef hcomp1;

//extern TIM_HandleTypeDef htim1;


typedef enum {
	OPEN_LOOP, ON_OFF
} fb_mode_t;

extern fb_mode_t fb_mode;

void feedback_init(void);
void feedback_uninit(void);
void fb_set_vout(float vout);
void fb_set_ref_out(float ref_out);
int fb_get_dac_level(float voltage);
void fb_handler_oo(void);
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp);

#endif /* LIBS_FEEDBACK_H_ */
