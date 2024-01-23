/**
 ******************************************************************************
 * @file         application.c
 * @brief        This file provides code serial management
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Bologna Design.
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
#include "main.h"
//#include <stdio.h>
#include <stdbool.h>

#include <string.h>


void test(void) {
	if (serial_is_command("prova", 0)) {
		serial_print("First command prova\n");
	} else if (serial_is_command("culo", 0)) {
		serial_print("First command culo\n");
	}

}

void setup(void) {
	serial_parser_init();

	// 1 B
	// 2 A
	// 3 D
	// 4 C

	// 1
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = 96;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR = 2054;

	//2
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = 2272;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = 4230;

	//3
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = 96;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = 2054;

	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = 2272;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP2xR = 4230;

	HAL_HRTIM_WaveformOutputStart(&hhrtim1,
			HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TB1
					+ HRTIM_OUTPUT_TB2 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TC2
					+ HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);
	HAL_HRTIM_WaveformCounterStart(&hhrtim1,
			HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B
					+ HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);
}

int i = 0;
void loop(void) {
	serial_parser_worker(&test);
	//		HAL_Delay(5000);
	//		HAL_HRTIM_WaveformOutputStop(&hhrtim1,
	//				HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TB1
	//						+ HRTIM_OUTPUT_TB2 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TC2
	//						+ HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);
	//		HAL_Delay(5000);
	//		HAL_HRTIM_WaveformOutputStart(&hhrtim1,
	//				HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TB1
	//						+ HRTIM_OUTPUT_TB2 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TC2
	//						+ HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);

	if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) { // reads GPIO User Button status
		i++;

//		if (i == 1) {
//			serial_print("Button pressed first time!\r");
//		} else if (i == 2) {
//			serial_print("Second time!\r");
//		} else {
//			char * buffer[20];
//			sprintf(buffer, "It's the %d th time you pressed!\r", i);
//			serial_print(buffer);
//		}
		HAL_Delay(200);
		//serial_parser_received_command();
	}

	// stop
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP2xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = 0;
	//		HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = 0;
}
