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

struct RgstrPrmHRTIM cmp = { 4352, 96, 2054, 2272, 4230, 96, 2054, 2272, 4230 };

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
				char *string[10];
				sprintf(string, "Frequency: %d", value);
				serial_print(string);
			} else {
				serial_print("Syntax error\n");
			}
		}
		/*
		 * DUTY
		 */
		else if (serial_is_command("duty", 1)) {
			if (serial_get_int(2, &value)) {
				char *string[10];
				sprintf(string, "Duty: %d", value);
				serial_print(string);
			} else {
				serial_print("Syntax error\n");
			}
		}
		/*
		 * DEAT TIME
		 */
		else if (serial_is_command("dt", 1)) {
			if (serial_get_int(2, &value)) {
				char *string[10];
				sprintf(string, "Dead time: %d", value);
				serial_print(string);
			} else {
				serial_print("Syntax error\n");
			}
		}
		else {
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
			serial_print("A long print should be performed\n");
		}
	}

	/*
	 * START
	 */
	else if (serial_is_command("start", 0)) {
		serial_print("Start converter\n");
	}

	/*
	 * STOP
	 */
	else if (serial_is_command("stop", 0)) {
		serial_print("Stop converter\n");
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

	pc_set_cmps(&cmp);
	pc_start();
}


void loop(void) {
	serial_parser_worker(&clParser);

	if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) { // reads GPIO User Button status

		HAL_Delay(200);
	}

}
