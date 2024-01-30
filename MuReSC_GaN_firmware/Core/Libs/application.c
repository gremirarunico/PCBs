/**
 ******************************************************************************
 * @file         application.c
 * @brief        This file provides code for the application
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
#include "application.h"
#include "serial_parser.h"
#include "power_converter.h"
#include "main.h"
//#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct WaveformParams waveform = { 1250000, 40, 50 };
struct RgstrPrmHRTIM htimpar = { 4352, 96, 2054, 2272, 4230, 96, 2054, 2272,
		4230 };

char *string[100];

void clParser(void) {
	int value = 0;
	/*
	 * SET
	 */
	if (serial_is_command("set", 0)) {
		/*
		 * FREQUENCY
		 */
		if (serial_is_command("frequency", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Frequency: %d", value);
				serial_print(string);
				waveform.frequency = value;
			} else {
				serial_print("Syntax error\n");
			}
		}
		/*
		 * DUTY
		 */
		else if (serial_is_command("duty", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Duty: %d", value);
				serial_print(string);
				waveform.dutyCycle = value;
			} else {
				serial_print("Syntax error\n");
			}
		}
		/*
		 * DEAT TIME
		 */
		else if (serial_is_command("dt", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Dead time: %d ns", value);
				serial_print(string);
				waveform.deadTime = value;
			} else {
				serial_print("Syntax error\n");
			}
		} else {
			serial_print("Syntax error\n");
		}

	}
	/*
	 * GET
	 */
	else if (serial_is_command("get", 0)) {
		/*
		 * ALL
		 */
		if (serial_is_command("all", 1)) {
			sprintf(string, "F: %d Hz, DT: %d ns, Duty: %d%%",
					waveform.frequency, waveform.deadTime, waveform.dutyCycle);
			serial_print(string);

			sprintf(string,
					"P: %d, A1: %d, A2: %d, B1: %d, B2: %d, C1: %d, C2: %d, D1: %d, D2: %d",
					htimpar.period, htimpar.A1, htimpar.A2, htimpar.B1,
					htimpar.B2, htimpar.C1, htimpar.C2, htimpar.D1, htimpar.D2);
			serial_print(string);
		}
	}

	/*
	 * UPDATE
	 */
	else if (serial_is_command("update", 0)) {
		pc_calculator_cmp(&waveform, &htimpar);
		pc_update(&htimpar);
		serial_print("Update sent\n");
	}

	/*
	 * START
	 */
	else if (serial_is_command("start", 0)) {
		pc_start();
		serial_print("Converter started\n");
	}

	/*
	 * STOP
	 */
	else if (serial_is_command("stop", 0)) {
		pc_stop();
		serial_print("Converter stopped\n");
	}

	/*
	 * ELSE
	 */
	else {
		serial_print("Syntax error\n");
	}

}

void setup(void) {
	serial_parser_init();

	pc_calculator_cmp(&waveform, &htimpar);
	pc_update(&htimpar);
}

void loop(void) {
	serial_parser_worker(&clParser);

	if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {

		HAL_Delay(200);
	}

}
